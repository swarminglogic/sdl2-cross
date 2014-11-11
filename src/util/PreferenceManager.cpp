#include <util/PreferenceManager.h>

#include <util/Assert.h>
#include <util/FileInfo.h>
#include <util/FileUtil.h>


PreferenceManager::PreferenceManager(const FileInfo& defaultPrefFileInfo,
                                     const FileInfo& systemPrefFileInfo,
                                     const FileInfo& userPrefFileInfo)
    : log_("PreferenceManager"),
      defaultPrefFileInfo_(defaultPrefFileInfo),
      defaultPref_(defaultPrefFileInfo_),
      systemPrefFileInfo_(systemPrefFileInfo),
      systemPref_(systemPrefFileInfo_),
      userPrefFileInfo_(userPrefFileInfo),
      userPref_(userPrefFileInfo_)
{
  assert(FileUtil::exists(defaultPrefFileInfo_));
  assert(FileUtil::exists(systemPrefFileInfo_));
  if (!FileUtil::exists(userPrefFileInfo_))
    FileUtil::write(userPrefFileInfo_, "; Generated file - version 1.0");

  // Load all preference files
  defaultPref_.load();
  systemPref_.load();
  userPref_.load();
}


PreferenceManager::~PreferenceManager()
{
}


bool PreferenceManager::hasEntry(const std::string& key) const
{
  return (userPref_.hasEntry(key) ||
          systemPref_.hasEntry(key) ||
          defaultPref_.hasEntry(key));
}


void PreferenceManager::load()
{
  userPref_.load();
}


bool PreferenceManager::save()
{
  return userPref_.save();
}
