#include <iostream>
#include "app.h"

using namespace structures;

void main(int argc, const char* argv[]) {
   App app;
   app.init(argc, argv);
   app.run();
}