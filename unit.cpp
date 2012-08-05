/*
unit.cpp -- system for declaring and managing units of measure
Copyright 2003 David J. Aronson
See also measure.*
*/


#include <math.h>
#include <iostream>
#include <vector>
using namespace std;


#include "unit.hpp"


// NOTE: THESE FOUR ARE OUT OF THE USUAL ORDER BECAUSE
// THEY MUST BE INITTED BEFORE ANY UNITS CAN BE CREATED!!!
UnitVector Unit::knownUnits;  // should replace with class introspection....
UnitVector Unit::baseUnits;
ulong Unit::lastPrime = 1;    // yes, that's not a prime... see Unit (name)
ulong Unit::lastTemp = 0;


// must be in Unit to be able to dictate numerator and denominator,
// plus it makes sense since it's not part of one specific system of units
Unit Unit::UNITLESS = Unit ("unitless", 1, 1);


// TODO: these should be moved out to an optional codefile
// first we declare some universal units...
Unit SECOND = Unit ("second");
// now some metric units...
Unit KILOGRAM = Unit ("kilogram");
Unit METER = Unit ("meter");
Unit COULOMB = Unit ("coulomb");
Unit MpS = Unit ("m/s", &METER, '/', &SECOND);
Unit MpSpS = Unit ("m/s^2", &MpS, '/', &SECOND);
Unit NEWTON = Unit ("newton", &KILOGRAM, '*', &MpSpS);
Unit JOULE = Unit ("joule", &METER, '*', &NEWTON);
Unit WATT = Unit ("watt", &JOULE, '/', &SECOND);
Unit M2 = Unit ("m^2", &METER, '*', &METER);
Unit PASCAL = Unit ("pascal", &NEWTON, '/', &M2);
Unit AMPERE = Unit ("ampere", &COULOMB, '/', &SECOND);
Unit VOLT = Unit ("volt", &WATT, '/', &AMPERE);
Unit OHM = Unit ("ohm", &VOLT, '/', &AMPERE);
// now the "English" units...
Unit FOOT = Unit ("foot");
Unit SLUG = Unit ("slug");  // THIS is the unit of mass!  Lbs are FORCE!
Unit FPS = Unit ("fps", &FOOT, '/', &SECOND);
Unit FPSPS = Unit ("fpsps", &FPS, '/', &SECOND);
Unit POUND = Unit ("pound", &SLUG, '*', &FPSPS);  // THIS IS FORCE, NOT MASS!!!
Unit FOOTPOUND = Unit ("foot-pound", &FOOT, '*', &POUND);
// ...and we'll skip anything else, including Knuth's first published work.


// PUBLIC STUFF


// Constructors


// make a base unit
Unit::Unit (string n)
{
  UnitIterator  basesEnd = baseUnits.end();
  char          foundPrime;
  UnitIterator  it;

  // find the next prime -- base units being primes is the key!
  do
  {
    ++lastPrime;
    foundPrime = 1;
    for (it = baseUnits.begin(); it != basesEnd && foundPrime != 0; it++)
    {
      if (lastPrime % (*it)->numerator == 0) foundPrime = 0;
    }
  } while (foundPrime == 0);
  UnitInit (n, lastPrime, 1);
  baseUnits.push_back (this);
}


// make a unit by multiplying or dividing existing units
Unit::Unit (string n, Unit * u1, char op, Unit * u2)
{
  if (op != '*' && op != '/') throw BadOperatorError (op);
  UnitBuildup (n, u1, op, u2);
  // eventually might check if it wound up identical to some base unit.
  // not sure what to do then if the names are different...
}


// Destructor


// delete units from lists where we might want to find them
Unit::~Unit (void)
{
  DelFrom (&knownUnits, 1);
  if (denominator == 1) DelFrom (&baseUnits, 0);
}


// Member methods


// Print the details of a unit, mainly for debugging purposes
string Unit::GetBreakdown (void)
{
  char    buf[32];
  string  s;

  s = GetPartialBreakdown (numerator);
  if (denominator != 1)
  {
    s += " / ";
    s += GetPartialBreakdown (denominator);
  }
  sprintf (buf, " (%lu/%lu)", numerator, denominator);
  s += buf;
  return s;
}


// raise a unit to a power, e.g., (m/s).power (3) = (m^3 / s^3)
Unit * Unit::power (int power)
{
  ulong  d = denominator;
  int    i;
  ulong  n = numerator;
  int    powTmp = (power > 0) ? power : -power;

  if (power == 0) return &UNITLESS;
  if (power == 1) return this;
  for (i = 1; i < powTmp; i++)
  {
    d *= denominator;
    n *= numerator;
  }
  if (power < 0)
  {
    ulong u = d;
    d = n;
    n = u;
  }
  return FindOrMakeUnitByNumbers (n, d);
}


// take the n'th root of a unit, e.g., (m^3 / s^3).root (3) = m/s
Unit * Unit::root (int power)
{
  int    powTmp = (power > 0) ? power : -power;
  ulong  den = (ulong) floor (pow (denominator, 1.0/powTmp));
  ulong  num = (ulong) floor (pow (numerator, 1.0/powTmp));
  ulong  swap;

  // verify!
  if ((ulong) floor (pow ((double) num, (double) powTmp)) != numerator)
  {
    throw BadRootError (this, power);
  }
  if ((ulong) floor (pow ((double) den, (double) powTmp)) != denominator)
  {
    throw BadRootError (this, power);
  }
  if (power < 0)
  {
    swap = den;
    den = num;
    num = swap;
  }
  return FindOrMakeUnitByNumbers (num, den);
}


