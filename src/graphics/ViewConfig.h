#ifndef GRAPHICS_VIEWCONFIG_H
#define GRAPHICS_VIEWCONFIG_H

#include <string>

#include <math/Size.h>


/**
 * ViewConfig class. Represents view-specific settings.
 *
 * @author SwarmingLogic
 */
class ViewConfig
{
public:
  ViewConfig();
  ViewConfig(Size screenSize,
             const std::string& windowTitle);

  virtual ~ViewConfig();

  const Size& getScreenSize() const;
  void setScreenSize(Size screenSize);
  int getScreenWidth() const;
  int getScreenHeight() const;

  const std::string& getWindowTitle() const;
  void setWindowTitle(const std::string& windowTitle);

  bool isFullScreen() const;
  void setIsFullScreen(bool isFullScreenSet);

  bool isResizeable() const;
  void setIsResizeable(bool isResizeable);

  bool isVSync() const;
  void setIsVSync(bool isVSync);

private:
  Size screenSize_;
  std::string windowTitle_;
  bool isFullScreen_;
  bool isResizeable_;
  bool isVSync_;
};

#endif
