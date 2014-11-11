#include <util/PreferenceHelper.h>

#include <util/Assert.h>
#include <util/FileInfo.h>
#include <util/FileUtil.h>


PreferenceHelper::PreferenceHelper(const FileInfo& defaultPrefFileInfo,
                                   const FileInfo& systemPrefFileInfo,
                                   const FileInfo& userPrefFileInfo)
    : log_("PreferenceHelper"),
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


PreferenceHelper::~PreferenceHelper()
{
}


bool PreferenceHelper::hasEntry(const std::string& key) const
{
  return (userPref_.hasEntry(key) ||
          systemPref_.hasEntry(key) ||
          defaultPref_.hasEntry(key));
}


void PreferenceHelper::load()
{
  userPref_.load();
}


bool PreferenceHelper::save()
{
  return userPref_.save();
}
