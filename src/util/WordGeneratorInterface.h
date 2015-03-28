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
  virtual void addInputWords(const std::vector<std::string>& words) = 0;
  virtual void prepare() = 0;
  virtual std::string generate() const = 0;
  virtual bool isInputWord(const std::string& word) const = 0;
  virtual ~WordGeneratorInterface(){}
};

using WordGeneratorPtr = std::unique_ptr<WordGeneratorInterface>;


#endif  // UTIL_WORDGENERATORINTERFACE_H
