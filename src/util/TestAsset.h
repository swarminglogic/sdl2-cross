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
    AssetImage imageb;
    TS_ASSERT_EQUALS(imageb.path(), "./assets/images/");
    TS_ASSERT_EQUALS(imageb.filename(), "");
    TS_ASSERT_EQUALS(imageb.type(), Asset::IMAGE);


    AssetImage image("test.png");
    TS_ASSERT_EQUALS(image.path(), "./assets/images/test.png");
    TS_ASSERT_EQUALS(image.filename(), "test.png");
    TS_ASSERT_EQUALS(image.type(), Asset::IMAGE);
    AssetImage imageCp = image;
    TS_ASSERT_EQUALS(imageCp.path(), "./assets/images/test.png");
    TS_ASSERT_EQUALS(imageCp.filename(), "test.png");
    TS_ASSERT_EQUALS(imageCp.type(), Asset::IMAGE);
    TS_ASSERT_EQUALS(image, imageCp);
  }

  void testAssetFont()
  {
    AssetFont fontb;
    TS_ASSERT_EQUALS(fontb.path(), "./assets/fonts/");
    TS_ASSERT_EQUALS(fontb.filename(), "");
    TS_ASSERT_EQUALS(fontb.type(), Asset::FONT);

    AssetFont font("test.ttf");
    TS_ASSERT_EQUALS(font.path(), "./assets/fonts/test.ttf");
    TS_ASSERT_EQUALS(font.filename(), "test.ttf");
    TS_ASSERT_EQUALS(font.type(), Asset::FONT);

    AssetFont fontCp = font;
    TS_ASSERT_EQUALS(fontCp.path(), "./assets/fonts/test.ttf");
    TS_ASSERT_EQUALS(fontCp.filename(), "test.ttf");
    TS_ASSERT_EQUALS(fontCp.type(), Asset::FONT);
    TS_ASSERT_EQUALS(font, fontCp);
  }

  void testAssetMesh()
  {
    AssetMesh meshb;
    TS_ASSERT_EQUALS(meshb.path(), "./assets/meshes/");
    TS_ASSERT_EQUALS(meshb.filename(), "");
    TS_ASSERT_EQUALS(meshb.type(), Asset::MESH);

    AssetMesh mesh("test.cobj");
    TS_ASSERT_EQUALS(mesh.path(), "./assets/meshes/test.cobj");
    TS_ASSERT_EQUALS(mesh.filename(), "test.cobj");
    TS_ASSERT_EQUALS(mesh.type(), Asset::MESH);

    AssetMesh meshCp = mesh;
    TS_ASSERT_EQUALS(meshCp.path(), "./assets/meshes/test.cobj");
    TS_ASSERT_EQUALS(meshCp.filename(), "test.cobj");
    TS_ASSERT_EQUALS(meshCp.type(), Asset::MESH);
    TS_ASSERT_EQUALS(mesh, meshCp);
  }

  void testAssetShader()
  {
    AssetShader shaderb;
    TS_ASSERT_EQUALS(shaderb.path(), "./assets/shaders/");
    TS_ASSERT_EQUALS(shaderb.filename(), "");
    TS_ASSERT_EQUALS(shaderb.type(), Asset::SHADER);

    AssetShader shader("test.frag");
    TS_ASSERT_EQUALS(shader.path(), "./assets/shaders/test.frag");
    TS_ASSERT_EQUALS(shader.filename(), "test.frag");
    TS_ASSERT_EQUALS(shader.type(), Asset::SHADER);

    AssetShader shaderCp = shader;
    TS_ASSERT_EQUALS(shaderCp.path(), "./assets/shaders/test.frag");
    TS_ASSERT_EQUALS(shaderCp.filename(), "test.frag");
    TS_ASSERT_EQUALS(shaderCp.type(), Asset::SHADER);
    TS_ASSERT_EQUALS(shader, shaderCp);
  }

  void testAssetSound()
  {
    AssetSound soundb;
    TS_ASSERT_EQUALS(soundb.path(), "./assets/sounds/");
    TS_ASSERT_EQUALS(soundb.filename(), "");
    TS_ASSERT_EQUALS(soundb.type(), Asset::SOUND);

    AssetSound sound("test.flac");
    TS_ASSERT_EQUALS(sound.path(), "./assets/sounds/test.flac");
    TS_ASSERT_EQUALS(sound.filename(), "test.flac");
    TS_ASSERT_EQUALS(sound.type(), Asset::SOUND);

    AssetSound soundCp = sound;
    TS_ASSERT_EQUALS(soundCp.path(), "./assets/sounds/test.flac");
    TS_ASSERT_EQUALS(soundCp.filename(), "test.flac");
    TS_ASSERT_EQUALS(soundCp.type(), Asset::SOUND);
    TS_ASSERT_EQUALS(sound, soundCp);
  }

  void testAssetMusic()
  {
    AssetMusic musicb;
    TS_ASSERT_EQUALS(musicb.path(), "./assets/music/");
    TS_ASSERT_EQUALS(musicb.filename(), "");
    TS_ASSERT_EQUALS(musicb.type(), Asset::MUSIC);

    AssetMusic music("test.flac");
    TS_ASSERT_EQUALS(music.path(), "./assets/music/test.flac");
    TS_ASSERT_EQUALS(music.filename(), "test.flac");
    TS_ASSERT_EQUALS(music.type(), Asset::MUSIC);

    AssetMusic musicCp = music;
    TS_ASSERT_EQUALS(musicCp.path(), "./assets/music/test.flac");
    TS_ASSERT_EQUALS(musicCp.filename(), "test.flac");
    TS_ASSERT_EQUALS(musicCp.type(), Asset::MUSIC);
    TS_ASSERT_EQUALS(music, musicCp);
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

    AssetShader shaderA("test1.frag");
    AssetShader shaderB("test2.frag");
    TS_ASSERT(shaderA < shaderB);
    TS_ASSERT(!(shaderB < shaderA));

    AssetFont   asset3;  // font
    AssetImage  asset2;  // image
    AssetMesh   asset4;  // mesh
    AssetMusic  asset6;  // music
    AssetShader asset1;  // shader
    AssetSound  asset5;  // sounds
    TS_ASSERT(asset3 < asset2);
    TS_ASSERT(asset2 < asset4);
    TS_ASSERT(asset4 < asset6);
    TS_ASSERT(asset6 < asset1);
    TS_ASSERT(asset1 < asset5);

    TS_ASSERT(!(asset2 < asset3));
    TS_ASSERT(!(asset4 < asset2));
    TS_ASSERT(!(asset6 < asset4));
    TS_ASSERT(!(asset1 < asset6));
    TS_ASSERT(!(asset5 < asset1));
  }

  void testEmptyString() {
    AssetImage img;
    AssetFont font;
    AssetMesh mesh;
    AssetShader shader;
    AssetSound sound;
    AssetMusic music;
    TS_ASSERT_EQUALS(img.filename(), "");
    TS_ASSERT_EQUALS(img.path(), "./assets/images/");

    TS_ASSERT_EQUALS(font.filename(), "");
    TS_ASSERT_EQUALS(font.path(), "./assets/fonts/");

    TS_ASSERT_EQUALS(mesh.filename(), "");
    TS_ASSERT_EQUALS(mesh.path(), "./assets/meshes/");

    TS_ASSERT_EQUALS(shader.filename(), "");
    TS_ASSERT_EQUALS(shader.path(), "./assets/shaders/");

    TS_ASSERT_EQUALS(sound.filename(), "");
    TS_ASSERT_EQUALS(sound.path(), "./assets/sounds/");

    TS_ASSERT_EQUALS(music.filename(), "");
    TS_ASSERT_EQUALS(music.path(), "./assets/music/");
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

#endif  // UTIL_TESTASSET_H
