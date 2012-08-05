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

  ParseArgs (argc, argv, &time);

  height = G * 0.5 * time.power (2);  // d = 1/2 a t^2
  cout << "At " << G.GetQuantity();
  cout << " meters per second per second," << endl << "an object will fall ";
  cout << height.GetQuantity() << " meters in ";
  cout << time.GetQuantity() << " seconds." << endl;
  exit (0);
}


void GiveUsage()
{
  cout << "falldist: tell how far an object will fall at 9.8 m/s/s" << endl;
  cout << "          in a given number of seconds (ignoring air resistance)." << endl;
  cout << "Usage: falldist time" << endl;
  cout << "where time is a non-negative number, in seconds" << endl;
  exit (1);
}


void ParseArgs (int argc, char * argv[], Measure * time)
{
  double d;

  if (argc != 2) GiveUsage();
  d = atof (argv[1]);
  if (d < 0) GiveUsage();
  *time = Measure (d, &SECOND);
}


// END OF FILE
