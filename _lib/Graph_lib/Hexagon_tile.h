#ifndef HEXAGON_TILE_H
#define HEXAGON_TILE_H 1


#include "GUI.h"
#include "Hexagon.h"


namespace Graph_lib
{

struct Hexagon_tile : Rectangle
{
  Hexagon_tile (Point p, int ww, int hh, int rr);

  void move (int dx, int dy) override;

protected:
  void draw_lines () const override;
private:
  Vector_ref<Hexagon> tile;
};

}

#endif // HEXAGON_TILE_H
