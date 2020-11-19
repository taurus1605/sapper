#include "miner.h"
#include "cell.h"

#include <ctime>
#include <iostream>
#include <vector>
#include <tuple>


const int n0 = 10;

std::string str(int i){
  char t = i + '0';
  return std::string(1,  t);
}

Sapper::Sapper(Graph_lib::Point p):
    Graph_lib::Window(p,  cell_size * n0 + margin_left + side_bar, cell_size * n0 + 2*margin_top, "SAPPER"),
    num_of_rows{n0},
    flags_counter{n0},
    btn_quit{Graph_lib::Point(margin_left + cell_size * num_of_rows + (side_bar - button_w)/2,
                                  y_max() - 2*button_h), button_w, button_h, "QUIT", cb_quit},
    btn_new_game{Graph_lib::Point(margin_left + cell_size * num_of_rows + (side_bar - button_w)/2,
                                  margin_top + button_w + button_h), button_w, button_h, "NEW GAME", cb_new_game},
    btn_size_10{Graph_lib::Point(btn_new_game.loc.x, btn_new_game.loc.y), button_w/3, button_h, "10x10", cb_size_10},
    btn_size_15{Graph_lib::Point(btn_new_game.loc.x + button_w/3, btn_new_game.loc.y), button_w/3, button_h, "15x15", cb_size_15},
    btn_size_20{Graph_lib::Point(btn_new_game.loc.x + 2* button_w/3, btn_new_game.loc.y), button_w/3, button_h, "20x20", cb_size_20},
    game_time{ Graph_lib::Point(x_max()-(side_bar - button_w) / 2 - button_w, margin_top), button_w, 30 }
{
    attach(btn_quit);
    attach(btn_new_game);
    attach(btn_size_10);
    attach(btn_size_15);
    attach(btn_size_20);
    attach(game_time);
    new_game_show();
    new_game(n0);
}

void Sapper::set_bombs(int num_of_bombs, int ii, int jj){                 //only for vector with "clear" elements!
    if (num_of_bombs < 0) {
        return;
        std::cerr<<"TOO FEW BOMBS";
    }
    flags_counter = num_of_bombs;
    int n = cells.size();
    std::srand(time(0));
    int counter = 0;
    while(counter < num_of_bombs){
        int i = rand()%n;
        int j = rand()%n;
        if(i == ii and j == jj) continue;
        if(cells[i][j].is_bombed() == false){
            cells[i][j].set_bomb(true);
            update_cells_around(i,j);
            ++counter;
        }
    }
}


void Sapper::cb_new_game(void* widget, void* win){
    Graph_lib::reference_to<Sapper>(win).new_game_menu();
}
void Sapper::cb_quit(void* widget, void* win){
    Graph_lib::reference_to<Sapper>(win).quit();
}
void Sapper::cb_clicked(void* widget, void* win){
    Graph_lib::reference_to<Sapper>(win).clicked(widget);
}
void Sapper::cb_size_10(void* widget, void* win){
    Graph_lib::reference_to<Sapper>(win).new_game(10);
}
void Sapper::cb_size_15(void* widget, void* win){
    Graph_lib::reference_to<Sapper>(win).new_game(15);
}
void Sapper::cb_size_20(void* widget, void* win){
    Graph_lib::reference_to<Sapper>(win).new_game(20);
}
void Sapper::new_game_menu(){
    if (game_time.clock_launched)
        game_time.reset();
    btn_new_game.hide();
    btn_size_10.show();
    btn_size_15.show();
    btn_size_20.show();
}
void Sapper::new_game_show(){
    btn_new_game.show();
    btn_size_10.hide();
    btn_size_15.hide();
    btn_size_20.hide();
}


