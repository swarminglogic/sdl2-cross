#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/algorithm/clamp.hpp>

#include <unordered_set>
#include <math/MathUtil.h>
#include <math/Random.h>
#include <util/File.h>
#include <util/StringUtil.h>
#include <util/WordGenerator.h>
#include <util/WordGeneratorFactory.h>
#include <util/WordGeneratorInterface.h>

int main(int argc, char **argv)
{
  if (argc != 5) {
    std::cerr << "Invalid parameters." << std::endl
              << "Usage: "
              << argv[0] << " WORDFILE N N_init #GEN" << std::endl;
    std::cerr << "" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::string> args(argv, argv + argc);
  File file(args[1], FileInfo::TYPE_UNSPECIFIED);
  if (!file.exists()) {
    std::cerr << "Error: File " << file.getFilename()
              << " was not found." << std::endl;
    return EXIT_FAILURE;
  }

  assert(args.size() == 5);
  int N = 0;
  int N_init = 0;
  int wordToGen = 0;
  try {
    N = boost::algorithm::clamp(static_cast<int>(std::atol(argv[2])),
                                1, WordGeneratorInterface::MAX_ORDER);
    N_init = static_cast<int>(std::atol(argv[3]));
    wordToGen = static_cast<int>(std::atol(argv[4]));
  } catch (...) {
    std::cerr << "Bad argument?" << std::endl;
  }


  // Main program
  Random::init();
  auto wg = WordGeneratorFactory::create(N, N_init);
  wg->addInputWords(file.read());
  wg->prepare();

  std::cerr << "Debug: N      = " << wg->getN() << std::endl;
  std::cerr << "Debug: N_init = " << wg->getN_init() << std::endl;
  std::cerr << "Debug: #Gen   = " << wordToGen << std::endl;
  std::cerr << "----------------------------------------" << std::endl;

  int tries = 0;
  int count = 0;

  std::unordered_set<std::string> newWords;
  while (count < wordToGen && tries < wordToGen * 1) {
    ++tries;
    std::string word = wg->generate();
    auto localListIt = newWords.find(word);
    if (!wg->isInputWord(word) && (localListIt == newWords.end())) {
      newWords.insert(word);
      word[0] = static_cast<char>(std::toupper(word[0]));
      std::cout << word << std::endl;
      ++count;
    }
  }
  return EXIT_SUCCESS;
}
