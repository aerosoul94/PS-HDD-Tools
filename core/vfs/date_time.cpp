#include "date_time.hpp"

namespace vfs {

VfsDateTime::VfsDateTime()
{
  init();
}

VfsDateTime::VfsDateTime(int day, int month, int year)
{
  init(day, month, year);
}

VfsDateTime::VfsDateTime(int day, int month, int year, 
  int second, int minute, int hour)
{
  init(day, month, year, second, minute, hour);
}

void VfsDateTime::init(int day, int month, int year, 
  int second, int minute, int hour)
{
  this->day = day;
  this->month = month;
  this->year = year;
  this->second = second;
  this->minute = minute;
  this->hour = hour;
}

void VfsDateTime::setDateTime(int day, int month, int year, 
  int second, int minute, int hour)
{
  init(day, month, year, second, minute, hour);
}

void VfsDateTime::setFromTm(const tm* t)
{
  this->day = t->tm_mday;
  this->month = t->tm_mon;
  this->year = t->tm_year;
  this->second = t->tm_sec;
  this->minute = t->tm_min;
  this->hour = t->tm_hour;
}

void VfsDateTime::setFromTime(const time_t* t)
{
  auto time = localtime(t);

  this->day = time->tm_mday;
  this->month = time->tm_mon + 1;
  this->year = time->tm_year + 1900;
  this->second = time->tm_sec;
  this->minute = time->tm_min;
  this->hour = time->tm_hour;
}

VfsDateTime* VfsDateTime::setDay(int day)
{
  this->day = day;
  return this;
}

VfsDateTime* VfsDateTime::setMonth(int month)
{
  this->month = month;
  return this;
}

VfsDateTime* VfsDateTime::setYear(int year)
{
  this->year = year;
  return this;
}

VfsDateTime* VfsDateTime::setSecond(int second)
{
  this->second = second;
  return this;
}

VfsDateTime* VfsDateTime::setMinute(int minute)
{
  this->minute = minute;
  return this;
}

VfsDateTime* VfsDateTime::setHour(int hour)
{
  this->hour = hour;
  return this;
}

int VfsDateTime::getDay() const
{
  return this->day;
}

int VfsDateTime::getMonth() const
{
  return this->month;
}

int VfsDateTime::getYear() const
{
  return this->year;
}

int VfsDateTime::getSecond() const
{
  return this->second;
}

int VfsDateTime::getMinute() const
{
  return this->minute;
}

int VfsDateTime::getHour() const
{
  return this->hour;
}

} /* namespace vfs */
