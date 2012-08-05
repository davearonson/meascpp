#include <iostream>
#include "measure.hpp"
#include "measuredefs.hpp"
#include "unit.hpp"
#include "unitdefs.hpp"


double Convert
(
  double   qty,
  Unit *   uIn,
  Unit *   uOut,
  char     op,
  Measure  cvtFactor
);
void GiveUsage();


int main (int argc, char * argv[])
{
  double   dIn;
  double   dOut;
  Measure  factor;
  char     op;
  Unit *   uIn;
  Unit *   uOut;

  if (argc != 3) GiveUsage();
  dIn = atof (argv[1]);
  switch (argv[2][0])
  {
    case 'f': case 'F':
      factor = CONVERT_FEETPERMETER;
      op = '/';
      uIn = &FOOT;
      uOut = &METER;
    break;
    case 'k': case 'K':
      factor = CONVERT_SLUGSPERKILOGRAM;
      op = '*';
      uIn = &KILOGRAM;
      uOut = &SLUG;
    break;
    case 'm': case 'M':
      factor = CONVERT_FEETPERMETER;
      op = '*';
      uIn = &METER;
      uOut = &FOOT;
    break;
    case 's': case 'S':
      factor = CONVERT_SLUGSPERKILOGRAM;
      op = '/';
      uIn = &SLUG;
      uOut = &KILOGRAM;
    break;
    default: GiveUsage(); break;
  }
  dOut = Convert (dIn, uIn, uOut, op, factor);

  cout << dIn << ' ' << uIn->GetName() << " = ";
  cout << dOut << ' ' << uOut->GetName() << endl;

  exit (0);
}


double Convert
(
  double   qty,
  Unit *   uIn,
  Unit *   uOut,
  char     op,
  Measure  cvtFactor
)
{
  Measure  mIn = Measure (qty, uIn);
  Measure  mOut = Measure (uOut);

  if (op == '*') mOut = mIn * cvtFactor;
  else if (op == '/') mOut = mIn / cvtFactor;
  else throw Unit::BadOperatorError (op);

  return mOut.GetQuantity();
}


void GiveUsage()
{
  cout << "convert: convert between feet/meters or kilos/slugs" << endl;
  cout << "Usage: convert quantity unit" << endl;
  cout << "where convert is a number, and unit is f, m, k, or s" << endl;
  cout << "(slug is the Imperial unit of mass, upon which" << endl;
  cout << "Earth's surface gravity exerts a force of one pound.)" << endl;
  exit (1);
}



// END OF FILE
