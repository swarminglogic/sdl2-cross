#include <math/Rectf.h>


Rectf::Rectf(const Pointf& position, const Sizef& size)
  : pos_(position),
    size_(size)
{}


Rectf::Rectf(const Sizef& size)
  : Rectf(Pointf(0.0f, 0.0f), size)
{}

Rectf::Rectf(float xv, float yv, float wv, float hv)
  : Rectf(Pointf(xv, yv), Sizef(wv, hv))
{}


Rectf::~Rectf() {}


float Rectf::x() const
{return pos_.x();}
void Rectf::x(float xv)
{pos_.x(xv);}

float Rectf::y() const
{return pos_.y();}
void Rectf::y(float yv)
{pos_.y(yv);}

float Rectf::w() const
{return size_.w();}
void Rectf::w(float wv)
{size_.w(wv);}

float Rectf::h() const
{return size_.h();}
void Rectf::h(float hv)
{size_.h(hv);}


void Rectf::setPosition(const Pointf& position)
{pos_ = position;}
Pointf Rectf::getPosition() const
{return pos_;}

void Rectf::setSize(const Sizef& size)
{size_ = size;}
Sizef Rectf::getSize() const
{return size_;}


const float* Rectf::getData() const
{
  return pos_.getData();
}

float* Rectf::getData()
{
  return pos_.getData();
}

