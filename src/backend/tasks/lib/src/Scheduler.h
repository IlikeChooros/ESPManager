#pragma once

#include <queue>
#include <list>

#include "./AsyncTask.h"
#include "./schedules.h"
#include "./lock.h"

BEGIN_TASKS_NAMESPACE

using _clock = uint32_t;

struct _ScheduledTask{
  // task is the task to be executed
  AsyncTask<> task;
  // schedule for the task
  ScheduleParams schedule;
  // nextExecution is used to store the next time the task should be executed
  _clock nextExecution;

  _ScheduledTask(): _ScheduledTask(AsyncTask<>(), ScheduleParams()) {}

  _ScheduledTask(const AsyncTask<>& task, const ScheduleParams& schedule):
    task(task), schedule(schedule), nextExecution(0) {}

  _ScheduledTask(const _ScheduledTask& other):
    task(other.task), schedule(other.schedule), nextExecution(other.nextExecution) {}
};

/*

## Scheduler

The Scheduler class is used to schedule tasks to be executed at a certain time. 


### Example

```cpp

Scheduler scheduler;
// add a task to the scheduler, that prints "Hello, World!" every second
scheduler.addTask(AsyncTask<>([](){
  Serial.println("Hello, World!");
}), ScheduleParams().every(1, TimeUnit::Seconds));


// run the scheduler
scheduler.run();
```
*/
class Scheduler
{
  std::unique_ptr<_TaskData> _taskData;
  _clock _now;
  std::list<struct _ScheduledTask> _tasks;
  TaskParams _params;

  // execute the task, and return the time until the next task in seconds
  static double _executeTask(Scheduler* scheduler, struct _ScheduledTask& task);

  // task runner, main task for the `Scheduler` class
  static void _taskRunner(void* param);

  // run the tasks, and return the time until the next task in milliseconds
  static double _runLockedTask(Scheduler* scheduler);

  public:
  // Create a new scheduler
  // @throws std::runtime_error if the scheduler is already created, 
  // only one instance of `Scheduler` is allowed
  Scheduler();
  ~Scheduler();
  Scheduler(const Scheduler&);

  /**
   * @brief Set the parameters for the scheduler task
   * @param params The parameters to be set
   * @return *this
  */
  Scheduler& setParams(const TaskParams& params);

  /**
   * @brief Add a task to the scheduler
   * @param task The `AsyncTask` to be added
   * @param schedule The schedule of the task
   * @return *this
  */
  Scheduler& addTask(const AsyncTask<>& task, ScheduleParams schedule);

  /**
   * @brief Add a task to the scheduler
   * @param task function task to be added
   * @param schedule The schedule of the task
   * @return *this
  */
  Scheduler& addTask(std::function<void(void)> task, const ScheduleParams& schedule);

  /**
   * @brief Add a task to the scheduler
   * @param task function task to be added
   * @param params The parameters of the task
   * @param schedule The schedule of the task
   * @return *this
  */
  Scheduler& addTask(
    std::function<void(void)> task, 
    const TaskParams& params, 
    const ScheduleParams& schedule
  );

  /**
   * @brief Run the scheduler asynchronusly, and start executing the tasks
  */
  void run();

  /**
   * @brief Same as `run()`, but executes in the current thread,
   *  should be called frequently
  */
  void execute();

  /**
   * @brief Stop the scheduler, killing all the tasks, must call `run` to start again
  */
  void stop();

  /**
   * @brief Pause the scheduler, the tasks will not be executed, 
   * doesn't affect already working tasks
  */
  void pause();

  /**
   * @brief Resume the scheduler, after it was paused
  */
  void resume();
};

END_TASKS_NAMESPACE