// compare two units for equality
int Unit::operator == (Unit & u)
{
  if (numerator != u.numerator) return 0;
  if (denominator != u.denominator) return 0;
  return 1;
}


// Static methods


// find the unit that matches a given construction from other units.
// for now, simple linear search.
// later, might replace by binary search, with knownUnits sorted
// (probably on (numerator << (8 * sizeof (ulong)) + denominator)
Unit * Unit::FindUnitByBuildup (Unit *  u1, char  op, Unit *  u2)
{
  ulong  den;
  ulong  num;

  if (op != '*' && op != '/') throw BadOperatorError (op);
  CalcBuildupNumbers (u1, op, u2, &num, &den);
  return FindOrMakeUnitByNumbers (num, den);
}


// print out all the units there are (mainly for debugging purposes)
string Unit::GetAllBreakdowns (void)
{
  UnitIterator  end = knownUnits.end();
  UnitIterator  it;
  string        s = "";
  for (it = knownUnits.begin(); it != end; it++)
  {
    s += (*it)->GetName() + ": " + (*it)->GetBreakdown() + '\n';
  }
  return s;
}


// PROTECTED ITEMS


// Constructors


// make a TEMPORARY unit by multiplying or dividing existing units
Unit::Unit (Unit * u1, char op, Unit * u2)
{
  if (op != '*' && op != '/') throw BadOperatorError (op);
  UnitBuildup ("", u1, op, u2);
}


// Other member methods


// delete unit from list, throwing error if required but not found
void Unit::DelFrom (UnitVector * v, char mustFind)
{
  UnitIterator end = v->end();
  UnitIterator it;

  for (it = v->begin(); it != end; it++)
  {
    if (*it == this)
    {
      v->erase (it);
      break;
    }
  }
  if (mustFind && it == end) throw NotFoundError (this);
}


// set a unit's fields based on building it up from extant units
void Unit::UnitBuildup (string n, Unit * u1, char op, Unit * u2)
{
  ulong  den;
  ulong  num;
  if (op != '*' && op != '/') throw BadOperatorError (op);
  CalcBuildupNumbers (u1, op, u2, &num, &den);
  UnitInit (n, num, den);
}


// do the main initialization of a new unit: set name, numerator, and
// denominator, reduce to lowest terms, and add to list of known units.
// TODO: check for name validity (if supplied, must not start w/ space),
// and reuse (in which case, throw NameReuseError, TBW).
void Unit::UnitInit (string n, ulong num, ulong den)
{
  UnitIterator  it;
  UnitIterator  unitsEnd = knownUnits.end();

  if (n[0] == ' ') throw BadNameError (n);
  else if (n != "") name = n;
  else
  {
    char  tmpStr[11];
    name = " TEMP";
    sprintf (tmpStr, "%lu", lastTemp++); // theoretically could overflow....
    name += tmpStr;
  }
  numerator = num;
  denominator = den;
  Reduce();
  for (it = knownUnits.begin(); it != unitsEnd; it++)
  {
    if (name == (*it)->name)
    {
      // remove that one, not this, because we're called from constructor.
      // we could throw error, but replacement is harmless.
      if (*(*it) == *this) knownUnits.remove (it);
      else throw NameReuseError (name, num, den, *it);
      break;
    }
  }
  knownUnits.push_back (this);
}


// Static methods


// Calculate the numbers that would be the numerator and denominator,
// of the unit that results from doing the indicated operation
// on the given existing units.
void Unit::CalcBuildupNumbers (Unit * u1, char op, Unit * u2,
                              ulong * num, ulong *den)
{
  *num = u1->numerator * ((op == '*') ? u2->numerator : u2->denominator);
  *den = u1->denominator * ((op == '*') ? u2->denominator : u2->numerator);
  Reduce (num, den);
}


// Find a unit by its numerator and denominator,
// and if not found, create it
Unit * Unit::FindOrMakeUnitByNumbers (ulong num, ulong den)
{
  Unit  * u = FindUnitByNumbers (num, den);
  if (u != NULL) return u;
  else return new Unit ("", num, den);
}


// Find a unit by its numerator and denominator,
// and if not found, return NULL
Unit * Unit::FindUnitByNumbers (ulong num, ulong den)
{
  UnitIterator  it;
  UnitIterator  unitsEnd = knownUnits.end();

  for (it = knownUnits.begin(); it != unitsEnd; it++)
  {
    if ((*it)->numerator == num && (*it)->denominator == den) return *it;
  }
  return NULL;
}


// print PART of a unit (either numerator or denominator),
// in terms of what base units make it up
string Unit::GetPartialBreakdown (ulong u)
{
  UnitIterator  basesEnd = baseUnits.end();
  UnitIterator  it;
  string        s = "";

  for (it = baseUnits.begin(); it != basesEnd; it++)
  {
    ulong num = (*it)->numerator;
    if (u % num == 0)
    {
      int  i;
      if (s != "") s += ' ';
      s += (*it)->name;
      for (i = 0; u % num == 0; i++) u /= num;
      if (i > 1)
      {
        char  buf[6];
        s += '^';
        sprintf (buf, "%u", i);
        s += buf;
      }
    }
  }
  return s;
}


// Reduce a numerator and denominator to lowest terms.
// (Note: only divides by extant base units.  No other factors SHOULD
// be able to creep in there, BUT....)
void Unit::Reduce (ulong * num, ulong * den)
{
  UnitIterator  basesEnd = baseUnits.end();
  UnitIterator  it;

  for (it = baseUnits.begin(); it != basesEnd; it++)
  {
    ulong u = (*it)->numerator;
    while (*num % u == 0 && *den % u == 0)
    {
      *num /= u;
      *den /= u;
    }
  }
}


// END OF FILE
