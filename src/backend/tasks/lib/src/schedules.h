#pragma once

#include <ctime>
#include <chrono>
#include "namespaces.h"

BEGIN_TASKS_NAMESPACE

/*

Time unit to be used in the `ScheduleParams` class.
Implements the time units: Seconds, Minutes, Hours, Days

*/
enum class TimeUnit{
  Milliseconds = 0,
  Seconds = 1,
  Minutes = 2,
  Hours = 3,
  Days = 4,
};

struct ScheduleParams{

  using time_point = uint32_t;

  int amount;
  TimeUnit unit;

  ScheduleParams(int amount = 60, TimeUnit unit = TimeUnit::Seconds):
    amount(amount), unit(unit) {}

  /**
   * Schedule the task to be executed every `amount` of `unit`
  */
  ScheduleParams& every(int amount, TimeUnit unit = TimeUnit::Seconds);

  time_point schedule(time_point now);
};

uint32_t updateTime(uint32_t now, int amount, TimeUnit unit);

END_TASKS_NAMESPACE
