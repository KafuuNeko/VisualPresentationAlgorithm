#pragma once

#include <functional>
#include <time.h>

inline void delay(double seconds, std::function<bool()> events) {
  double start = clock();
  double lay = static_cast<double>(seconds) * CLOCKS_PER_SEC;
  while ((clock() - start) < lay && events())
    ;
}