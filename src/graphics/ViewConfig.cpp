#include <graphics/ViewConfig.h>


ViewConfig::ViewConfig()
  : ViewConfig(Size(800,600), "")
{
}

ViewConfig::ViewConfig(Size screenSize,
                       const std::string& windowTitle)
  : screenSize_(screenSize),
    windowTitle_(windowTitle),
    isFullScreen_(false),
    isResizeable_(false),
    isVSync_(true)
{
}

ViewConfig::~ViewConfig()
{
}

const Size& ViewConfig::getScreenSize() const
{
  return screenSize_;
}

void ViewConfig::setScreenSize(Size screenSize)
{
  screenSize_ = screenSize;
}

int ViewConfig::getScreenWidth() const
{
  return screenSize_.width();
}

int ViewConfig::getScreenHeight() const
{
  return screenSize_.height();
}


const std::string& ViewConfig::getWindowTitle() const
{
  return windowTitle_;
}

void ViewConfig::setWindowTitle(const std::string& windowTitle)
{
  windowTitle_ = windowTitle;
}

bool ViewConfig::isFullScreen() const
{
  return isFullScreen_;
}

void ViewConfig::setIsFullScreen(bool isFullScreenSet)
{
  isFullScreen_ = isFullScreenSet;
}

bool ViewConfig::isResizeable() const
 {
   return isResizeable_;
 }

void ViewConfig::setIsResizeable(bool isResizeableEnabled)
{
  isResizeable_ = isResizeableEnabled;
}

bool ViewConfig::isVSync() const
{
  return isVSync_;
}

void ViewConfig::setIsVSync(bool isVSyncEnabled)
{
  isVSync_ = isVSyncEnabled;
}
