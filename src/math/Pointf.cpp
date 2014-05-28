#include <math/Pointf.h>

#include <math/Sizef.h>


Pointf::Pointf()
  : d_{0.0f, 0.0f}
{}


Pointf::Pointf(float xv, float yv)
  : d_{xv, yv}
{}


Pointf::~Pointf()
{}


float Pointf::getY() const
{ return d_[1]; }
void Pointf::setY(float yv)
{ d_[1] = yv; }
float Pointf::y() const
{ return d_[1]; }
void Pointf::y(float yv)
{ d_[1] = yv; }


float Pointf::getX() const
{ return d_[0]; }
void Pointf::setX(float xv)
{ d_[0] = xv; }
float Pointf::x() const
{ return d_[0]; }
void Pointf::x(float xv)
{ d_[0] = xv; }


float Pointf::getLength() const
{
  return std::sqrt(d_[0]*d_[0] + d_[1]*d_[1]);
}

void Pointf::normalize()
{
  const float length = getLength();
  d_[0] /= length;
  d_[1] /= length;
}

void Pointf::transpose()
{
  const float temp = d_[0];
  d_[0] = d_[1];
  d_[1] = temp;
}

Pointf& Pointf::operator+=(const Pointf& rhs)
{
  d_[1] += rhs.d_[1];
  d_[0] += rhs.d_[0];
  return *this;
}

Pointf& Pointf::operator-=(const Pointf& rhs)
{
  d_[1] -= rhs.d_[1];
  d_[0] -= rhs.d_[0];
  return *this;
}

Pointf& Pointf::operator*=(float rhs)
{
  d_[1] *= rhs;
  d_[0] *= rhs;
  return *this;
}

Pointf& Pointf::operator/=(float rhs)
{
  d_[1] /= rhs;
  d_[0] /= rhs;
  return *this;
}


const float* Pointf::getData() const
{return d_;}
float* Pointf::getData()
{return d_;}


Pointf::operator Sizef() const {
  return Sizef(d_[0], d_[1]);
}
