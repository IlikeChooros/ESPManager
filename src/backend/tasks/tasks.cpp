#include "tasks.h"

SemaphoreHandle_t taskClockMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t taskWifiScanMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t taskWifiConnectMutex = xSemaphoreCreateMutex();
SemaphoreHandle_t errorMutex = xSemaphoreCreateMutex();

SemaphoreHandle_t getSemaphore(const tasks& task){
  switch (task)
  {
  case tasks::clock:
    return taskClockMutex;
  case tasks::wifiScan:
    return taskWifiScanMutex;
  case tasks::wifiConn:
    return taskWifiConnectMutex;
  case tasks::error:
    return errorMutex;
  default:
    return NULL;
  }
}