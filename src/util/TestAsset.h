#ifndef UTIL_TESTASSET_H
#define UTIL_TESTASSET_H

#include <memory>

#include <util/Asset.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Asset class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestAsset : public CxxTest::TestSuite
{
public:

  void testBasePath()
  {
    TS_ASSERT_EQUALS(Asset::getBasePath(), "./assets/");
  }

  void testAssetImage()
  {
    AssetImage image("test.png");
    TS_ASSERT_EQUALS(image.path(), "./assets/images/test.png");
    TS_ASSERT_EQUALS(image.filename(), "test.png");
    TS_ASSERT_EQUALS(image.type(), Asset::IMAGE);
  }

  void testAssetFont()
  {
    AssetFont font("test.ttf");
    TS_ASSERT_EQUALS(font.path(), "./assets/fonts/test.ttf");
    TS_ASSERT_EQUALS(font.filename(), "test.ttf");
    TS_ASSERT_EQUALS(font.type(), Asset::FONT);
  }

  void testAssetMesh()
  {
    AssetMesh mesh("test.cobj");
    TS_ASSERT_EQUALS(mesh.path(), "./assets/meshes/test.cobj");
    TS_ASSERT_EQUALS(mesh.filename(), "test.cobj");
    TS_ASSERT_EQUALS(mesh.type(), Asset::MESH);
  }

  void testAssetShader()
  {
    AssetShader shader("test.frag");
    TS_ASSERT_EQUALS(shader.path(), "./assets/shaders/test.frag");
    TS_ASSERT_EQUALS(shader.filename(), "test.frag");
    TS_ASSERT_EQUALS(shader.type(), Asset::SHADER);
  }

  void testAssetSound()
  {
    AssetSound shader("test.flac");
    TS_ASSERT_EQUALS(shader.path(), "./assets/sounds/test.flac");
    TS_ASSERT_EQUALS(shader.filename(), "test.flac");
    TS_ASSERT_EQUALS(shader.type(), Asset::SOUND);
  }

  void testAssetMusic()
  {
    AssetMusic shader("test.flac");
    TS_ASSERT_EQUALS(shader.path(), "./assets/music/test.flac");
    TS_ASSERT_EQUALS(shader.filename(), "test.flac");
    TS_ASSERT_EQUALS(shader.type(), Asset::MUSIC);
  }

  void testVirtual() {
    std::unique_ptr<Asset> asset(new AssetShader("test.frag"));
    TS_ASSERT_EQUALS(asset->path(), "./assets/shaders/test.frag");
    TS_ASSERT_EQUALS(asset->filename(), "test.frag");
    TS_ASSERT_EQUALS(asset->type(), Asset::SHADER);

    asset.reset(new AssetImage("image.png"));
    TS_ASSERT_EQUALS(asset->path(), "./assets/images/image.png");
    TS_ASSERT_EQUALS(asset->filename(), "image.png");
    TS_ASSERT_EQUALS(asset->type(), Asset::IMAGE);
  }

  void testOperators() {
    // Comparison
    AssetShader shader("test.frag");
    AssetShader shaderSame("test.frag");
    TS_ASSERT(shader == shaderSame);
    TS_ASSERT_EQUALS(shader, shaderSame);
    AssetShader shaderDifferentPath("other.frag");
    TS_ASSERT_DIFFERS(shader, shaderDifferentPath);
    AssetImage shaderDifferentType("test.frag");
    TS_ASSERT_DIFFERS(shader, shaderDifferentType);
  }

  void testEmptyString() {
    AssetImage img;
    AssetFont font;
    AssetMesh mesh;
    AssetShader shader;
    TS_ASSERT_EQUALS(img.filename(), "");
    TS_ASSERT_EQUALS(img.path(), "./assets/images/");

    TS_ASSERT_EQUALS(font.filename(), "");
    TS_ASSERT_EQUALS(font.path(), "./assets/fonts/");

    TS_ASSERT_EQUALS(mesh.filename(), "");
    TS_ASSERT_EQUALS(mesh.path(), "./assets/meshes/");

    TS_ASSERT_EQUALS(shader.filename(), "");
    TS_ASSERT_EQUALS(shader.path(), "./assets/shaders/");
  }

  void testCopy() {
    AssetShader shader("test.frag");
    AssetShader shadercp = shader;
    TS_ASSERT_EQUALS(shader, shadercp);

    AssetShader shadercp2(shader);
    TS_ASSERT_EQUALS(shader, shadercp2);
  }

private:
};

#endif
