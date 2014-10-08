#ifndef MATH_POINTF_H
#define MATH_POINTF_H

#include <cmath>
#include <limits>


class Sizef;

/**
 * Pointf class. Simple class representing an float-pair.
 * Logically represents x and y.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Pointf
{
 public:
  Pointf();
  Pointf(float x, float y);
  ~Pointf();

  float getY() const;
  void setY(float y);
  float y() const;
  void y(float y);

  float getX() const;
  void setX(float x);
  float x() const;
  void x(float x);

  float getLength() const;
  void normalize();

  /**
   * Swaps x <-> y
   */
  void transpose();

  Pointf& operator+=(const Pointf& rhs);
  Pointf& operator-=(const Pointf& rhs);
  Pointf& operator*=(float rhs);
  Pointf& operator/=(float rhs);

  friend inline const Pointf operator+(const Pointf& lhs, const Pointf& rhs);
  friend inline const Pointf operator-(const Pointf& lhs, const Pointf& rhs);

  // Multiplication/division with float pointF rounds to nearest integer.
  friend inline const Pointf operator*(const Pointf& lhs, float rhs);
  friend inline const Pointf operator*(float lhs, const Pointf &rhs);
  friend inline const Pointf operator/(const Pointf& lhs, float rhs);

  friend inline bool operator==(const Pointf& lhs, const Pointf& rhs);
  friend inline bool operator!=(const Pointf& lhs, const Pointf& rhs);

  explicit operator Sizef() const;


  /**
   * Returns pointer to data, which is a 2-sized float array.
   * This is for used for giving direct access to members (e.g. for use
   * w/OpenGL).
   **/
  const float* getData() const;
  float* getData();

 private:
  float d_[2];
};

inline const Pointf operator+(const Pointf& lhs, const Pointf& rhs)
{return Pointf(lhs.d_[0] + rhs.d_[0], lhs.d_[1] + rhs.d_[1]);}

inline const Pointf operator-(const Pointf& lhs, const Pointf& rhs)
{return Pointf(lhs.d_[0] - rhs.d_[0], lhs.d_[1] - rhs.d_[1]);}

inline const Pointf operator*(const Pointf& lhs, float rhs)
{return Pointf(lhs.d_[0] * rhs, lhs.d_[1] * rhs);}

inline const Pointf operator*(float lhs, const Pointf &rhs)
{return Pointf(lhs * rhs.d_[0], lhs * rhs.d_[1]);}

inline const Pointf operator/(const Pointf& lhs, float rhs)
{return Pointf(lhs.d_[0] / rhs, lhs.d_[1] / rhs);}

inline bool operator==(const Pointf& lhs, const Pointf& rhs)
{
  static const float eps = std::numeric_limits<float>::epsilon();
  return ((std::fabs(lhs.d_[0] - rhs.d_[0]) < eps) &&
          (std::fabs(lhs.d_[1] - rhs.d_[1]) < eps));
}

inline bool operator!=(const Pointf& lhs, const Pointf& rhs)
{return !(lhs == rhs);}

#endif  // MATH_POINTF_H
