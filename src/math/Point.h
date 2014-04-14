#ifndef MATH_POINT_H
#define MATH_POINT_H

#include <cmath>



/**
 * Point class. Simple class representing an int-pair.
 * Logically represents x and y.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Point
{
public:
  Point();
  Point(int x, int y);
  ~Point();

  int getY() const;
  void setY(int y);
  int y() const;
  void y(int y);

  int getX() const;
  void setX(int x);
  int x() const;
  void x(int x);

  /**
   * Swaps x <-> y
   */
  void transpose();

  Point& operator+=(const Point& rhs);
  Point& operator-=(const Point& rhs);
  Point& operator*=(double rhs);
  Point& operator/=(double rhs);

  friend inline const Point operator+(const Point& lhs, const Point& rhs);
  friend inline const Point operator-(const Point& lhs, const Point& rhs);

  // Multiplication/division with float point rounds to nearest integer.
  friend inline const Point operator*(const Point& lhs, double rhs);
  friend inline const Point operator*(double lhs, const Point &rhs);
  friend inline const Point operator/(const Point& lhs, double rhs);

  friend inline bool operator==(const Point& lhs, const Point& rhs);
  friend inline bool operator!=(const Point& lhs, const Point& rhs);


  /**
   * Returns pointer to data, which is a 2-sized int array.
   * This is for used for giving direct access to members (e.g. for use
   * w/OpenGL).
   **/
  const int* getData() const;
  int* getData();


private:
  int d_[2];
};

inline const Point operator+(const Point& lhs, const Point& rhs)
{return Point(lhs.d_[0] + rhs.d_[0], lhs.d_[1] + rhs.d_[1]);}

inline const Point operator-(const Point& lhs, const Point& rhs)
{return Point(lhs.d_[0] - rhs.d_[0], lhs.d_[1] - rhs.d_[1]);}

inline const Point operator*(const Point& lhs, double rhs)
{return Point(static_cast<int>(std::round(lhs.d_[0] * rhs)),
              static_cast<int>(std::round(lhs.d_[1] * rhs)));}

inline const Point operator*(double lhs, const Point &rhs)
{return Point(static_cast<int>(std::round(lhs * rhs.d_[0])),
              static_cast<int>(std::round(lhs * rhs.d_[1])));}

inline const Point operator/(const Point& lhs, double rhs)
{return Point(static_cast<int>(std::round(lhs.d_[0] / rhs)),
              static_cast<int>(std::round(lhs.d_[1] / rhs)));}

inline bool operator==(const Point& lhs, const Point& rhs)
{return (lhs.d_[0] == rhs.d_[0]) && (lhs.d_[1] == rhs.d_[1]);}

inline bool operator!=(const Point& lhs, const Point& rhs)
{return (lhs.d_[0] != rhs.d_[0]) || (lhs.d_[1] != rhs.d_[1]);}

#endif
