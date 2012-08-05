/*
unit.hpp (Copyright 2003 David J. Aronson)
System for declaring and managing units of measure.
See also measure.*
*/


#ifndef UNIT_H
#define UNIT_H

#include <vector>
#include <string>
using namespace std;


#define UNITNAMELEN 16


class Unit;

typedef vector <Unit *> UnitVector;
typedef UnitVector::iterator UnitIterator;

typedef unsigned long ulong;

class Unit
{
public:
  // Constructors
  Unit (string n);
  Unit (string  n, Unit *  u1, char  op, Unit *  u2);
  // Destructor
  ~Unit (void);
  // Member data -- NONE!
  // Member methods
  string  GetBreakdown (void);
  string  GetName (void) { return name; }
  Unit *  power (int pow);
  Unit *  root (int pow);
  int operator == (Unit & u);
  int operator != (Unit & u) { return ! (*this == u); }
  // Static methods
  static Unit * FindUnitByBuildup (Unit *  u1, char  op, Unit *  u2);
  static string GetAllBreakdowns (void);
  // Static data
  static Unit  UNITLESS;
  // Exception classes
  class BadNameError
  {
  public:
    string name;
    BadNameError (string n) { name = n; }
  };
  class BadOperatorError
  {
  public:
    char op;
    BadOperatorError (char c) { op = c; }
  };
  class BadRootError
  {
  public:
    Unit *  unit;
    int     root;
    BadRootError (Unit * u, int r)
    {
      unit = u;
      root = r;
    }
  };
  class MismatchError
  {
  public:
    Unit *u1, *u2;
    MismatchError (Unit * a, Unit * b)
    {
      u1 = a;
      u2 = b;
    }
  };
  class NameReuseError
  {
  public:
    ulong   denominator;
    string  name;
    ulong   numerator;
    Unit *  old;
    NameReuseError (string s, ulong num, ulong den, Unit * u)
    {
      denominator = den;
      name = s;
      numerator = num;
      old = u;
    }
  };
  class NotFoundError
  {
  public:
    Unit *u;
    NotFoundError (Unit * bad) { u = bad; }
  };
protected:
  // Member data
  string  name;
  ulong   numerator;
  ulong   denominator;
  // Static data
  static ulong       lastPrime; // see name-only constructor
  static ulong       lastTemp;  // see no-name constructor (prot)
  static UnitVector  knownUnits;
  static UnitVector  baseUnits;
  // Constructors
  Unit (string n, ulong num, ulong den) { UnitInit (n, num, den); }
  Unit (Unit * u1, char op, Unit * u2);
  // Other member methods
  void  DelFrom (UnitVector * v, char mustFind);
  void  UnitBuildup (string n, Unit * u1, char op, Unit * u2);
  void  UnitInit (string n, ulong num, ulong den);
  void  CheckCompatibility (Unit & u);
  void  Reduce (void) { Reduce (&numerator, &denominator); }
  // Static methods
  static void    CalcBuildupNumbers (Unit * u1, char op, Unit * u2,
                                     ulong * num, ulong * den);
  static Unit *  FindUnitByNumbers (ulong num, ulong den);
  static Unit *  FindOrMakeUnitByNumbers (ulong num, ulong den);
  static string  GetPartialBreakdown (ulong u);
  static void    Reduce (ulong * num, ulong * den);
};

#endif


// END OF FILE
