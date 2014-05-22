#include <util/TextBoxText.h>

#include <cassert>
#include <cmath>

#include <util/StringUtil.h>


TextBoxText::TextBoxText()
  : source_(),
    boxWidth_(0),
    height_(12u),
    width_{5u},
    formatted_()
{
}


TextBoxText::~TextBoxText()
{
}


void TextBoxText::setHeight(uint8_t height)
{height_ = height;}
void TextBoxText::setWidthFixed(uint8_t width)
{width_.fill(width);}
void TextBoxText::setWidth(const std::array<uint8_t, 256>& widths)
{width_ = widths;}

uint8_t TextBoxText::getHeight() const
{
  return height_;
}

const std::array<uint8_t, 256>& TextBoxText::getWidths() const
{
  return width_;
}




void TextBoxText::setBoxWidth(unsigned int boxWidth)
{boxWidth_ = boxWidth;}

void TextBoxText::setText(const std::string& text)
{
  source_ = text;
  buildFormatted();
}


int TextBoxText::getFormattedHeight() const
{
  return static_cast<int>(formatted_.size()) * height_;
}


std::vector<std::string> TextBoxText::getFormatted(float partial) const
{
  assert(partial >= 0.0f);
  if (partial >= 1.0f)
    return formatted_;

  long unsigned int totalCharacters = 0;
  for (const auto& line : formatted_)
    totalCharacters += line.size();

  long unsigned int charsToPrint =
      static_cast<long unsigned int>(
          std::round(partial * static_cast<float>(totalCharacters)));

  std::vector<std::string> partialText;
  if (charsToPrint == 0u) {
    partialText.push_back("");
    return partialText;
  }

  for (const auto& line : formatted_) {
    if (charsToPrint >= line.size()) {
      // whole line fits
      partialText.push_back(line);
      charsToPrint -= line.size();
    } else {
      if (charsToPrint > 0)
        partialText.push_back(line.substr(0, charsToPrint));
      break;
    }
  }

  return partialText;
}


const std::vector<std::string>& TextBoxText::getFormatted() const
{
  return formatted_;
}

size_t TextBoxText::getNCharacters() const
{
  return source_.size();
}


void TextBoxText::buildFormatted()
{
  typedef std::vector<std::string> StringVector;

  // Preformatting manual linebreaks.
  formatted_.clear();

  StringVector lines;
  StringUtil::split(source_, lines, '\n');
  for (auto& line : lines) {
    StringUtil::rtrim(line);
  }

  // Infinite box width, never create new brakes
  if (boxWidth_ == 0) {
    formatted_ = lines;
    return;
  }

  // Checking each line against width
  for (const auto& line : lines) {
    std::string fline(""); // formatted line
    uint flineWidth = 0;

    StringVector words = StringUtil::split(line, ' ');
    for (const auto& word : words) {
      const uint wordWidth = textWidth(word);
      // word fits in current line
      if (flineWidth + wordWidth <= boxWidth_) {
        fline      += word + " ";
        flineWidth += wordWidth + width_[' '];
      } else {
        // Push current line
        StringUtil::rtrim(fline);
        formatted_.push_back(fline);

        // Store left over word
        fline      = word + " ";
        flineWidth = wordWidth + width_[' '];
        // TODO swarminglogic, 2013-09-27: add check for hyphenation.
      }
    }
    StringUtil::rtrim(fline);
    formatted_.push_back(fline);
  }
}


unsigned int TextBoxText::textWidth(const std::string& text) const
{
  unsigned int width = 0;
  for (size_t i = 0 ; i < text.size() ; ++i) {
    width += width_[text[i]];
  }
  return width;
}
