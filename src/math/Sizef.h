#ifndef MATH_SIZEF_H
#define MATH_SIZEF_H

#include <cmath>
#include <limits>

class Pointf;

/**
 * Sizef class. Simple class representing an float-pair.
 * Logically represents width and height.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Sizef
{
public:
  Sizef();
  Sizef(float width, float height);
  ~Sizef();

  float getHeight() const;
  void setHeight(float height);
  float height() const;
  void height(float height);
  float h() const;
  void h(float height);

  float getWidth() const;
  void setWidth(float width);
  float width() const;
  void width(float width);
  float w() const;
  void w(float width);

  void normalize();
  float getLength() const;

  /**
   * Swaps width <-> height
   */
  void transpose();

  Sizef& operator+=(const Sizef& rhs);
  Sizef& operator-=(const Sizef& rhs);
  Sizef& operator*=(float rhs);
  Sizef& operator/=(float rhs);

  friend inline const Sizef operator+(const Sizef& lhs, const Sizef& rhs);
  friend inline const Sizef operator-(const Sizef& lhs, const Sizef& rhs);

  // Multiplication/division with float sizef rounds to nearest integer.
  friend inline const Sizef operator*(const Sizef& lhs, float rhs);
  friend inline const Sizef operator*(float lhs, const Sizef &rhs);
  friend inline const Sizef operator/(const Sizef& lhs, float rhs);

  friend inline bool operator==(const Sizef& lhs, const Sizef& rhs);
  friend inline bool operator!=(const Sizef& lhs, const Sizef& rhs);

  /**
   * Returns pointer to data, which is a 2-sized float array.
   * This is for used for giving direct access to members (e.g. for use
   * w/OpenGL).
   **/
  const float* getData() const;
  float* getData();

  explicit operator Pointf() const;

private:
  float d_[2];
};

inline const Sizef operator+(const Sizef& lhs, const Sizef& rhs)
{return Sizef(lhs.d_[0] + rhs.d_[0], lhs.d_[1] + rhs.d_[1]);}

inline const Sizef operator-(const Sizef& lhs, const Sizef& rhs)
{return Sizef(lhs.d_[0] - rhs.d_[0], lhs.d_[1] - rhs.d_[1]);}

inline const Sizef operator*(const Sizef& lhs, float rhs)
{return Sizef(lhs.d_[0] * rhs, lhs.d_[1] * rhs);}

inline const Sizef operator*(float lhs, const Sizef &rhs)
{return Sizef(lhs * rhs.d_[0], lhs * rhs.d_[1]);}

inline const Sizef operator/(const Sizef& lhs, float rhs)
{return Sizef(lhs.d_[0] / rhs, lhs.d_[1] / rhs);}

inline bool operator==(const Sizef& lhs, const Sizef& rhs)
{
  static const float eps = std::numeric_limits<float>::epsilon();
  return ((std::fabs(lhs.d_[0] - rhs.d_[0]) < eps) &&
          (std::fabs(lhs.d_[1] - rhs.d_[1]) < eps));
}

inline bool operator!=(const Sizef& lhs, const Sizef& rhs)
{return !(lhs == rhs);}

#endif
