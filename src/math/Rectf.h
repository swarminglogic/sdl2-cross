#ifndef MATH_RECTF_H
#define MATH_RECTF_H

#include <math/Pointf.h>
#include <math/Sizef.h>


/**
 * Rectf class, represents a rectangle with position and dimension.
 *
 * @note No virtual functions to avoid extra pointer to vftable.
 *       Don't inherit from this class.
 * @author SwarmingLogic (Roald Fernandez)
 */
class Rectf
{
 public:
  Rectf(const Pointf& position, const Sizef& size);
  explicit Rectf(const Sizef& size);
  Rectf(float x, float y, float w, float h);
  ~Rectf();

  float x() const;
  void x(float x);
  float y() const;
  void y(float y);
  float w() const;
  void w(float w);
  float h() const;
  void h(float h);

  void setPosition(const Pointf& position);
  Pointf getPosition() const;
  void setSize(const Sizef& size);
  Sizef getSize() const;


  friend inline bool operator==(const Rectf& lhs, const Rectf& rhs);
  friend inline bool operator!=(const Rectf& lhs, const Rectf& rhs);

  /**
   * Returns pointer to data, which is a 4-sized int array, corresponding to
   * pos-x pos-y size-w size-h
   *
   * This is for used for giving direct access to members (e.g. for use
   * w/OpenGL).
   **/
  const float* getData() const;
  float* getData();

 private:
  Pointf pos_;
  Sizef size_;
};

inline bool operator==(const Rectf& lhs, const Rectf& rhs)
{return (lhs.pos_ == rhs.pos_) && (lhs.size_ == rhs.size_);}
inline bool operator!=(const Rectf& lhs, const Rectf& rhs)
{return !(lhs == rhs);}

#endif  // MATH_RECTF_H
