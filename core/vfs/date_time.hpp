#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP

#include <ctime>

namespace vfs {

class VfsDateTime
{
public:
  VfsDateTime();
  VfsDateTime(int day, int month, int year);
  VfsDateTime(int day, int month, int year, int second, int minute, int hour);

  void setFromTm(const tm* t);
  void setFromTime(const time_t* t);
  void setDateTime(int day, int month, int year,
                   int second, int minute, int hour);

  VfsDateTime* setDay(int day);
  VfsDateTime* setMonth(int month);
  VfsDateTime* setYear(int year);
  VfsDateTime* setSecond(int second);
  VfsDateTime* setMinute(int minute);
  VfsDateTime* setHour(int hour);

  int getDay() const;
  int getMonth() const;
  int getYear() const;
  int getSecond() const;
  int getMinute() const;
  int getHour() const;

private:
  void init(int day = 0, int month = 0, int year = 0, 
            int second = 0, int minute = 0, int hour = 0);

  int day;
  int month;
  int year;
  int second;
  int minute;
  int hour;
};

} /* namespace vfs */

#endif