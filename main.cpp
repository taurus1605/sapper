#include <iostream>

#include "constants.h"
#include "lib.h"
#include "game.h"


int main()
try
{
  Sapper game{ Graph_lib::Point(200, 200) };

  return Graph_lib::gui_main();
}
catch (std::exception& e)
{
  std::cerr << "Error: " << e.what() << std::endl;
  return 1;
}
catch (...)
{
  std::cerr << "Something went wrong." << std::endl;
  return 2;
}
