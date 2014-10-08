#ifndef MATH_RECT_H
#define MATH_RECT_H

#include <math/Point.h>
#include <math/Size.h>

/**
 * Rect class, represents a rectangle, position and dimension.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Rect
{
 public:
  Rect(const Point& position, const Size& size);
  explicit Rect(const Size& size);
  Rect(int x, int y, int w, int h);
  ~Rect();

  int x() const;
  void x(int x);
  int y() const;
  void y(int y);
  int w() const;
  void w(int w);
  int h() const;
  void h(int h);

  void setPosition(const Point& position);
  Point getPosition() const;
  void setSize(const Size& size);
  Size getSize() const;

  friend inline bool operator==(const Rect& lhs, const Rect& rhs);
  friend inline bool operator!=(const Rect& lhs, const Rect& rhs);

  /**
   * Returns pointer to data, which is a 4-sized int array, corresponding to
   * pos-x pos-y size-w size-h
   *
   * This is for used for giving direct access to members (e.g. for use
   * w/OpenGL).
   **/
  const int* getData() const;
  int* getData();

 private:
  Point pos_;
  Size size_;
};

inline bool operator==(const Rect& lhs, const Rect& rhs)
{return (lhs.pos_ == rhs.pos_) && (lhs.size_ == rhs.size_);}
inline bool operator!=(const Rect& lhs, const Rect& rhs)
{return !(lhs == rhs);}

#endif  // MATH_RECT_H
