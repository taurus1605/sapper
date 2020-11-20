#ifndef SAPPER_CLOCK_SHAPE_H
#define SAPPER_CLOCK_SHAPE_H 1


#include <string>
#include <ctime>
#include "../_lib/Graph_lib/Graph.h"
#include "../_lib/Graph_lib/Window.h"
#include "../_lib/Graph_lib/GUI.h"


class Out_box_resizable : public Graph_lib::Out_box
{
public:
  Out_box_resizable (Graph_lib::Point p, int w, int h);

  void resize (int w, int h);
};


class Clock_shape : public Out_box_resizable
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
