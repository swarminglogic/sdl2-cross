#ifndef UTIL_TEXTBOXTEXT_H
#define UTIL_TEXTBOXTEXT_H

#include <array>
#include <string>
#include <vector>


/**
 * TextBoxText class.
 *
 * For preparing input for image generation.  Creates strings, while bein aware
 * of character width, accounting for \n characters, and otherwise wrapping at
 * specified pixel width. It also hyphenates when word doesn't fit into width.
 *
 * Also supports partial write (value [0..1]), where the remaining text is not
 * written. It is still aware of line breaks.
 *
 * Can also calculate image height (number of lines * line height).
 *
 * For zoom multiplier, just set the width and hight as a multiplier.
 * then enlarge the render, or use the multiplier functionality of ImageRender.
 *
 * @note character width and height is limited to range [0..255] pixels.
 * @note A ttf font can also be analyzed to create the table of character sizes.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TextBoxText
{
public:
  TextBoxText();
  virtual ~TextBoxText();

  // Set character height / width table
  void setHeight(uint8_t height);
  void setWidthFixed(uint8_t width);
  void setWidth(const std::array<uint8_t, 256>& widths);

  uint8_t getHeight() const;
  const std::array<uint8_t, 256>& getWidths() const;

  /**
   * Set box width in font pixels.
   * For unlimited width, set to 0.
   */
  void setBoxWidth(unsigned int boxWidth);

  /**
   * Sets the raw text to format.
   *
   * Uses configurations set prior to this call. To recompute formatting,
   * either set the text again, or call buildFormatted();
   */
  void setText(const std::string& text);

  int getFormattedHeight() const;
  std::vector<std::string> getFormatted(float partial) const;
  const std::vector<std::string>& getFormatted() const;

  size_t getNCharacters() const;

  void buildFormatted();

private:
  unsigned int textWidth(const std::string& text) const;

  std::string source_;
  unsigned int boxWidth_;
  uint8_t height_ = 0;
  // Table with the widths for each ascii character.
  std::array<uint8_t, 256> width_;
  std::vector<std::string> formatted_;
};

#endif
