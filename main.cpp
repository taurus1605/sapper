#include "miner.h"
#include <iostream>


int main ()
try
{
  Sapper sss{ Graph_lib::Point(100, 100) };

  return Graph_lib::gui_main();
}
catch (std::exception& e)
{
  std::cerr << e.what() << std::endl;
}
catch (...)
{
  std::cerr << "Ooops, smth went wrong. Call to developer!" << std::endl;
}

