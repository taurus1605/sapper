#ifndef SAPPER_GAME_H
#define SAPPER_GAME_H


#include "constants.h"
#include "cell.h"
#include "clock_shape.h"
#include "vector_ref_cl.h"

#include <vector>
#include <tuple>


class Sapper : public Graph_lib::Window
{
public:
  Sapper (Graph_lib::Point p);

private:
  bool first_click{ true };

  void set_bombs (int n, int ii, int jj);
  void new_game (int n);
  void quit ();
  void clicked (void* widget);
  void new_game_menu ();
  void new_game_show ();
  void update_cells_around (int, int);

  void end_game ();
  void open_area (int i, int j);

  static void cb_clicked     (void* widget, void* win);
  static void cb_new_game    (void* widget, void* win);
  static void cb_quit        (void* widget, void* win);
  static void cb_size_s1     (void* widget, void* win);
  static void cb_size_s2     (void* widget, void* win);
  static void cb_size_s3     (void* widget, void* win);

  unsigned int num_of_bombs{0};
  unsigned int flags_counter{0};
  int num_of_rows{size_1lvl};

  Vector_ref_cl<Vector_ref_cl<Cell> > cells;
  Graph_lib::Button btn_quit;
  Graph_lib::Button btn_new_game;
  //Graph_lib::Out_box curr_out;
  Graph_lib::Button btn_size_s1;
  Graph_lib::Button btn_size_s2;
  Graph_lib::Button btn_size_s3;
  Clock_shape game_time;
};


bool is_consist (std::vector<std::tuple<int,int>> v, std::tuple<int,int> el);


#endif // MINER_H
