#include "miner.h"
#include "cell.h"

#include <ctime>
#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>


std::string str (int i)
{
  std::stringstream sstr;
  sstr << i;
  return sstr.str();
}


Sapper::Sapper(Graph_lib::Point p)
  : Graph_lib::Window{ p,  window_w, window_h, "SAPPER" }
    , flags_counter{ size_easy_bombs }
    , num_of_rows{ size_easy }
    , btn_quit{ Graph_lib::Point(margin_lr + cell_size * num_of_rows + (sidebar_w - button_w)/2, y_max() - 2*button_h),
             button_w, button_h, "QUIT", cb_quit }
    , btn_new_game{ Graph_lib::Point(margin_lr + cell_size * num_of_rows + (sidebar_w - button_w)/2, margin_tb + button_w + button_h),
                 button_w, button_h, "NEW GAME", cb_new_game }
    , btn_size_easy{ Graph_lib::Point(btn_new_game.loc.x, btn_new_game.loc.y), button_w/3, button_h, (str(size_easy)+"x"+str(size_easy)), cb_size_easy }
    , btn_size_normal{ Graph_lib::Point(btn_new_game.loc.x + button_w/3, btn_new_game.loc.y), button_w/3, button_h, (str(size_normal)+"x"+str(size_normal)), cb_size_normal }
    , btn_size_hard{ Graph_lib::Point(btn_new_game.loc.x + 2* button_w/3, btn_new_game.loc.y), button_w/3, button_h, (str(size_hard)+"x"+str(size_hard)), cb_size_hard }
    , game_time{ Graph_lib::Point(x_max()-(sidebar_w - button_w)/2 - button_w, margin_tb), button_w, game_timer_h }
{
    attach(btn_quit);
    attach(btn_new_game);
    attach(btn_size_easy);
    attach(btn_size_normal);
    attach(btn_size_hard);
    attach(game_time);
    new_game_show();
    new_game(size_easy);
}


void Sapper::cb_new_game (void*, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game_menu();
}

void Sapper::cb_quit (void*, void* win)
{
  Graph_lib::reference_to<Sapper>(win).quit();
}

void Sapper::cb_clicked (void* widget, void* win)
{
  Graph_lib::reference_to<Sapper>(win).clicked(widget);
}

void Sapper::cb_size_easy (void*, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game(size_easy);
}

void Sapper::cb_size_normal (void*, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game(size_normal);
}

void Sapper::cb_size_hard (void*, void* win)
{
  Graph_lib::reference_to<Sapper>(win).new_game(size_hard);
}


void Sapper::new_game_menu ()
{
  if (game_time.clock_launched)
    game_time.reset();
  btn_new_game.hide();
  btn_size_easy.show();
  btn_size_normal.show();
  btn_size_hard.show();
}

void Sapper::new_game_show ()
{
  btn_new_game.show();
  btn_size_easy.hide();
  btn_size_normal.hide();
  btn_size_hard.hide();
}

void Sapper::set_bombs (int num_of_bombs, int ii, int jj)
{                 //only for vector with "clear" elements!
  if (num_of_bombs < 0) throw std::runtime_error{ "too few bombs" };

  int n{ cells.size() };
  int counter{ 0 };
  flags_counter = num_of_bombs;

  std::srand(time(0));

  while (counter < num_of_bombs)
  {
    int i{ rand() % n };
    int j{ rand() % n };
    if (i == ii && j == jj) continue;
    if (!cells[i][j].is_bombed())
    {
      cells[i][j].set_bomb(true);
      update_cells_around(i, j);
      ++counter;
    }
  }
}

void Sapper::resize_window (int w, int h, int n)
{
  resize(w, h);
  size_range(w, h, w, h);

  btn_new_game.resize(button_w, button_h);
  btn_new_game.move(-btn_new_game.loc.x + (margin_lr + cell_size*n + (sidebar_w - button_w) / 2),
                    -btn_new_game.loc.y + (margin_tb + button_w));
  btn_quit.resize(button_w, button_h);
  btn_quit.move(-btn_quit.loc.x + (margin_lr + cell_size * n + (sidebar_w - button_w) / 2),
                -btn_quit.loc.y + (y_max() - margin_tb - button_h));
  btn_size_easy.resize(button_w/3, button_h);
  btn_size_normal.resize(button_w/3, button_h);
  btn_size_hard.resize(button_w/3, button_h);
  btn_size_easy.move(-btn_size_easy.loc.x + btn_new_game.loc.x,
                     -btn_size_easy.loc.y + btn_new_game.loc.y);
  btn_size_normal.move(-btn_size_normal.loc.x + btn_new_game.loc.x + button_w / 3,
                       -btn_size_normal.loc.y + btn_new_game.loc.y);
  btn_size_hard.move(-btn_size_hard.loc.x + btn_new_game.loc.x + button_w / 3 * 2,
                     -btn_size_hard.loc.y + btn_new_game.loc.y);
  game_time.resize(button_w, button_h);
  game_time.move(-game_time.loc.x + x_max()-(sidebar_w - button_w) / 2 - button_w,
                 -game_time.loc.y + margin_tb);
}

