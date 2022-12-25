#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "ArborMaster" ? 0 : 1;
}
