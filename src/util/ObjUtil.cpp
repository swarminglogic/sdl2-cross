#include <util/ObjUtil.h>

#include <sstream>

#include <util/CObjUtil.h>
#include <util/FileUtil.h>
#include <util/StringUtil.h>
#include <util/Assert.h>


std::vector<tinyobj::shape_t> ObjUtil::read(AssetMesh meshfile)
{
  // check if file is compressed, and if so, use ObjUtil::readCompressedObj()
  bool isCompressed = (StringUtil::suffix(meshfile.path(), 5) == ".cobj");
  if (isCompressed) {
    std::string data = FileUtil::read(meshfile.path());
    std::stringstream ss(data);
    return CObjUtil::readCompressedObj(ss);
  } else {
    assert(false && "Unsupported ATM");
    std::vector<tinyobj::shape_t> tempShapes;  // LCOV_EXCL_LINE
    return tempShapes;  // LCOV_EXCL_LINE
  }
}
