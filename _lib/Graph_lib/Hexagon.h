#ifndef HEXAGON_H
#define HEXAGON_H 1


#include "GUI.h"


namespace Graph_lib {

  struct Hexagon : Shape
  {
    Hexagon (Point p, int r);

    Point center () const;
    int width () const;
    int height () const;
    int edge () const;

  protected:
      void draw_lines () const override;
  };

} // of namespace Graph_lib


#endif // HEXAGON_H
