#include "GUI.h"
#include "Hexagon.h"


namespace Graph_lib
{

  Hexagon::Hexagon (Point p, int r)
  {
    constexpr int n = 6;

    for (int i{ 0 }; i < n; ++i)
    {
      Point cur;
      double phi = 2 * pi * i / n;

      cur.x = p.x + round(r * cos(phi));
      cur.y = p.y + round(r * sin(phi));

      add(cur);
    }
  }

  Point Hexagon::center() const
  {
    return Point{(point(0).x + point(3).x)/2, point(0).y};
  }

  int Hexagon::width() const
  {
    return point(0).x - point(3).x;
  }

  int Hexagon::height() const
  {
    return point(1).y - point(5).y;
  }

  int Hexagon::edge () const
  {
    return point(1).x - point(2).x;
  }

  void Hexagon::draw_lines () const
  {
    if (fill_color().visibility())
    {
      fl_color(fill_color().as_int());
      fl_begin_complex_polygon();
      for (int i = 0; i < number_of_points(); ++i)
      {
        fl_vertex(point(i).x, point(i).y);
      }
      fl_end_complex_polygon();
      fl_color(color().as_int());
    }

    if (color().visibility())
    {
      Shape::draw_lines();

      fl_line (point(number_of_points()-1).x, point(number_of_points()-1).y, point(0).x, point(0).y);
    }
  }

} // of namespace Graph_lib
