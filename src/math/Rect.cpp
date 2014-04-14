#include <math/Rect.h>


Rect::Rect(const Point& position, const Size& size)
  : pos_(position),
    size_(size)
{}

Rect::Rect(const Size& size)
  : Rect(Point(0, 0), size)
{}

Rect::Rect(int xv, int yv, int wv, int hv)
  : Rect(Point(xv, yv), Size(wv, hv))
{}


Rect::~Rect() {}


int Rect::x() const
{return pos_.x();}
void Rect::x(int xv)
{pos_.x(xv);}

int Rect::y() const
{return pos_.y();}
void Rect::y(int yv)
{pos_.y(yv);}

int Rect::w() const
{return size_.w();}
void Rect::w(int wv)
{size_.w(wv);}

int Rect::h() const
{return size_.h();}
void Rect::h(int hv)
{size_.h(hv);}


void Rect::setPosition(const Point& position)
{pos_ = position;}
Point Rect::getPosition() const
{return pos_;}

void Rect::setSize(const Size& size)
{size_ = size;}
Size Rect::getSize() const
{return size_;}


const int* Rect::getData() const
{
  return pos_.getData();
}

int* Rect::getData()
{
  return pos_.getData();
}

