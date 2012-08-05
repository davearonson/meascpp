#include <iostream>
#include "measure.hpp"
#include "measuredefs.hpp"
#include "unit.hpp"
#include "unitdefs.hpp"

void ParseArgs (int argc, char * argv[], Measure * time);

int main (int argc, char * argv[])
{
  Measure height = Measure (&METER);
  Measure time = Measure (&SECOND);

  ParseArgs (argc, argv, &height);

  time = (height * 2.0 / G).root (2);  // t = sqrt (2d/a)
  cout << "At " << G.GetQuantity();
  cout << " meters per second per second," << endl << "an object will fall ";
  cout << height.GetQuantity() << " meters in ";
  cout << time.GetQuantity() << " seconds." << endl;
  exit (0);
}


void GiveUsage()
{
  cout << "falltime: tell how long an object will take to fall a given" << endl;
  cout << "          height at 9.8 m/s/s (ignoring air resistance)." << endl;
  cout << "Usage: falltime height" << endl;
  cout << "where height is a non-negative number, in meters" << endl;
  exit (1);
}


void ParseArgs (int argc, char * argv[], Measure * height)
{
  double d;

  if (argc != 2) GiveUsage();
  d = atof (argv[1]);
  if (d < 0) GiveUsage();
  *height = Measure (d, &METER);
}


// END OF FILE
