#include "Scheduler.h"

BEGIN_TASKS_NAMESPACE

bool _compareScheduledTask(const _ScheduledTask& a, const _ScheduledTask& b){
  return a.nextExecution > b.nextExecution;
}

_clock getNow(){
  return pdTICKS_TO_MS(xTaskGetTickCount());
}

time_t to_time_t(_clock time){
  return time_t(time / 1000);
}

// execute a task if this is the correct time, and return the time until the next execution in milliseconds
double Scheduler::_executeTask(Scheduler* scheduler, struct _ScheduledTask& task){
  if(task.nextExecution <= scheduler->_now){
    
    // make a copy of the task, and run it
    AsyncTask<>(task.task).run();
    // update the next execution time
    task.nextExecution = task.schedule.schedule(scheduler->_now);

    // time_t temp = to_time_t(task.nextExecution);
    // struct tm next = *localtime(&temp);

    // // Get the number of milliseconds
    // auto milliseconds = task.nextExecution % 1000;
    // Serial.println("Next execution " + String(task.task._params.name.c_str()) + ": " + String(next.tm_hour) + ":" + String(next.tm_min) + ":" + String(next.tm_sec) + ":" + String(milliseconds));
  } 
  // return the time until the next execution in milliseconds
  return task.nextExecution - scheduler->_now;
}

double Scheduler::_runLockedTask(Scheduler* scheduler){
  double minTime = INT_MAX;

  // time_t temp = to_time_t(scheduler->_now);
  // struct tm* now = localtime(&temp);

  // auto milliseconds = scheduler->_now % 1000;
  // Serial.println("Now: " + String(now->tm_hour) + ":" + String(now->tm_min) + ":" + String(now->tm_sec) + ":" + String(milliseconds));

  // lock the mutex
  Lock lock(scheduler->_taskData->_mutex);

  // iterate over all the tasks, and execute them
  for(auto it = scheduler->_tasks.begin(); it != scheduler->_tasks.end(); it++){
    minTime = std::min(minTime, _executeTask(scheduler, *it));
  }

  // sort the tasks by the next execution time
  scheduler->_tasks.sort(_compareScheduledTask);

  // Serial.println("Min time: " + String(minTime) + "\n");

  // update the current time
  scheduler->_now = getNow();

  // return the time until the next task in milliseconds
  return std::max(minTime, 10.0);
}

void Scheduler::_taskRunner(void* param){
  /*
  
  Main task for the `Scheduler` class.

  This task will run all the tasks in the `Scheduler`'s task list, and will
  sleep for the `frequency` time, and then run the tasks again.
  Basically, this is the main loop of the `Scheduler` class, where all the tasks
  are executed.
  
  */
  Scheduler* scheduler = static_cast<Scheduler*>(param);

  scheduler->_now = getNow();
  
  // The frequency of the task runner, using 50ms as the default
  TickType_t frequency = pdMS_TO_TICKS(50);
  TickType_t timer = xTaskGetTickCount();

  for(;;){
    vTaskDelayUntil(&timer, frequency);
    _runLockedTask(scheduler);
  }
}

Scheduler::Scheduler():
  _taskData(nullptr), _now(), _tasks(), _params() {}

Scheduler::Scheduler(const Scheduler& other):
  _now(other._now), _tasks(other._tasks), _params(other._params) {}

Scheduler::~Scheduler(){
  stop();
}

Scheduler& Scheduler::setParams(const TaskParams& params){
  _params = params;
  return *this;
}

Scheduler& Scheduler::addTask(const AsyncTask<>& task, ScheduleParams schedule){
  // Add a task to the list of tasks, user might have called `run` before adding tasks
  // so we must check if the taskData is initialized and use the mutex, same for the
  // other setter methods
  if (_taskData){
    Lock lock(_taskData->_mutex);
    _tasks.emplace_back(task, schedule);
  } else {
    _tasks.emplace_back(task, schedule);
  }
  return *this;
}

Scheduler& Scheduler::addTask(std::function<void(void)> task, const TaskParams& params, const ScheduleParams& schedule){
  return addTask(AsyncTask<>(params, task), schedule);
}

Scheduler& Scheduler::addTask(std::function<void(void)> task, const ScheduleParams& schedule){
  return addTask(AsyncTask<>(task), schedule);
}

void Scheduler::run(){

  // if the scheduler is already running, return
  if (_taskData != nullptr){
    return;
  }
  
  _taskData.reset(new _TaskData());

  _now = getNow();
  
  if(_params.usePinnedCore){
    xTaskCreatePinnedToCore(
      _taskRunner, "Scheduler", _params.stackSize, this, tskIDLE_PRIORITY, &_taskData->_handle, _params.core
    );
  } else{
    xTaskCreate(
      _taskRunner, "Scheduler", _params.stackSize, this, tskIDLE_PRIORITY, &_taskData->_handle
    );
  }
}

void Scheduler::execute(){
  _runLockedTask(this);
}

void Scheduler::stop(){
  if (_taskData == nullptr || _taskData->_signal != _TaskSignal::RUN){
    return;
  }
  Lock lock(_taskData->_mutex);
  vTaskDelete(_taskData->_handle);
  _taskData.reset();
}

void Scheduler::pause(){
  if (_taskData == nullptr || _taskData->_signal != _TaskSignal::RUN){
    return;
  }
  vTaskSuspend(_taskData->_handle);
  _taskData->_signal = _TaskSignal::PAUSE;
}

void Scheduler::resume(){
  if (_taskData == nullptr || _taskData->_signal != _TaskSignal::PAUSE){
    return;
  }
  _taskData->_signal = _TaskSignal::RUN;
  vTaskResume(_taskData->_handle);
}

END_TASKS_NAMESPACE