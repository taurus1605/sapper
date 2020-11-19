#include "constants.h"
#include "game.h"
#include "cell.h"
#include "clock_shape.h"

#include <ctime>
#include <iostream>
#include <vector>
#include <tuple>


Sapper::Sapper (Graph_lib::Point p)
  : Graph_lib::Window{ p,  cell_size*size_1lvl + 2*margin_left + side_bar, cell_size * size_1lvl + 2*margin_top, "SAPPER"}
    , num_of_rows{ size_1lvl }
    , flags_counter{ size_1lvl }
    , btn_quit{ Graph_lib::Point(2*margin_left + cell_size * num_of_rows + (side_bar - button_w)/2,
                                                               2*margin_top + cell_size * num_of_rows - 2*button_h), button_w, button_h, "QUIT", cb_quit}
    , btn_new_game{ Graph_lib::Point(2*margin_left + cell_size * num_of_rows + (side_bar - button_w)/2,
                                                                   margin_top +button_w + button_h), button_w, button_h, "NEW GAME", cb_new_game}
//    , curr_out{Graph_lib::Point(2*margin_left + cell_size * num_of_rows + (side_bar - button_w)/2,
//                                                               margin_top), button_w, button_w, ""}
    , btn_size_s1{ Graph_lib::Point(btn_new_game.loc.x, btn_new_game.loc.y), button_w/3, button_h, "10x10", cb_size_s1}
    , btn_size_s2{ Graph_lib::Point(btn_new_game.loc.x + button_w/3, btn_new_game.loc.y), button_w/3, button_h, "15x15", cb_size_s2}
    , btn_size_s3{ Graph_lib::Point(btn_new_game.loc.x + 2* button_w/3, btn_new_game.loc.y), button_w/3, button_h, "20x20", cb_size_s3}
    , game_time{ Graph_lib::Point(x_max()-side_bar+margin_left, margin_top), side_bar-2*margin_left, 30 }
{
  attach(btn_quit);
  attach(btn_new_game);
//  attach(curr_out);
  attach(btn_size_s1);
  attach(btn_size_s2);
  attach(btn_size_s3);
  attach(game_time);
  new_game_show();
  new_game(size_1lvl);
}

void Sapper::set_bombs(int num_of_bombs, int ii, int jj)                  //only for vector with "clear" elements!
{
  if (num_of_bombs < 0)
  {
    return;
    std::cerr<<"TOO FEW BOMBS";
  }
  flags_counter = num_of_bombs;
  int n = cells.size();
  std::srand(time(0));
  int counter = 0;
  while (counter < num_of_bombs)
  {
    int i = rand()%n;
    int j = rand()%n;
    if (i == ii and j == jj) continue;
    if (!cells[i][j].is_bombed())
    {
      cells[i][j].set_bomb(true);
      update_cells_around(i,j);
      ++counter;
    }
  }
}

void Sapper::cb_new_game (void* widget, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game_menu();
}

void Sapper::cb_quit (void* widget, void* win)
{
  Graph_lib::reference_to<Sapper>(win).quit();
}

void Sapper::cb_clicked (void* widget, void* win)
{
  Graph_lib::reference_to<Sapper>(win).clicked(widget);
}

void Sapper::cb_size_s1 (void* widget, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game(size_1lvl);
}

void Sapper::cb_size_s2 (void* widget, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game(size_2lvl);
}

void Sapper::cb_size_s3 (void* widget, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game(size_3lvl);
}

void Sapper::new_game_menu ()
{
  if (game_time.clock_launched)
    game_time.reset();
  btn_new_game.hide();
  btn_size_s1.show();
  btn_size_s2.show();
  btn_size_s3.show();
}

void Sapper::new_game_show ()
{
  btn_new_game.show();
  btn_size_s1.hide();
  btn_size_s2.hide();
  btn_size_s3.hide();
}


void Sapper::new_game (int n)
{
//  if (!first_click)
//    game_time.reset();
  game_time.clear();
  first_click = true;
  new_game_show();
  size_range(0,0, 20000,20000);
  int hh = cell_size * n + 2 * margin_top;
  int ww = cell_size * n + 4 * margin_left + side_bar;
  resize(ww,hh);
  size_range(ww,hh,ww,hh);

  for(int i = 0; i < cells.size(); ++i)
  {
    for(int j = 0; j < cells[i].size(); ++j)
      detach(cells[i][j]);
  }
  cells.clear();
  for(int i = 0; i < n; ++i)
  {
    cells.push_back(new Vector_ref_cl<Cell>);
    for(int j = 0; j < n; ++j)
    {
      cells[i].push_back(new Cell{ Graph_lib::Point(margin_left + i*cell_size, margin_top + j*cell_size), cell_size, cb_clicked });
      attach(cells[i][j]);
    }
  }
  num_of_bombs = (n-2) * 2;
  if (n == size_1lvl) num_of_bombs = 16;
  if (n == size_2lvl) num_of_bombs = 30;
  if (n == size_3lvl) num_of_bombs = 50;
//  curr_out.put(num_of_bombs);
}

void Sapper::quit ()
{
  hide();
}

