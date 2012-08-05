/*
measure.hpp (Copyright 2003 David J. Aronson)
System for avoiding improper mixing of units of measure
*/

#ifndef MEASURE_H
#define MEASURE_H

class Unit;

class Measure;

class Measure
{
public:
  // Constructors
  Measure (void);
  Measure (Unit * u);
  Measure (double q, Unit * u);
  // Member methods
  double   GetQuantity() { return quantity; }
  Unit *   GetUnit() { return unit; }
  Measure  power (int pow);
  Measure  root (int pow);
  Measure  operator + (Measure m);
  Measure  operator - (Measure m);
  Measure  operator * (Measure m);
  Measure  operator / (Measure m);
  void     operator += (Measure m) { *this = *this + m; }
  void     operator -= (Measure m) { *this = *this - m; }
  void     operator *= (Measure m) { *this = *this * m; }
  void     operator /= (Measure m) { *this = *this / m; }
  void     operator *= (double d) { *this = *this * d; }
  void     operator /= (double d) { *this = *this / d; }
  Measure  operator * (double d) { return Measure (quantity * d, unit); }
  Measure  operator / (double d) { return Measure (quantity / d, unit); }
  // overloading of + and - to add numbers purposely OMITTED!
  // same for autoincrement/autodecrement
  Measure  operator = (Measure m);
  int      operator == (Measure m);
  int      operator < (Measure m);
  int      operator > (Measure m);
  int      operator <= (Measure m);
  int      operator >= (Measure m);
protected:
  // Member data
  double  quantity;
  Unit *  unit;
  // Member methods
  void    CheckUnits (Unit * u1, Unit * u2);
};

#endif // ifndef MEASURE_H


// END OF FILE
