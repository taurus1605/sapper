#ifndef MINER_H
#define MINER_H 1


#include "constants.h"
#include "cell.h"
#include "clock_shape.h"
#include "vector_ref_cl.h"

#include "../_lib/Graph_lib/GUI.h"
#include "../_lib/Graph_lib/Graph.h"

#include <string>
#include <tuple>
#include <vector>


enum class Image_type
{
  first_flag_sign=1,
  second_flag_sign,
  defeat,
  win
};


class Sapper : public Graph_lib::Window
{
public:
  Sapper(Graph_lib::Point p);

  void set_img(std::string, Image_type);

private:
  unsigned int num_of_bombs{0};
  unsigned int flags_counter{0};
  int num_of_rows{size_easy};
  bool first_click{true};

  Vector_ref_cl<Vector_ref_cl<Cell> > cells;
  Button_resizable btn_quit;
  Button_resizable btn_new_game;
  Button_resizable btn_size_easy;
  Button_resizable btn_size_normal;
  Button_resizable btn_size_hard;
  Graph_lib::Image* first_flag_counter{nullptr};
  Graph_lib::Image* second_flag_counter{nullptr};
  Clock_shape game_time;

  void end_game (int, int);
  void set_bombs (int n, int ii, int jj);
  void new_game (int n);
  void quit ();
  void clicked (void* widget);
  void new_game_menu ();
  void new_game_show ();
  void update_cells_around (int, int);
  void open_area (int i, int j);
  void resize_window (int w, int h, int n);

  static void cb_clicked(void* widget, void* win);
  static void cb_new_game(void* widget, void* win);
  static void cb_quit(void* widget, void* win);
  static void cb_size_easy(void* widget, void* win);
  static void cb_size_normal(void* widget, void* win);
  static void cb_size_hard(void* widget, void* win);
};


#endif // MINER_H
