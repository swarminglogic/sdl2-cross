#ifndef MATH_MATHUTIL_H
#define MATH_MATHUTIL_H

#include <cstdint>


class Rectf;
class Pointf;
class Size;

/**
 * MathUtil class.
 * Static functions.
 * @author SwarmingLogic (Roald Fernandez)
 */
class MathUtil
{
 public:
  static float PI;
  static float PIHALF;
  static float PIQUARTER;

  /**
   * Linear interpolation between [min, max].
   * parameter, a, is interpolation factor [0, 1] (clamped).
   */
  static float lerp(float min, float max, float a);

  /**
   * Clamps value to [min, max]
   */
  static float clamp(float value, float min, float max);

  /**
   * Clamps value to [0, 1]
   */
  static float clamp(float value);

  /**
   * Linear map of [0, 1] (asserted) range to [0, 255].
   */
  static uint8_t mapToU8(float value);

  /**
   * Linear map of [-1, 1] (asserted) range to [0, 254]
   */
  static uint8_t mapToU8special(float value);


  /**
   * Calculates the next greater or equal power of two.
   * 3 -> 4
   * 6 -> 8,
   * 9 -> 16, etc
   */
  static unsigned int nextPow2(unsigned int v);

  /**
   * Calculates the next or equal square.
   *  3 -> 4 (2*2)
   *  5 -> 9 (3*3)
   *  9 -> 9 (3*3)
   * 12 -> 16 (4*4)
   * etc..
   */
  static unsigned int nextSquare(unsigned int v);

  /**
   * Same as above, but returns base.
   *  3 -> 2 (2*2)
   *  5 -> 3 (3*3)
   *  9 -> 3 (3*3)
   * 12 -> 4 (4*4)
   */
  static unsigned int nextSquareBase(unsigned int v);

  /**
   * Returns previous or equal square.
   *  3 -> 1 (1*1)
   *  5 -> 4 (2*2)
   *  9 -> 9 (3*3)
   */
  static unsigned int prevSquare(unsigned int v);

  /**
   * Same as above, but returns base.
   *  3 -> 1 (1*1)
   *  5 -> 2 (2*2)
   *  9 -> 3 (3*3)
   */
  static unsigned int prevSquareBase(unsigned int v);

  /**
   * Checks if value is a power of 2.
   */
  static bool isPow2(unsigned int v);

  /**
   * Checks if value is a square.
   */
  static bool isSquare(unsigned int v);

  /**
   * Translates texture coordinates from original image texture coordinates, to
   * translated coordinates, as a result of the image not being a power of two.
   */
  static float nextPow2TexCoord(float original, int dimension);

  static Pointf nextPow2TexCoord(const Pointf& original,
                                 const Size& dimension);

  static bool isRectangleOverlap(const Rectf& a, const Rectf& b);

 private:
  MathUtil();
  virtual ~MathUtil();

  // NonCopyable
  MathUtil(const MathUtil& c);
  MathUtil& operator=(const MathUtil& c);
};

#endif  // MATH_MATHUTIL_H
