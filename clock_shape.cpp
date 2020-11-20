#include "clock_shape.h"

#include <ctime>
#include <sstream>
#include <string>
#include <iostream>


Out_box_resizable::Out_box_resizable (Graph_lib::Point p, int w, int h)
  : Graph_lib::Out_box{ p, w, h, "" }
{ }

Clock_shape::Clock_shape (Graph_lib::Point p, int w, int h)
  : Out_box_resizable{ p, w, h }
{ }


int get_local_time ()
{
  time_t calendar{ std::time(nullptr) };
  tm* local{ std::localtime(&calendar) };

  if (local)
    return local->tm_hour*3600 + local->tm_min*60 + local->tm_sec;

  return -1;
}

std::string time_style (int mm, int ss)
{
  std::stringstream sstr;
  sstr << "                   "; // indentation brfore time

  if (mm / 10 == 0)
    sstr << "0" << mm << ":";
  else
    sstr << mm << ":";

  if (ss / 10 == 0)
    sstr << "0" << ss;
  else if (ss / 100 == 0)
    sstr << ss;
  else
    sstr << ss % 100;

  return sstr.str();
}


void Out_box_resizable::resize (int w, int h)
{
  pw->size(w, h);
}


void Clock_shape::cb_timeout (void* pwin)
{
  Graph_lib::reference_to<Clock_shape>(pwin).timeout();
}

void Clock_shape::update ()
{
  int current{ get_local_time() - start_time };
  int mm{ current / 60 };
  int ss{ current % 60 };

  put(time_style(mm, ss));
}

void Clock_shape::start ()
{
  time_t calendar{ std::time(nullptr) };
  tm* cur_time = std::localtime(&calendar);

  start_time = cur_time->tm_hour*3600 + cur_time->tm_min*60 + cur_time->tm_sec;

  clock_launched = true;

  Fl::repeat_timeout(dt, cb_timeout, this);
}

void Clock_shape::reset ()
{
  clock_launched = false;
  Fl::remove_timeout(cb_timeout, this);
}

void Clock_shape::clear ()
{
  put("");
}

void Clock_shape::timeout ()
{
  update();
  Fl::add_timeout(dt, cb_timeout, this);
}
