#include <util/Clock.h>

#include <chrono>
#include <iomanip>
#include <sstream>


using namespace std::chrono;

Clock::Clock()
  : time_()
{
  update();
}


Clock::~Clock()
{
}

void Clock::update()
{
  const std::time_t tnow = now();
  time_ = *std::localtime(&tnow);
}

std::time_t Clock::now()
{
  const system_clock::time_point now{system_clock::now()};
  const std::time_t tnow = system_clock::to_time_t(now);
  return tnow;
}


std::string Clock::getTimeStamp()
{
  std::stringstream ss;
  ss << std::setfill('0')
     << getYear() << '-'
     << std::setw(2) << getMonth()   << '-'
     << std::setw(2) << getDay()     << ' '
     << std::setw(2) << getHours()   << ':'
     << std::setw(2) << getMinutes() << ':'
     << std::setw(2) << getSeconds();
  return ss.str();
}


int Clock::getYear() const
{
  return static_cast<int>(time_.tm_year) + 1900;
}

int Clock::getMonth() const
{
  return static_cast<int>(time_.tm_mon);
}

int Clock::getDay() const
{
  return static_cast<int>(time_.tm_mday);
}

int Clock::getHours() const
{
  return static_cast<int>(time_.tm_hour);
}

int Clock::getMinutes() const
{
  return static_cast<int>(time_.tm_min);
}

int Clock::getSeconds() const
{
  return static_cast<int>(time_.tm_sec);
}
