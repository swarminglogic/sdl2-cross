#ifndef MATH_SIZE_H
#define MATH_SIZE_H

#include <cmath>


/**
 * Size class. Simple class representing an int-pair.
 * Logically represents width and height.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Size
{
public:
  Size();
  Size(int width, int height);
  ~Size();

  // Many overloads, because I'm lazy.
  int getHeight() const;
  void setHeight(int height);
  int height() const;
  void height(int height);
  int h() const;
  void h(int height);

  int getWidth() const;
  void setWidth(int width);
  int width() const;
  void width(int width);
  int w() const;
  void w(int width);


  /**
   * Swaps width <-> height
   */
  void transpose();

  Size& operator+=(const Size& rhs);
  Size& operator-=(const Size& rhs);
  Size& operator*=(double rhs);
  Size& operator/=(double rhs);

  friend inline const Size operator+(const Size& lhs, const Size& rhs);
  friend inline const Size operator-(const Size& lhs, const Size& rhs);

  // Multiplication/division with float point rounds to nearest integer.
  friend inline const Size operator*(const Size& lhs, double rhs);
  friend inline const Size operator*(double lhs, const Size &rhs);
  friend inline const Size operator/(const Size& lhs, double rhs);

  friend inline bool operator==(const Size& lhs, const Size& rhs);
  friend inline bool operator!=(const Size& lhs, const Size& rhs);


  /**
   * Returns pointer to data, which is a 2-sized int array.
   * This is for used for giving direct access to members (e.g. for use
   * w/OpenGL).
   */
  const int* getData() const;
  int* getData();


private:
  int d_[2];
};

inline const Size operator+(const Size& lhs, const Size& rhs)
{return Size(lhs.d_[0] + rhs.d_[0], lhs.d_[1] + rhs.d_[1]);}

inline const Size operator-(const Size& lhs, const Size& rhs)
{return Size(lhs.d_[0] - rhs.d_[0], lhs.d_[1] - rhs.d_[1]);}

inline const Size operator*(const Size& lhs, double rhs)
{return Size(static_cast<int>(std::round(lhs.d_[0] * rhs)),
             static_cast<int>(std::round(lhs.d_[1] * rhs)));}

inline const Size operator*(double lhs, const Size &rhs)
{return Size(static_cast<int>(std::round(lhs * rhs.d_[0])),
             static_cast<int>(std::round(lhs * rhs.d_[1])));}

inline const Size operator/(const Size& lhs, double rhs)
{return Size(static_cast<int>(std::round(lhs.d_[0] / rhs)),
             static_cast<int>(std::round(lhs.d_[1] / rhs)));}

inline bool operator==(const Size& lhs, const Size& rhs)
{return (lhs.d_[0] == rhs.d_[0]) && (lhs.d_[1] == rhs.d_[1]);}

inline bool operator!=(const Size& lhs, const Size& rhs)
{return (lhs.d_[0] != rhs.d_[0]) || (lhs.d_[1] != rhs.d_[1]);}

#endif