void Sapper::new_game(int n)
{
  game_time.clear();
  first_click = true;
  size_range(0,0, 4000,4000);

  int hh{ cell_size*n + 2*margin_tb};
  int ww{ cell_size*n + margin_lr + sidebar_w};
  resize_window(ww, hh, n);
  new_game_show();

  for (int i{ 0 }; i < cells.size(); ++i)
  {
    for (int j{ 0 }; j < cells[i].size(); ++j)
    {
      detach(cells[i][j]);
      (*cells[i][j].img_ptr).~Image();
    }
  }

  cells.clear();
  for (int i = 0; i < n; ++i)
  {
    cells.push_back(new Vector_ref_cl<Cell>);
    for (int j = 0; j < n; ++j)
    {
      cells[i].push_back(new Cell{ Graph_lib::Point(margin_lr + i*cell_size, margin_tb + j*cell_size), cell_size, cb_clicked });
      attach(cells[i][j]);
    }
  }

  if (n == size_easy)   num_of_bombs = size_easy_bombs;
  if (n == size_normal) num_of_bombs = size_normal_bombs;
  if (n == size_hard)   num_of_bombs = size_hard_bombs;

  std::string n1{ img_lib + str(num_of_bombs/10) + ".png" };
  std::string n2{ img_lib + str(num_of_bombs%10) + ".png" };
  set_img(n1, Image_type::first_flag_sign);
  set_img(n2, Image_type::second_flag_sign);
}

void Sapper::quit ()
{
  hide();
}

void Sapper::clicked (void *widget)
{
  short click = Fl::event_button();
  Fl_Widget& w{ Graph_lib::reference_to<Fl_Widget>(widget) };
  int x{ w.x() - margin_lr };
  int y{ w.y() - margin_tb };
  int i{ x / cell_size };
  int j{ y / cell_size };

  Cell& c{ cells[i][j] };

  if (first_click)
  {
    if(click != 1) return;
      first_click = false;
    game_time.start();
    set_bombs(num_of_bombs, i, j);
    std::string n1{ img_lib + str(num_of_bombs/10) + ".png" };
    std::string n2{ img_lib + str(num_of_bombs%10) + ".png" };
    set_img(n1, Image_type::first_flag_sign);
    set_img(n2, Image_type::second_flag_sign);
  }

  if (c.is_opened()) return; // do nothing when click on opened btn
  if (click == 3) // if clicked right mouse button
  {
    if (flags_counter == 0 && !c.is_flaged()) return;
    c.set_flag(!c.is_flaged());
    if (!c.is_flaged())
    {
      ++flags_counter;
      c.set_img(img_btn_pulled);
      attach(*c.img_ptr);
    }
    else
    {
      --flags_counter;
      c.set_img(img_flag);
      attach(*c.img_ptr);
    }
    std::string n1{ img_lib + str(flags_counter/10) + ".png"};
    std::string n2{ img_lib + str(flags_counter%10) + ".png"};
    set_img(n1, Image_type::first_flag_sign);
    set_img(n2, Image_type::second_flag_sign);
  }
  if (click == 1) // if clicked left mouse button
  {
    if (c.is_flaged()) return;
    if (c.is_bombed())
    {
      end_game(i,j);
      set_img(img_defeat, Image_type::defeat);
    }
    else
    {
      open_area(i,j);
      redraw();
    }
  }

  if (flags_counter == 0)
  {
    int closed_counter{ 0 };
    for (int i{ 0 }; i < cells.size(); ++i)
    {
      for (int j{ 0 }; j < cells.size(); ++j)
      {
        if (!cells[i][j].is_opened()) ++closed_counter;
      }
    }
    if (closed_counter == num_of_bombs)
    {
      end_game(-1,-1);
      set_img(img_win, Image_type::win);
    }
  }
  redraw();
}

