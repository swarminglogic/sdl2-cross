#include <math/Point.h>


Point::Point()
  : d_{0, 0}
{}


Point::Point(int xv, int yv)
  : d_{xv, yv}
{}


Point::~Point()
{}


int Point::getY() const
{ return d_[1]; }
void Point::setY(int yv)
{ d_[1] = yv; }

int Point::y() const
{ return d_[1]; }
void Point::y(int yv)
{ d_[1] = yv; }


int Point::getX() const
{ return d_[0]; }
void Point::setX(int xv)
{ d_[0] = xv; }


int Point::x() const
{ return d_[0]; }
void Point::x(int xv)
{ d_[0] = xv; }


void Point::transpose()
{
  const int temp = d_[0];
  d_[0] = d_[1];
  d_[1] = temp;
}

Point& Point::operator+=(const Point& rhs)
{
  d_[1] += rhs.d_[1];
  d_[0] += rhs.d_[0];
  return *this;
}

Point& Point::operator-=(const Point& rhs)
{
  d_[1] -= rhs.d_[1];
  d_[0] -= rhs.d_[0];
  return *this;
}

Point& Point::operator*=(double rhs)
{
  d_[1] = static_cast<int>(d_[1] * rhs);
  d_[0] = static_cast<int>(d_[0] * rhs);
  return *this;
}

Point& Point::operator/=(double rhs)
{
  d_[1] = static_cast<int>(d_[1] / rhs);
  d_[0] = static_cast<int>(d_[0] / rhs);
  return *this;
}

const int* Point::getData() const
{return d_;}
int* Point::getData()
{return d_;}
