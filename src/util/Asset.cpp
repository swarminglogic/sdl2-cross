#include <util/Asset.h>

Asset::Asset()
  : type_(INVALID_TYPE),
    subfolder_(""),
    filename_("")
{
}

Asset::~Asset()
{
}


Asset::Type Asset::type() const
{
  return type_;
}


std::string Asset::path() const
{
  return subfolder_ + filename_;
}

std::string Asset::filename() const
{
  return filename_;
}


// Shader Asset
AssetShader::AssetShader() : AssetShader("") {}
AssetShader::AssetShader(const std::string& resource)
{
  type_ = SHADER;
  subfolder_ = std::string("shaders/");
  filename_  = resource;
}
AssetShader::~AssetShader(){}


// Image Asset
AssetImage::AssetImage() : AssetImage("") {}
AssetImage::AssetImage(const std::string& resource)
{
  type_ = IMAGE;
  subfolder_ = std::string("images/");
  filename_  = resource;
}
AssetImage::~AssetImage(){}


// Font Asset
AssetFont::AssetFont() : AssetFont("") {}
AssetFont::AssetFont(const std::string& resource)
{
  type_ = FONT;
  subfolder_ = std::string("fonts/");
  filename_  = resource;
}
AssetFont::~AssetFont(){}


// Mesh Asset
AssetMesh::AssetMesh() : AssetMesh("") {}
AssetMesh::AssetMesh(const std::string& resource)
{
  type_ = MESH;
  subfolder_ = std::string("meshes/");
  filename_  = resource;
}
AssetMesh::~AssetMesh(){}



// Sound Asset
AssetSound::AssetSound() : AssetSound("") {}
AssetSound::AssetSound(const std::string& resource)
{
  type_ = SOUND;
  subfolder_ = std::string("sounds/");
  filename_  = resource;
}
AssetSound::~AssetSound(){}


// Music Asset
AssetMusic::AssetMusic() : AssetMusic("") {}
AssetMusic::AssetMusic(const std::string& resource)
{
  type_ = MUSIC;
  subfolder_ = std::string("music/");
  filename_  = resource;
}
AssetMusic::~AssetMusic(){}
