#pragma once

#include <Arduino.h>

#include "namespaces.h"
#include <functional>
#include <tuple>
#include <memory>

// `apply` implementation for tuples
#include "tuple.h"

BEGIN_TASKS_NAMESPACE

/**
  * Task parameters, used to create a task, has information about the task:
  * - stack size (default is 4096)
  * - priority (default is tskIDLE_PRIORITY)
  * - name (default is "Task")
  * - use pinned core (default is false)
  * - core (default is 0)
*/
struct TaskParams{
    // stack size, default is 4096
    int stackSize = 4096;

    // priority of the task, default is tskIDLE_PRIORITY, probably shouldn't be changed
    int priority = tskIDLE_PRIORITY;

    // name of the task, default is "Task"
    std::string name = "Task";

    // use pinned core, default is false
    bool usePinnedCore = false;

    // core to pin the task to (0 or 1), default is 0 (if usePinnedCore is true)
    int core = 0;
    
    TaskParams(
        int stackSize = 4096, 
        int priority = tskIDLE_PRIORITY, 
        std::string name = "Task", 
        bool usePinnedCore = false, 
        int core = 0
    ): stackSize(stackSize), priority(priority), name(name),
    usePinnedCore(usePinnedCore), core(core) {}
    
    TaskParams(const TaskParams& other){
        *this = other;
    }

    TaskParams& operator=(const TaskParams& other){
        stackSize = other.stackSize;
        priority = other.priority;
        name = other.name;
        usePinnedCore = other.usePinnedCore;
        core = other.core;
        return *this;
    }

    TaskParams& setStackSize(int size){
        stackSize = size;
        return *this;
    }

    TaskParams& setPriority(int p){
        priority = p;
        return *this;
    }

    TaskParams& setName(const std::string& n){
        name = n;
        return *this;
    }

    TaskParams& setUsePinnedCore(bool use){
        usePinnedCore = use;
        return *this;
    }

    TaskParams& setCore(int c){
        core = c;
        return *this;
    }
};


/**
 * ## TaskSignal
 * 
 * An enum class that represents the signals that can be sent to a task.
*/
enum class _TaskSignal{
    STOP = 1,
    PAUSE = 2,
    RUN = 3,
};

/**
 * Task data, used to store the task handle.
 * 
 * To pass the `TaskHandle_t` to the task function, we need to store it in the heap.
 * Also, we need to store the signal that was sent to the task, and a mutex to protect
 * the signal
*/
struct _TaskData{
    TaskHandle_t _handle;
    _TaskSignal _signal;
    SemaphoreHandle_t _mutex;

    _TaskData(TaskHandle_t handle = NULL, _TaskSignal signal = _TaskSignal::RUN):
        _handle(handle), _signal(signal), _mutex(xSemaphoreCreateMutex()) {}
    ~_TaskData(){
        if (_mutex){
            vSemaphoreDelete(_mutex);
        }
    }
};


template <typename... _ArgTypes>
class AsyncTask;

/**
 * @brief Delete the task and free the memory
 * @param p The task to be deleted
 * @param kill If true, the task will be killed, otherwise it will be left running
 * and returns the handle
*/
template <typename... _ArgTypes>
TaskHandle_t _deleteTask(AsyncTask<_ArgTypes...>* p, bool kill){
    TaskHandle_t handle = NULL;

    if (p->_data){
        handle = p->_data->_handle;
    }

    delete p->_data;
    delete p;

    if (handle && kill){
        vTaskDelete(handle);
    }

    return handle;
}

/**
 * Base class for AsyncTask, used to store parameters and the task data
*/
class BaseAsyncTask{
  public:
    BaseAsyncTask(const TaskParams& params): _params(params), _data(nullptr) {}
    BaseAsyncTask(): BaseAsyncTask(TaskParams()) {}
    virtual ~BaseAsyncTask() = default;

    /**
     * @brief Stop the task, this will send a signal to the task to stop it, doesn't work immediately
    */
    void stop();

    /**
     * @brief Pause the task, you can resume the task using `resume()`
    */
    void pause();

    /**
     * @brief Resume the task, after it was paused
    */
    void resume();

  protected:
    /**
     * @brief Wrapper for the task function, casts the task, runs it and deletes it
    */
    template <typename _Res, typename... _ArgTypes>
    static void _taskWrapper(void *param){
        // Get the task from the parameter and cast it to the correct type.
        // This is probably the only way to do this, since we can't use lambdas
        // as task functions, and we can't use static_cast with lambdas.
        // The task also must be allocated on the heap, since there's a possibility
        // that the task will be deleted before it's done if it was on stack memory.
        auto task = static_cast<AsyncTask<_ArgTypes...>*>(param);
        
        if (!task){
            return;
        }

        // First, check if the task was stopped, using a mutex
        if (xSemaphoreTake(task->_data->_mutex, portMAX_DELAY) == pdTRUE){
            // Must store the mutex in a local variable, since the task can be deleted
            SemaphoreHandle_t mutex = task->_data->_mutex;
            // If the task was stopped, delete it and return
            if (task->_data->_signal == _TaskSignal::STOP){
                TaskHandle_t handle = _deleteTask<_ArgTypes...>(task, false);
                xSemaphoreGive(mutex);
                if (handle){
                    vTaskDelete(handle);
                }
                return;
            }
            xSemaphoreGive(mutex);
        }

        // Run the task on the current thread
        task->_runTask();
        
        // Delete the task after it's done, must be called after the task is done,
        // also terminates the FreeRTOS task
        _deleteTask<_ArgTypes...>(task, true);
    }

