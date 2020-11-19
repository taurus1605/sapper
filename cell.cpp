#include "cell.h"
#include "lib.h"


Button_with_pw::Button_with_pw (Graph_lib::Point p, int size_, Graph_lib::Callback cb)
  : Graph_lib::Button{p, size_,size_,"", cb}
{ }

Cell::Cell (Graph_lib::Point p, int size, Graph_lib::Callback cb)
  : Button_with_pw{ p, size, cb }
{ }

void Cell::show_bomb ()
{
  get_pw()->color(Graph_lib::Color::dark_red);
}

