#include <math/MathUtil.h>

#include <algorithm>

#include <math/Pointf.h>
#include <math/Rectf.h>
#include <math/Size.h>
#include <math/Sizef.h>
#include <util/assert.h>


float MathUtil::PI = 3.14159265359f;
float MathUtil::PIHALF = 1.57079633f;
float MathUtil::PIQUARTER = 0.78539816f;

float MathUtil::lerp(float min, float max, float a)
{
  assert(min <= max);  // LCOV_EXCL_LINE
  a = clamp(a);
  return min + (max - min) * a;
}


float MathUtil::clamp(float value, float min, float max)
{
  return std::min(std::max(value, min), max);
}


float MathUtil::clamp(float value)
{
  return clamp(value, 0.0f, 1.0f);
}


uint8_t MathUtil::mapToU8(float value)
{
  assert(value >= 0.0f);  // LCOV_EXCL_LINE
  assert(value <= 1.0f);  // LCOV_EXCL_LINE
  static const float offset = 1.0f / 255.0f;
  return static_cast<uint8_t>(255u * value + offset);
}


uint8_t MathUtil::mapToU8special(float value)
{
  assert(value >= -1.0f);  // LCOV_EXCL_LINE
  assert(value <= 1.0f);  // LCOV_EXCL_LINE
  static const float red = 254.0f / 255.0f;
  return mapToU8(red *0.5f * (value + 1.0f));
}



unsigned int MathUtil::nextPow2(unsigned int v)
{
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

unsigned int MathUtil::nextSquare(unsigned int v)
{
  unsigned int base = nextSquareBase(v);
  return base*base;
}


unsigned int MathUtil::nextSquareBase(unsigned int v)
{
  unsigned int base =
    static_cast<unsigned int>(std::ceil(std::sqrt(static_cast<float>(v))));
  return base;
}


unsigned int MathUtil::prevSquare(unsigned int v)
{
  unsigned int base = prevSquareBase(v);
  return base*base;
}


unsigned int MathUtil::prevSquareBase(unsigned int v)
{
  if (v == 0) return 0u;
  unsigned int base =
    static_cast<unsigned int>(std::floor(std::sqrt(static_cast<float>(v))));
  return base;
}


bool MathUtil::isPow2(unsigned int v)
{
  return v && !(v & (v - 1));
}


bool MathUtil::isSquare(unsigned int v)
{
  return v == nextSquare(v);
}


float MathUtil::nextPow2TexCoord(float original, int dimension)
{
  const float ratio = (static_cast<float>(dimension) /
                       static_cast<float>(nextPow2(dimension)));
  return original * ratio;
}


Pointf MathUtil::nextPow2TexCoord(const Pointf& original,
                                  const Size& dimension)
{
  return Pointf(nextPow2TexCoord(original.x(), dimension.w()),
                nextPow2TexCoord(original.y(), dimension.h()));
}


bool MathUtil::isRectangleOverlap(const Rectf& a, const Rectf& b)
{
  const float ax2 = a.x() + a.w();
  const float ay2 = a.y() + a.h();
  const float bx2 = b.x() + b.w();
  const float by2 = b.y() + b.h();

  return (a.x() < bx2   &&
          ax2   > b.x() &&
          a.y() < by2   &&
          ay2   > b.y());
}
