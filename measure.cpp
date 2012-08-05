/*
measure.cpp (Copyright 2003 David J. Aronson)
System for avoiding improper mixing of units of measure
*/

#include <math.h>

#include "measure.hpp"
#include "unit.hpp"


#include "unitdefs.hpp"

// TODO: these should be moved out to an optional codefile
// first some reasonably normal constants
Measure G = Measure (9.8, &MpSpS);
// next some conversions; have to declare their units too
Unit FEETPERMETER = Unit ("feet per meter", &FOOT, '/', &METER);
Measure CONVERT_FEETPERMETER = Measure (3.280833333333333333333, &FEETPERMETER);
Unit SLUGSPERKILOGRAM = Unit ("slugs per kilogram", &SLUG, '/', &KILOGRAM);
Measure CONVERT_SLUGSPERKILOGRAM = Measure (2.2, &SLUGSPERKILOGRAM);


// PUBLIC STUFF


// Constructors


// This is for when we don't even know what unit the Measure will be in.
// Note that ONLY such a Measure can change its unit!
Measure::Measure (void)
{
  quantity = 0;
  unit = NULL;
}


// used just to declare that an obj is of this unit,
// when we don't know the quantity yet
Measure::Measure (Unit * u)
{
  quantity = 0;
  unit = u;
}
 
 
// declare a measure, and set it to a quantity at the same time
Measure::Measure (double q, Unit * u)
{
  quantity = q;
  unit = u;
}


// Member methods -- mostly overloaded ops and other basic math


// raise a measure to a power, like c in e=mc^2
Measure Measure::power (int power)
{
  double  d = quantity;
  int     i;
  int     powTmp = (power > 0) ? power : -power;

  if (power == 0) return Measure (1, &Unit::UNITLESS);
  if (power == 1) return *this;
  for (i = 1; i < powTmp; i++) d *= quantity;
  if (power < 0) d = 1.0 / d;
  return Measure (d, unit->power (power));
}


// take the nth root of a measure, like da in t = sqrt (2da)
// (time it takes something to go distance "d" under acceleration "a")
Measure Measure::root (int power)
{
  if (power == 1) return *this;
  return Measure (pow (quantity, 1.0/power), unit->root (power));
}


Measure Measure::operator + (Measure m)
{
  CheckUnits (unit, m.unit);
  return Measure (quantity + m.quantity, unit);
}


Measure Measure::operator - (Measure m)
{
  CheckUnits (unit, m.unit);
  return Measure (quantity - m.quantity, unit);
}


Measure Measure::operator * (Measure m)
{
  return Measure (quantity * m.quantity,
                  Unit::FindUnitByBuildup (unit, '*', m.unit));
}


Measure Measure::operator / (Measure m)
{
  return Measure (quantity / m.quantity,
                  Unit::FindUnitByBuildup (unit, '/', m.unit));
}


Measure Measure::operator = (Measure m)  // ASSIGNMENT
{
  if (unit == NULL) unit = m.unit;
  else CheckUnits (unit, m.unit);
  quantity = m.quantity;
  return *this;
}


int Measure::operator == (Measure m)  // EQUALITY
{
  CheckUnits (unit, m.unit);
  return quantity == m.quantity;
}


int Measure::operator < (Measure m)
{
  CheckUnits (unit, m.unit);
  return quantity < m.quantity;
}


int Measure::operator > (Measure m)
{
  CheckUnits (unit, m.unit);
  return quantity > m.quantity;
}


int Measure::operator <= (Measure m)
{
  CheckUnits (unit, m.unit);
  return quantity <= m.quantity;
}


int Measure::operator >= (Measure m)
{
  CheckUnits (unit, m.unit);
  return quantity >= m.quantity;
}


// PROTECTED STUFF


// Member methods


// make sure the units are compatible --
// should even be same pointers, but....
void Measure::CheckUnits (Unit * u1, Unit * u2)
{
  if (*u1 != *u2) throw Unit::MismatchError (u1, u2);
}


// END OF FILE
