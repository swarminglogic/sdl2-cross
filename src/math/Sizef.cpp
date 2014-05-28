#include <math/Sizef.h>

#include <math/Pointf.h>


Sizef::Sizef()
  : d_{0.0f, 0.0f}
{}


Sizef::Sizef(float widthv, float heightv)
  : d_{widthv, heightv}
{}


Sizef::~Sizef()
{}


float Sizef::getHeight() const
{ return d_[1]; }
void Sizef::setHeight(float heightv)
{ d_[1] = heightv; }
float Sizef::height() const
{ return getHeight(); }
void Sizef::height(float heightv)
{ setHeight(heightv); }
float Sizef::h() const
{ return getHeight(); }
void Sizef::h(float heightv)
{ setHeight(heightv); }


float Sizef::getWidth() const
{ return d_[0]; }
void Sizef::setWidth(float widthv)
{ d_[0] = widthv; }
float Sizef::width() const
{ return getWidth(); }
void Sizef::width(float widthv)
{ setWidth(widthv); }
float Sizef::w() const
{ return getWidth(); }
void Sizef::w(float widthv)
{ setWidth(widthv); }


float Sizef::getLength() const
{
  return std::sqrt(d_[0]*d_[0] + d_[1]*d_[1]);
}

void Sizef::normalize()
{
  const float length = getLength();
  d_[0] /= length;
  d_[1] /= length;
}

void Sizef::transpose()
{
  const float temp = d_[0];
  d_[0] = d_[1];
  d_[1] = temp;
}

Sizef& Sizef::operator+=(const Sizef& rhs)
{
  d_[1] += rhs.d_[1];
  d_[0] += rhs.d_[0];
  return *this;
}

Sizef& Sizef::operator-=(const Sizef& rhs)
{
  d_[1] -= rhs.d_[1];
  d_[0] -= rhs.d_[0];
  return *this;
}

Sizef& Sizef::operator*=(float rhs)
{
  d_[1] *= rhs;
  d_[0] *= rhs;
  return *this;
}

Sizef& Sizef::operator/=(float rhs)
{
  d_[1] /= rhs;
  d_[0] /= rhs;
  return *this;
}

const float* Sizef::getData() const
{return d_;}
float* Sizef::getData()
{return d_;}

Sizef::operator Pointf() const {
  return Pointf(d_[0], d_[1]);
}
