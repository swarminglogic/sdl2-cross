#include <util/CharMap.h>

#include <algorithm>

#include <math/MathUtil.h>
#include <util/assert.h>


CharMap::CharMap()
  : CharMap(Size(0, 0))
{}

CharMap::CharMap(const Size& charSize)
  : defaultTraitIndex_(0),
    traits_(),
    charSize_(charSize),
    tBoxSize_(20 * charSize.w(), 5 * charSize.h())
{}
CharMap::~CharMap()
{}


Point CharMap::getCharOffset(Trait trait, const char c)
{
  int index = getTraitIndex(trait);
  if (index < 0) index = defaultTraitIndex_;

  const Point tBoxIndex = convertIndexToTBoxPosition(index);
  const Point charIndex = convertCharToTBoxIndex(c);

  return Point(tBoxIndex.x() * tBoxSize_.w() + charIndex.x() * charSize_.w(),
               tBoxIndex.y() * tBoxSize_.h() + charIndex.y() * charSize_.h());
}

Point CharMap::getCharIndexOffset(Trait trait, const char c)
{
  const Point charOffset = getCharOffset(trait, c);
  return Point(charOffset.x() / charSize_.w(), charOffset.y() / charSize_.h());
}



std::vector<CharMap::Trait> CharMap::getTraits() const
{return traits_;}

void CharMap::addTrait(Trait trait)
{traits_.push_back(trait);}

void CharMap::setTraits(const std::vector<Trait>& traits)
{traits_ = traits;}


void CharMap::setDefaultTrait(Trait trait)
{
  const int traitIndex = getTraitIndex(trait);
  assert(traitIndex >= 0 && "Invalid trait cannot be set as default.");
  defaultTraitIndex_ = traitIndex;
}

std::size_t CharMap::getNTraits() const
{
  return traits_.size();
}

Size CharMap::getCharSize() const
{
  return charSize_;
}

void CharMap::setCharSize(const Size& charSize)
{
  charSize_ = charSize;
  tBoxSize_ = Size(20 * charSize.w(), 5 * charSize.h());
}

Point CharMap::convertIndexToTBoxPosition(int index)
{
  if (index == 0)
    return Point(0, 0);

  const int base = static_cast<int>(MathUtil::prevSquareBase(index));
  if (index + 1 == (base + 1) * (base + 1))
    return Point(base, base);

  int d = index - base * base;
  int x = std::min(d, base);
  int y = 0;
  if (x == base)
    y = d - base;
  else
    y = base;

  return Point(x, y);
}

Point CharMap::convertCharToTBoxIndex(char c)
{
  const int i = c - ' ';
  if ( i < 0 || i > 99 )
    return Point(0, 0);
  else
    return Point(i % 20, i / 20);
}


// Privates
int CharMap::getTraitIndex(Trait trait) const
{
  int index =
    static_cast<int>(std::find(traits_.begin(),
                               traits_.end(), trait) - traits_.begin());

  if (index < static_cast<int>(traits_.size()))
    return index;
  else
    return -1;
}
