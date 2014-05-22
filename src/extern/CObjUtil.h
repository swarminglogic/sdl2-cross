#ifndef EXTERN_COBJUTIL_H
#define EXTERN_COBJUTIL_H

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include <extern/tiny_obj_loader.h>


//
// TODO swarminglogic, 2013-10-08: Add error detection.
//

/**
 * CObjUtil class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class CObjUtil
{
public:
  static void write(std::ostream& stream,
                    const std::vector<tinyobj::shape_t>& shapes);

  static std::vector<tinyobj::shape_t> read(std::istream& stream);

private:
  CObjUtil();
  virtual ~CObjUtil();
  // NonCopyable
  CObjUtil(const CObjUtil& c);
  CObjUtil& operator=(const CObjUtil& c);
};

#endif
