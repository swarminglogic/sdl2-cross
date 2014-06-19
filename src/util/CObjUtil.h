#ifndef UTIL_COBJUTIL_H
#define UTIL_COBJUTIL_H

#include <fstream>

#include <extern/tiny_obj_loader.h>


/**
 * CObjUtil class.
 *
 * @author SwarmingLogic
 */
class CObjUtil
{
public:
  static void writeCompressedObj(std::ostream& stream,
                                 const std::vector<tinyobj::shape_t>& shapes);
  static std::vector<tinyobj::shape_t> readCompressedObj(std::istream& stream);

private:
  CObjUtil();
  virtual ~CObjUtil();

  // NonCopyable
  CObjUtil(const CObjUtil& c);
  CObjUtil& operator=(const CObjUtil& c);
};

#endif
