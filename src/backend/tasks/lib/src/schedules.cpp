#include "schedules.h"

BEGIN_TASKS_NAMESPACE

ScheduleParams& ScheduleParams::every(int amount, TimeUnit unit){
  this->amount = amount;
  this->unit = unit;
  return *this;
}

using time_point = uint32_t;

time_point ScheduleParams::schedule(time_point now){
  return updateTime(now, amount, unit);
}

time_point updateTime(time_point now, int amount, TimeUnit unit){
  using namespace std::chrono;
  high_resolution_clock::duration multiplier;
  switch(unit){
    case TimeUnit::Milliseconds:
      multiplier = milliseconds(1);
      break;
    case TimeUnit::Seconds:
      multiplier = seconds(1);
      break;
    case TimeUnit::Minutes:
      multiplier = minutes(1);
      break;
    case TimeUnit::Hours:
      multiplier = hours(1);
      break;
    case TimeUnit::Days:
      multiplier = hours(24);
      break;
    default:
      break;
  }
  return now + (duration_cast<milliseconds>(amount * multiplier)).count();
}

END_TASKS_NAMESPACE
