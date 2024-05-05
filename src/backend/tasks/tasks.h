#pragma once

#include "lib/src/Scheduler.h"

using namespace async_tasks;

enum class tasks{
  clock,
  wifi,
  wifiScan,
  wifiConn,
  error
};

/**
 * @brief Get semaphore for given task
 * @param task name of the task
 * @return Semaphore handle
*/
SemaphoreHandle_t getSemaphore(const tasks& task);