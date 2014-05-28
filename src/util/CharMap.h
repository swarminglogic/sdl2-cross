#ifndef UTIL_CHARMAP_H
#define UTIL_CHARMAP_H

#include <vector>

#include <math/Point.h>
#include <math/Pointf.h>
#include <math/Size.h>


/**
 * CharMap class, for representing texture offset into a character map image.
 * This supports a set of traits (colors, stylizations, etc).
 *
 * It assumes being comprised of sub-images of 20x5 characters each, starting
 * with space ' ', and following ASCII ordering. I.e:
 *
 * <pre>
 * +--------------------+
 * | !"#$%&'()*+,-./0123|
 * |456789:;<=>?@ABCDEFG|
 * |HIJKLMNOPQRSTUVWXYZ[|
 * |\]^_`abcdefghijklmno|
 * |pqrstuvwxyz{|}~     |
 * +--------------------+
 * </pre>
 * This sub-image is referred to as a TBox (t for trait)
 *
 * The ordering of TBoxes, corresponding to the specified enumerated list of
 * traits structured and ordered as following:
 *   [ 0 ] [ 2 ] [ 6 ] [ 12] [ 20]
 *   [ 1 ] [ 3 ] [ 7 ] [ 13] [ 21]
 *   [ 4 ] [ 5 ] [ 8 ] [ 14] [ 22]
 *   [ 9 ] [ 10] [ 11] [ 15] [ 23]
 *   [ 16] [ 17] [ 18] [ 19] [ 24]
 *
 * Currently, only 25 varations pr CharMap are supported. The traitss
 * themselves can be any number traits, given by the Trait enum.
 * // TODO swarminglogic, 2013-10-01: Generalize for any.
 *
 * If a trait is requested that is not contained in the charmap, it will
 * default to 0, or one specified by setDefault().
 *
 * Texture offset is acquired using a trait and char* to look up.
 *
 * @note CharMap is NOT aware of Surface resizing for OpenGL for powers of two.
 *
 * TODO swarminglogic, 2013-10-01: Add trait map, to set up aliases between
 *                                 traits.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class CharMap
{
public:
  enum Trait{
    C_WHITE = 0,
    C_GOLDEN,
    C_RED,
    C_GREEN,
    C_CYAN,
    C_MAGENTA,
    C_BLUE,
    C_GREY,
    C_OLIVE,
    NO_TRAIT
  };

  CharMap();

  /**
   * The constant width/height character size (in pixels).
   * Actual glyph dimensions are not relevant here.
   */
  CharMap(const Size& charSize);
  virtual ~CharMap();

  /**
   * Returns pixel offset to char (for the specified trait)
   */
  Point getCharOffset(Trait trait, const char c);

  /**
   * Returns index offset to char box containing glyph (for the specified trait)
   */
  Point getCharIndexOffset(Trait trait, const char c);

  /**
   * Functions to set up traits.
   */
  std::vector<Trait> getTraits() const;
  void addTrait(Trait trait);
  void setTraits(const std::vector<Trait>& traits);
  void setDefaultTrait(Trait traits);
  std::size_t getNTraits() const;

  /**
   * Size of the rectangles containing each character.
   * These are constant for all characters, as this class is only responsible
   * for providing a lookup into the data, not aware of the glyph dimensions.
   */
  Size getCharSize() const;
  void setCharSize(const Size& charSize);


  // Helper functions, made public for testing, otherwise not very much user for
  // them.
  static Point convertIndexToTBoxPosition(int index);
  static Point convertCharToTBoxIndex(char c);

private:
  int defaultTraitIndex_;
  int getTraitIndex(Trait trait) const;
  std::vector<Trait> traits_;
  Size charSize_;
  Size tBoxSize_;
};

#endif