void Sapper::new_game(int n){
    game_time.clear();
    first_click = true;
    size_range(0,0, 4000,4000);

    int hh{ cell_size * n + 2*margin_top};
    int ww{ cell_size * n + margin_left + side_bar};

    resize(ww,hh);
    size_range(ww,hh,ww,hh);

    btn_new_game.get_pw()->size(button_w, button_h);
    btn_new_game.move(-btn_new_game.loc.x + (margin_left + cell_size * n + (side_bar - button_w) / 2),
                      -btn_new_game.loc.y + (margin_top + button_w));
    btn_quit.get_pw()->size(button_w, button_h);
    btn_quit.move(-btn_quit.loc.x + (margin_left + cell_size * n + (side_bar - button_w) / 2),
                      -btn_quit.loc.y + (y_max() - margin_top - button_h));
    btn_size_10.get_pw()->size(button_w/3, button_h);
    btn_size_15.get_pw()->size(button_w/3, button_h);
    btn_size_20.get_pw()->size(button_w/3, button_h);
    btn_size_10.move(-btn_size_10.loc.x + btn_new_game.loc.x,
                      -btn_size_10.loc.y + btn_new_game.loc.y);
    btn_size_15.move(-btn_size_15.loc.x + btn_new_game.loc.x + button_w / 3,
                      -btn_size_15.loc.y + btn_new_game.loc.y);
    btn_size_20.move(-btn_size_20.loc.x + btn_new_game.loc.x + button_w / 3 * 2,
                      -btn_size_20.loc.y + btn_new_game.loc.y);
    game_time.get_pw()->size(button_w, button_h);
    game_time.move(-game_time.loc.x + x_max()-(side_bar - button_w) / 2 - button_w,
                   -game_time.loc.y + margin_top);

    new_game_show();

    for(int i = 0; i < cells.size(); ++i){
        for(int j = 0; j < cells[i].size(); ++j){
            detach(cells[i][j]);
            (*cells[i][j].img_ptr).~Image();
        }
    }
    cells.clear();
    for(int i = 0; i < n; ++i){
        cells.push_back(new Vector_ref_cl<Cell>);
        for(int j = 0; j < n; ++j){
            cells[i].push_back(new Cell{Graph_lib::Point(margin_left + i*cell_size, margin_top + j*cell_size), cell_size, cb_clicked});
            attach(cells[i][j]);
        }
    }
    num_of_bombs = (n-2) * 2;
    if (n == 10 or n == 9) num_of_bombs = 15;
    if (n == 15) num_of_bombs = 30;
    if (n == 20) num_of_bombs = 50;
    std::string n1{ str(num_of_bombs/10) + ".png" };
    std::string n2{ str(num_of_bombs%10) + ".png" };
    set_img(n1, 1);
    set_img(n2, 2);
}

void Sapper::quit(){
    hide();
}


void Sapper::clicked(void *widget){
    short click = Fl::event_button();
    Fl_Widget& w{ Graph_lib::reference_to<Fl_Widget>(widget) };
    int x{ w.x() - margin_left };
    int y{ w.y() - margin_top };
    int i{ x / cell_size };
    int j{ y / cell_size };

    Cell& c{ cells[i][j] };

    if(first_click){
        if(click != 1) return;
        first_click = false;
        game_time.start();
        set_bombs(num_of_bombs, i, j);
        std::string n1{ str(num_of_bombs/10) + ".png" };
        std::string n2{ str(num_of_bombs%10) + ".png" };
        set_img(n1, 1);
        set_img(n2, 2);
    }

    if(c.is_opened()) return;
    if(click == 3){
        if(flags_counter == 0) return;
        c.set_flag(!c.is_flaged());
        if(!c.is_flaged()) {
            ++flags_counter;
            c.set_img("btn_pulled.png");
            attach(*c.img_ptr);
        }
        else{
            --flags_counter;
            c.set_img("flag.png");
            attach(*c.img_ptr);

        }
        std::string n1{ str(flags_counter/10) + ".png"};
        std::string n2{ str(flags_counter%10) + ".png"};
        set_img(n1, 1);
        set_img(n2, 2);
    }
    if (click == 1) {
        if(c.is_flaged()) return;
        if(c.is_bombed()) {
            end_game(i,j);
            set_img("defeat.png", 3);
        }
        else {
            open_area(i,j);
            Fl:redraw();
        }
    }


    if(flags_counter == 0){
        int closed_counter = 0;
        for(int i = 0; i < cells.size(); ++i){
            for(int j = 0; j < cells.size(); ++j){
                if(cells[i][j].is_opened() == false) ++closed_counter;
            }
        }
        if(closed_counter == num_of_bombs) {
            end_game(-1,-1);
            set_img("win.png", 4);
        }
    }
    Fl::redraw();
}

