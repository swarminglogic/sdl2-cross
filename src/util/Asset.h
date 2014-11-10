#ifndef UTIL_ASSET_H
#define UTIL_ASSET_H

#include <string>


/**
 * Asset class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Asset
{
 public:
  enum Type{
    SHADER,
    IMAGE,
    FONT,
    MESH,
    SOUND,
    MUSIC,
    INVALID_TYPE
  };

  Asset();

  virtual ~Asset();

  Asset::Type type() const;

  /**
   * Full path from base resource
   * e.g. ./assets/shaders/deferredquad.frag
   */
  std::string path() const;

  /**
   * Only filename
   * e.g. deferredquad.frag
   */
  std::string filename() const;


  friend inline bool operator<(const Asset& lhs, const Asset& rhs);
  friend inline bool operator==(const Asset& lhs, const Asset& rhs);
  friend inline bool operator!=(const Asset& lhs, const Asset& rhs);


 protected:
  Asset::Type type_;
  std::string subfolder_;
  std::string filename_;

 private:
  static const std::string base_;

  // TODO swarminglogic, 2013-11-05: Remove
  static std::string typeToPath(Asset::Type type);
};



inline bool operator<(const Asset& lhs, const Asset& rhs) {
  return lhs.path() < rhs.path();
}


inline bool operator==(const Asset& lhs, const Asset& rhs) {
  return ((lhs.type_ == rhs.type_) &&
          (lhs.filename_ == rhs.filename_));
}
inline bool operator!=(const Asset& lhs, const Asset& rhs) {
  return !(lhs == rhs);
}



class AssetShader : public Asset
{
 public:
  AssetShader();
  explicit AssetShader(const std::string& resource);
  virtual ~AssetShader();
};

class AssetImage : public Asset
{
 public:
  AssetImage();
  explicit AssetImage(const std::string& resource);
  virtual ~AssetImage();
};

class AssetFont : public Asset
{
 public:
  AssetFont();
  explicit AssetFont(const std::string& resource);
  virtual ~AssetFont();
};

class AssetMesh : public Asset
{
 public:
  AssetMesh();
  explicit AssetMesh(const std::string& resource);
  virtual ~AssetMesh();
};


class AssetSound : public Asset
{
 public:
  AssetSound();
  explicit AssetSound(const std::string& resource);
  virtual ~AssetSound();
};



class AssetMusic : public Asset
{
 public:
  AssetMusic();
  explicit AssetMusic(const std::string& resource);
  virtual ~AssetMusic();
};




#endif  // UTIL_ASSET_H
