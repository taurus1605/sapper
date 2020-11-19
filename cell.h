#ifndef CELL_H
#define CELL_H


#include "lib.h"
#include <string>


class Button_with_pw : public Graph_lib::Button
{
public:
  Button_with_pw (Graph_lib::Point p, int size_, Graph_lib::Callback cb);
  Fl_Widget* get_pw () const { return pw; }
};


class Cell : public Button_with_pw
{
public:
  Cell (Graph_lib::Point p, int size_, Graph_lib::Callback cb);
  void set_bomb (bool bomb_)   { bombed = bomb_; }
  void set_flag (bool flag_)   { flaged = flag_; }
  void set_open ()             { opened = true;  }
  void show_bomb ();

  bool is_bombed() const { return bombed; }
  bool is_flaged() const { return flaged; }
  bool is_opened() const { return opened; }
  int bombs_around {0};

private:
  bool bombed{ false };
  bool flaged{ false };
  bool opened{ false };
};


#endif // CELL_H