void Sapper::clicked (void *widget)
{
  Fl_Widget& w = Graph_lib::reference_to<Fl_Widget>(widget);
  int x = w.x() - margin_left;
  int y = w.y() - margin_top;
  int i = x / cell_size;
  int j = y / cell_size;


  if (first_click)
  {
    first_click = false;
    set_bombs(num_of_bombs, i, j);
    game_time.start();
//    curr_out.put(num_of_bombs);
  }


  Cell& c = cells[i][j];
  if (c.is_opened()) return;
  if (Fl::event_button() == 3)
  {
    if (flags_counter == 0) return;
    c.set_flag(!c.is_flaged());
    if (!c.is_flaged())
    {
      ++flags_counter;
      c.get_pw()->color(fl_rgb_color(195,195,195));
    }
    else
    {
      --flags_counter;
      c.get_pw()->color(Graph_lib::Color::white);
    }
//    curr_out.put(flags_counter);
  }
  if (Fl::event_button() == 1)
  {
    if (c.is_flaged()) return;
    if (c.is_bombed()) end_game();
    else
    {
      open_area(i,j);
      Fl:redraw();
    }
  }


  if (flags_counter == 0)
  {
    int closed_counter = 0;
    for(int i = 0; i < cells.size(); ++i)
    {
      for(int j = 0; j < cells.size(); ++j)
      {
        if(cells[i][j].is_opened() == false) ++closed_counter;
      }
    }
    if (closed_counter == num_of_bombs)
    {
      end_game();
//      curr_out.put("YOU WIN!!!");
    }
  }
}

void Sapper::end_game ()
{
  for(int i = 0; i < cells.size(); ++i)
  {
    for(int j = 0; j < cells.size(); ++j)
    {
      cells[i][j].set_open();
      if(cells[i][j].is_bombed())
      {
        cells[i][j].show_bomb();
      }
      if(cells[i][j].is_flaged() && !cells[i][j].is_bombed())
      {
        cells[i][j].get_pw()->color(Graph_lib::Color::red);
      }
      if(cells[i][j].is_flaged() && cells[i][j].is_bombed())
      {
        cells[i][j].get_pw()->color(Graph_lib::Color::dark_green);
      }
    }
  }
  game_time.reset();
  Fl::redraw();
}

bool is_consist (std::vector<std::tuple<int, int>> v, std::tuple<int, int> el)
{
  for (size_t i{0}; i < v.size(); ++i)
  {
    if (std::get<0>(v[i]) == std::get<0>(el) && std::get<1>(v[i]) == std::get<1>(el))
      return true;
  }
  return false;
}

void Sapper::open_area (int i, int j)
{
  std::vector<std::tuple<int, int>> queue;
  std::vector<std::tuple<int, int>> visited;
  queue.push_back(std::make_tuple(i, j));
  int field_size = cells[0].size();

  while (queue.size())
  {
    std::tuple<int, int> cur = queue[queue.size()-1];
    queue.pop_back();

    if (!is_consist(queue, cur))
      visited.push_back(cur);

    if (cells[std::get<0>(cur)][std::get<1>(cur)].is_bombed() ||
        cells[std::get<0>(cur)][std::get<1>(cur)].is_opened() ||
        cells[std::get<0>(cur)][std::get<1>(cur)].is_flaged())
      continue;

    cells[std::get<0>(cur)][std::get<1>(cur)].set_open();
    cells[std::get<0>(cur)][std::get<1>(cur)].get_pw()->color(Graph_lib::Color::yellow);

    if (cells[std::get<0>(cur)][std::get<1>(cur)].bombs_around > 0)
    {
      cells[std::get<0>(cur)][std::get<1>(cur)].label = char(cells[std::get<0>(cur)][std::get<1>(cur)].bombs_around + int('0'));
      continue;
    }

    if (std::get<0>(cur) > 0)
    {
      if (!is_consist(visited, std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur))))
        queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur)));
      if (std::get<1>(cur) > 0)
        if (!is_consist(visited, std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur) - 1)))
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur) - 1));
      if (std::get<1>(cur) < field_size - 1)
        if (!is_consist(visited, std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur) + 1)))
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur) + 1));
    }
    if (std::get<0>(cur) < field_size - 1)
    {
      if (!is_consist(visited, std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur))))
        queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur)));
      if (std::get<1>(cur) > 0)
        if (!is_consist(visited, std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur) - 1)))
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur) - 1));
      if (std::get<1>(cur) < field_size - 1)
        if (!is_consist(visited, std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur) + 1)))
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur) + 1));
    }
    if (std::get<1>(cur) > 0)
      if (!is_consist(visited, std::make_tuple(std::get<0>(cur), std::get<1>(cur) - 1)))
        queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur), std::get<1>(cur) - 1));
    if (std::get<1>(cur) < field_size - 1)
      if (!is_consist(visited, std::make_tuple(std::get<0>(cur), std::get<1>(cur) + 1)))
        queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur), std::get<1>(cur) + 1));
  }
}

void Sapper::update_cells_around (int i, int j)
{
  if (i > 0)
  {
    if (!cells[i-1][j].is_bombed())
      cells[i-1][j].bombs_around++;
    if (j > 0)
      if (!cells[i-1][j-1].is_bombed())
        cells[i-1][j-1].bombs_around++;
    if (j < cells[i].size() - 1)
      if (!cells[i-1][j+1].is_bombed())
        cells[i-1][j+1].bombs_around++;
  }
  if (i < cells[i].size() - 1)
  {
    if (!cells[i+1][j].is_bombed())
      cells[i+1][j].bombs_around++;
    if (j > 0)
      if (!cells[i+1][j-1].is_bombed())
        cells[i+1][j-1].bombs_around++;
    if (j < cells[i].size() - 1)
      if (!cells[i+1][j+1].is_bombed())
        cells[i+1][j+1].bombs_around++;
  }
  if (j > 0)
    if (!cells[i][j-1].is_bombed())
      cells[i][j-1].bombs_around++;
  if (j < cells[i].size() - 1)
    if (!cells[i][j+1].is_bombed())
      cells[i][j+1].bombs_around++;
}