void Sapper::end_game(int i_, int j_){
    for(int i = 0; i < cells.size(); ++i){
        for(int j = 0; j < cells.size(); ++j){
            cells[i][j].set_open();
            if(i == i_ and j == j_) {
                cells[i][j].set_img("bomb_explosed.png");
                attach(*cells[i][j].img_ptr);
                continue;
            }
            if(cells[i][j].is_bombed()) {
                cells[i][j].set_img("bomb.png");
                attach(*cells[i][j].img_ptr);
            }
            if(cells[i][j].is_flaged() and cells[i][j].is_bombed()==false){
                cells[i][j].set_img("error_bomb.png");
                attach(*cells[i][j].img_ptr);

            }
            if(cells[i][j].is_flaged() and cells[i][j].is_bombed()){
                cells[i][j].set_img("bomb_defused.png");
                attach(*cells[i][j].img_ptr);
            }
        }
    }
    game_time.reset();
    Fl::redraw();
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
void Sapper::set_img(std::string str, int n){
    if(n==1){
        if(first_flag_counter != nullptr) (*first_flag_counter).~Image();
        first_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_left + cell_size * cells.size() + side_bar/2 - cell_size,
                                                                   margin_top + button_w/3), str);
        attach(*first_flag_counter);
        return;
    }
    if(n==2){
        if(second_flag_counter != nullptr) (*second_flag_counter).~Image();
        second_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_left + cell_size * cells.size() + side_bar/2,
                                                                    margin_top + button_w/3), str);
        attach(*second_flag_counter);

        return;
    }
    if(n == 3){  //defeat!
        if(first_flag_counter != nullptr) detach(*first_flag_counter);
        if(second_flag_counter != nullptr) detach(*second_flag_counter);
        first_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_left + cell_size * cells.size() + side_bar/2 - 40,
                                                                   margin_top + button_w/3), str);
        attach(*first_flag_counter);
        return;
    }
    if(n == 4){  //win!
        if(first_flag_counter != nullptr) detach(*first_flag_counter);
        if(second_flag_counter != nullptr) detach(*second_flag_counter);
        first_flag_counter = new Graph_lib::Image(Graph_lib::Point(margin_left + cell_size * cells.size() + (side_bar - 150)/2,
                                                                   margin_top + button_w/3 + 10), str);
        attach(*first_flag_counter);
        return;
    }
}

bool is_consist (std::vector<std::tuple<int, int> >& v, std::tuple<int, int> el)
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
    std::cerr << "Area opened ";
    std::vector<std::tuple<int, int> > queue;
    std::vector<std::tuple<int, int> > visited;
    queue.push_back(std::make_tuple(i, j));
    int field_size = cells[0].size();

    while (queue.size())
    {
        std::tuple<int, int> cur{ queue[queue.size()-1] };
        queue.pop_back();

        if (!is_consist(queue, cur))
            visited.push_back(cur);

        if (cells[std::get<0>(cur)][std::get<1>(cur)].is_bombed() ||
            cells[std::get<0>(cur)][std::get<1>(cur)].is_opened() ||
            cells[std::get<0>(cur)][std::get<1>(cur)].is_flaged())
            continue;

        cells[std::get<0>(cur)][std::get<1>(cur)].set_open();

        if (cells[std::get<0>(cur)][std::get<1>(cur)].bombs_around > 0)
        {
            switch (cells[std::get<0>(cur)][std::get<1>(cur)].bombs_around) {
            case 1:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("1.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            case 2:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("2.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            case 3:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("3.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            case 4:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("4.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            case 5:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("5.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            case 6:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("6.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            case 7:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("7.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            case 8:
                cells[std::get<0>(cur)][std::get<1>(cur)].set_img("8.png");
                attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
                break;
            default:
                std::cerr<<"Error: image didt't attached\n";
                return;
            }
            Fl::redraw();
            continue;
        }
        else
        {
            cells[std::get<0>(cur)][std::get<1>(cur)].set_img("btn_pushed.png");
            attach(*cells[std::get<0>(cur)][std::get<1>(cur)].img_ptr);
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
