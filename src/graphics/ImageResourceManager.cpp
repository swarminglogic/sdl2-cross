#include <graphics/ImageResourceManager.h>

#include <graphics/Surface.h>


ImageResourceManager::ImageResourceManager()
  : log_("ImageResourceManager")
{
}


ImageResourceManager::~ImageResourceManager()
{
}



SurfaceShPtr
ImageResourceManager::load(const AssetImage& image)
{
  log_.d() << "Trying to load image: " << image.path() << Log::end;
  SurfaceShPtr resource = loadedImages_[image];
  if (!resource) {
    resource.reset(new Surface(image));

    // Check that load went ok, and store in table if so
    if (resource->getSurface())
      loadedImages_[image] = resource;
    else
      resource.reset();
  }
  return resource;
}


SurfaceShPtr ImageResourceManager::loadImage(const std::string&  imagefile)
{
  return load(AssetImage(imagefile));
}
