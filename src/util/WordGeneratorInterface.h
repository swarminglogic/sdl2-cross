#ifndef UTIL_WORDGENERATORINTERFACE_H
#define UTIL_WORDGENERATORINTERFACE_H

#include <iosfwd>
#include <memory>
#include <vector>


/**
 * WordGeneratorInterface class.
 *
 * This is the common interface for all classes made from the class
 * templates using this interface, e.g. WordGenerator
 *
 * WordGeneratorFactory creates appropriate type from dynamic values.
 *
 * @author SwarmingLogic
 */
class WordGeneratorInterface
{
 public:
  virtual void addInputWords(const std::string& text, char dlim = '\n') = 0;
  virtual void addInputWords(const std::vector<std::string>& words) = 0;
  virtual void prepare() = 0;
  virtual std::string generate() const = 0;
  virtual bool isInputWord(const std::string& word) const = 0;
  virtual int getN() const = 0;
  virtual int getN_init() const = 0;
  virtual ~WordGeneratorInterface(){}

  static const int MAX_ORDER = 6;
  static const char END_VALUE = '#';
};

using WordGeneratorPtr = std::unique_ptr<WordGeneratorInterface>;


#endif  // UTIL_WORDGENERATORINTERFACE_H
