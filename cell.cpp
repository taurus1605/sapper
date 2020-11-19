#include "cell.h"
#include <iostream>

Button_with_pw::Button_with_pw(Graph_lib::Point p, int w, int h, const std::string& label,  Graph_lib::Callback cb)
    :Button(p, w,h,label , cb)
{}


Cell::Cell(Graph_lib::Point p, int size, Graph_lib::Callback cb)
    :Button_with_pw{p, size, size,"", cb}
{
  set_img("btn_pulled.png");
}

void Cell::attach(Graph_lib::Window &win){
  Button::attach(win);
  win.attach(*img_ptr);
}

void Cell::set_img(std::string name){
  if(img_ptr != nullptr) (*img_ptr).~Image();
  img_ptr = new Graph_lib::Image(loc, name);
}