    template <typename... _ArgTypes>
    friend TaskHandle_t _deleteTask(AsyncTask<_ArgTypes...>* p, bool kill);
    friend class Scheduler;

    _TaskData *_data;
    TaskParams _params;
};


/**
 * ## AsyncTask
 * 
 * A class that represents a task that can be run in the background.
 * The task is a one-time task, it can't be run multiple times.
 * 
*/
template <typename... _ArgTypes>
class AsyncTask : public BaseAsyncTask{

    using _TaskType = std::function<void(_ArgTypes...)>;

    _TaskType _task;
    std::tuple<_ArgTypes...> _args;

public:
    AsyncTask() = default;
    AsyncTask(_TaskType task);
    AsyncTask(const TaskParams& params);
    AsyncTask(
      const TaskParams& params, 
      _TaskType task
    );
    AsyncTask(const AsyncTask& other);

    inline AsyncTask& setParams(const TaskParams& params){
        _params = params;
        return *this;
    }

    inline AsyncTask& setTask(_TaskType task){
        _task = task;
        return *this;
    }

    /**
     * @brief Run the task in the background
    */
    inline void run(_ArgTypes... args){
        // If the task is already running, don't run it again
        if (_data){
            return;
        }
        _args = std::make_tuple(args...);
        if (_task){
            _data = new _TaskData();
            if (_params.usePinnedCore){
                xTaskCreatePinnedToCore(
                    _taskWrapper<void, _ArgTypes...>, _params.name.c_str(), _params.stackSize,
                    copy(), _params.priority, &_data->_handle, _params.core
                );
                return;
            }
            xTaskCreate(
                _taskWrapper<void, _ArgTypes...>, _params.name.c_str(), _params.stackSize,
                copy(), _params.priority, &_data->_handle
            );
        }
    }
    
    /**
     * @brief Same as `run(...)`, but with operator overloading
    */
    inline void operator()(_ArgTypes... args){
        run(args...);
    }

    /**
     * @brief Run the task in the current thread, used internally
    */
    inline void _runTask(){
      if (_task){
          apply(_task, _args);
      }
    }

    AsyncTask& operator=(const AsyncTask& other){
        _params = other._params;
        _task = other._task;
        _args = other._args;
        return *this;
    }

    /**
     * @brief Copy the task to the heap, used internally
    */
    AsyncTask* copy() const{
        auto ptr = new AsyncTask(*this);
        ptr->_data = _data;
        return ptr;
    }
};

template <typename... _ArgTypes>
AsyncTask<_ArgTypes...>::AsyncTask(_TaskType task): 
    AsyncTask(TaskParams(), task) {}

template <typename... _ArgTypes>
AsyncTask<_ArgTypes...>::AsyncTask(const TaskParams& params) : 
    AsyncTask(params, nullptr) {}

template <typename... _ArgTypes>
AsyncTask<_ArgTypes...>::AsyncTask(
  const TaskParams& params, 
  _TaskType task
) : BaseAsyncTask(params), _task(task) {}

template <typename... _ArgTypes>
AsyncTask<_ArgTypes...>::AsyncTask(const AsyncTask& other){
    *this = other;
}

/**
 * ## AsyncTask
 * 
 * A class that represents a task that can be run in the background.
 * The task is a one-time task, it can't be run multiple times.
 * 
*/
template <>
class AsyncTask<> : public BaseAsyncTask{

    using _TaskType = std::function<void()>;

    _TaskType _task;

public:
    AsyncTask();
    AsyncTask(_TaskType task);
    AsyncTask(const TaskParams& params);
    AsyncTask(const TaskParams& params, _TaskType task);
    AsyncTask(const AsyncTask& other);

    AsyncTask& setParams(const TaskParams& params);
    AsyncTask& setTask(_TaskType task);

    /**
     * @brief Run the task in the background
    */
    void run();

    /**
     * @brief Same as `run()`, but with operator overloading
    */
    void operator()();

    /**
     * @brief Run the task in the current thread, used internally
    */
    void _runTask();    

    AsyncTask& operator=(const AsyncTask& other);

    /**
     * @brief Copy the task to the heap, used internally
    */
    AsyncTask* copy() const;
};


END_TASKS_NAMESPACE