void Sapper::end_game (int i_, int j_)
{
  for (int i{ 0 }; i < cells.size(); ++i)
  {
    for(int j{0}; j < cells.size(); ++j)
    {
      cells[i][j].set_open();
      if (i == i_ and j == j_)
      {
        cells[i][j].set_img(img_bomb_explosed);
        attach(*cells[i][j].img_ptr);
        continue;
      }
      if (cells[i][j].is_bombed())
      {
        cells[i][j].set_img(img_bomb);
        attach(*cells[i][j].img_ptr);
      }
      if (cells[i][j].is_flaged() && !cells[i][j].is_bombed())
      {
        cells[i][j].set_img(img_error_bomb);
        attach(*cells[i][j].img_ptr);
      }
      if (cells[i][j].is_flaged() and cells[i][j].is_bombed())
      {
        cells[i][j].set_img(img_bomb_defused);
        attach(*cells[i][j].img_ptr);
      }
    }
  }
  game_time.reset();
  redraw();
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

void Sapper::set_img (std::string str, Image_type imgt)
{
  if (imgt == Image_type::first_flag_sign)
  {
    if (first_flag_counter != nullptr) (*first_flag_counter).~Image();
    first_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_lr + cell_size * cells.size() + sidebar_w/2 - cell_size,
                                                              margin_tb + button_w/3), str);
    attach(*first_flag_counter);
    return;
  }
  if (imgt == Image_type::second_flag_sign)
  {
    if (second_flag_counter != nullptr) (*second_flag_counter).~Image();
    second_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_lr + cell_size * cells.size() + sidebar_w/2,
                                                                margin_tb + button_w/3), str);
    attach(*second_flag_counter);

    return;
  }
  if (imgt == Image_type::defeat)
  {  // defeat!
    if(first_flag_counter != nullptr) detach(*first_flag_counter);
    if(second_flag_counter != nullptr) detach(*second_flag_counter);
    first_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_lr + cell_size * cells.size() + sidebar_w/2 - 40,
                                                               margin_tb + button_w/3), str);
    attach(*first_flag_counter);
    return;
  }
  if (imgt == Image_type::win)
  {  // win!
    if(first_flag_counter != nullptr) detach(*first_flag_counter);
    if(second_flag_counter != nullptr) detach(*second_flag_counter);
    first_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_lr + cell_size * cells.size() + (sidebar_w - 150)/2,
                                                               margin_tb + button_w/3 + 10), str);
    attach(*first_flag_counter);
    return;
  }
}

void Sapper::open_area (int i, int j)
{
  std::vector<std::tuple<int, int> > queue;
  queue.push_back(std::make_tuple(i, j));
  int field_size = cells.size();

  while (queue.size())
  {
    std::tuple<int, int> cur{ queue[queue.size()-1] };
    queue.pop_back();

    if (cells[std::get<0>(cur)][std::get<1>(cur)].is_bombed() ||
      cells[std::get<0>(cur)][std::get<1>(cur)].is_opened() ||
      cells[std::get<0>(cur)][std::get<1>(cur)].is_flaged())
      continue;

    cells[std::get<0>(cur)][std::get<1>(cur)].set_open();

    if (cells[std::get<0>(cur)][std::get<1>(cur)].bombs_around > 0)
    {
      switch (cells[std::get<0>(cur)][std::get<1>(cur)].bombs_around)
      {
        case 1:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"1.png");
          break;
        case 2:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"2.png");
          break;
        case 3:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"3.png");
          break;
        case 4:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"4.png");
          break;
        case 5:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"5.png");
          break;
        case 6:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"6.png");
          break;
        case 7:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"7.png");
          break;
        case 8:
          cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_lib+"8.png");
          break;
        default:
          throw std::runtime_error{"image didn't pushed"};
        }
        attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
        redraw();
        continue;
      }
      else
      {
        cells[std::get<0>(cur)][std::get<1>(cur)].set_img(img_btn_pushed);
        attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
      }

      if (std::get<0>(cur) > 0)
      {
        if (!cells[std::get<0>(cur) - 1][std::get<1>(cur)].is_opened())
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur)));
        if (std::get<1>(cur) > 0)
          if (!cells[std::get<0>(cur) - 1][std::get<1>(cur) - 1].is_opened())
            queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur) - 1));
        if (std::get<1>(cur) < field_size - 1)
          if (!cells[std::get<0>(cur) - 1][std::get<1>(cur) + 1].is_opened())
            queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) - 1, std::get<1>(cur) + 1));
      }
      if (std::get<0>(cur) < field_size - 1)
      {
        if (!cells[std::get<0>(cur) + 1][std::get<1>(cur)].is_opened())
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur)));
        if (std::get<1>(cur) > 0)
          if (!cells[std::get<0>(cur) + 1][std::get<1>(cur) - 1].is_opened())
            queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur) - 1));
        if (std::get<1>(cur) < field_size - 1)
          if (!cells[std::get<0>(cur) + 1][std::get<1>(cur) + 1].is_opened())
            queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur) + 1, std::get<1>(cur) + 1));
      }
      if (std::get<1>(cur) > 0)
        if (!cells[std::get<0>(cur)][std::get<1>(cur) - 1].is_opened())
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur), std::get<1>(cur) - 1));
      if (std::get<1>(cur) < field_size - 1)
        if (!cells[std::get<0>(cur)][std::get<1>(cur) + 1].is_opened())
          queue.insert(queue.begin(), std::make_tuple(std::get<0>(cur), std::get<1>(cur) + 1));
  }
}
