#include <math/Size.h>


Size::Size()
  : d_{0, 0}
{}


Size::Size(int widthv, int heightv)
  : d_{widthv, heightv}
{}


Size::~Size()
{}


int Size::getHeight() const
{ return d_[1]; }
void Size::setHeight(int heightv)
{ d_[1] = heightv; }
int Size::height() const
{ return getHeight(); }
void Size::height(int heightv)
{ setHeight(heightv); }
int Size::h() const
{ return getHeight(); }
void Size::h(int heightv)
{ setHeight(heightv); }


int Size::getWidth() const
{ return d_[0]; }
void Size::setWidth(int widthv)
{ d_[0] = widthv; }
int Size::width() const
{ return getWidth(); }
void Size::width(int widthv)
{ setWidth(widthv); }
int Size::w() const
{ return getWidth(); }
void Size::w(int widthv)
{ setWidth(widthv); }


void Size::transpose()
{
  const int temp = d_[0];
  d_[0] = d_[1];
  d_[1] = temp;
}

Size& Size::operator+=(const Size& rhs)
{
  d_[1] += rhs.d_[1];
  d_[0] += rhs.d_[0];
  return *this;
}

Size& Size::operator-=(const Size& rhs)
{
  d_[1] -= rhs.d_[1];
  d_[0] -= rhs.d_[0];
  return *this;
}

Size& Size::operator*=(double rhs)
{
  d_[1] = static_cast<int>(d_[1] * rhs);
  d_[0] = static_cast<int>(d_[0] * rhs);
  return *this;
}

Size& Size::operator/=(double rhs)
{
  d_[1] = static_cast<int>(d_[1] / rhs);
  d_[0] = static_cast<int>(d_[0] / rhs);
  return *this;
}

const int* Size::getData() const
{return d_;}
int* Size::getData()
{return d_;}

