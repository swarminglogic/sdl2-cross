#ifndef GRAPHICS_IMAGERESOURCEMANAGER_H
#define GRAPHICS_IMAGERESOURCEMANAGER_H

#include <iosfwd>
#include <map>
#include <memory>

#include <unordered_map>
#include <util/Asset.h>
#include <util/Log.h>


class Surface;


/**
 * ImageResourceManager class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ImageResourceManager
{
public:
  typedef std::shared_ptr<Surface> SurfaceShPtr;

  ImageResourceManager();
  virtual ~ImageResourceManager();

  /**
   * Returns shared pointer to Surface with the image loaded.
   * If the image isn't found, it returns a nullptr.
   */
  SurfaceShPtr load(const AssetImage& image);
  SurfaceShPtr loadImage(const std::string&  imagefile);

private:
  Log log_;
  std::map<AssetImage, SurfaceShPtr> loadedImages_;

  // NonCopyable
  ImageResourceManager(const ImageResourceManager& c);
  ImageResourceManager& operator=(const ImageResourceManager& c);
};

#endif
