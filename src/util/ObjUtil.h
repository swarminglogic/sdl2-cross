#ifndef UTIL_OBJUTIL_H
#define UTIL_OBJUTIL_H

#include <vector>

#include <extern/tiny_obj_loader.h>
#include <util/Asset.h>


//
// TODO swarminglogic, 2013-10-08: Add error detection.
//

/**
 * ObjUtil class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ObjUtil
{
public:
  static std::vector<tinyobj::shape_t> read(AssetMesh meshfile);


private:
  ObjUtil();
  virtual ~ObjUtil();

  // NonCopyable
  ObjUtil(const ObjUtil& c);
  ObjUtil& operator=(const ObjUtil& c);
};

#endif
