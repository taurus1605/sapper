#ifndef SAPPER_CLOCK_SHAPE_H
#define SAPPER_CLOCK_SHAPE_H


#include "lib.h"
#include "constants.h"

#include <string>
#include <ctime>


class Clock_shape : public Graph_lib::Out_box
{
public:
  Clock_shape (Graph_lib::Point p, int w, int h);

  void update ();
  void start ();
  void reset ();
  void clear ();

  void timeout ();

  bool clock_launched{ false };

private:
  Graph_lib::Window* pwin;
  int start_time; // in secs from 00:00 last day
  const double dt{ 0.5 };

  static void cb_timeout (void* pwin);
};


int get_local_time ();

std::string time_style (int mm, int ss);


#endif // SAPPER_CLOCK_SHAPE_H
