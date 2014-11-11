#ifndef UTIL_PREFERENCEMANAGER_H
#define UTIL_PREFERENCEMANAGER_H

#include <boost/optional.hpp>

#include <util/Exception.h>
#include <util/FileInfo.h>
#include <util/Log.h>
#include <util/PropertyFile.h>


/**
 * PreferenceManager class.
 *
 * @author SwarmingLogic
 */
class PreferenceManager
{
 public:
  /**
   * Manager class to query values stored in configuration files.
   *
   * It checks all three preference files, tries to return the
   * preference in the order of first occurence:
   *
   * Default -> System -> User -> (Exception Thrown)
   *
   * Default preference file must exist (asserted),
   * System preference file must exist (asserted)
   * If user prefences file doesn't exist, it creates an empty file.
   * User preference file must be of type TYPE_WRITABLE (asserted)
   *
   * Both default and system preferences are assumed to not
   * change during runtime. For this reason, load() only loads
   * the user preference.
   *
   * @param systemPrefences The system preference file.
   * @param userPrefences The user preference file.
   */
  PreferenceManager(const FileInfo& defaultPrefFileInfo,
                    const FileInfo& systemPrefFileInfo,
                    const FileInfo& userPrefFileInfo);

  virtual ~PreferenceManager();


  /**
   * General entry that checks either user or system preferences file.
   */
  bool hasEntry(const std::string& key) const;

  template <class T>
  boost::optional<T> getOptional(const std::string& key) const {
    boost::optional<T> user = userPref_.getOptional<T>(key);
    if (user)
      return user;

    boost::optional<T> sys = systemPref_.getOptional<T>(key);
    if (sys)
      return sys;

    boost::optional<T> def = defaultPref_.getOptional<T>(key);
    return def;
  }

  template <class T>
  T get(const std::string& key) const {
    boost::optional<T> ret = PreferenceManager::getOptional<T>(key);
    if (ret)
      return ret.get();
    else
      throw Exception("Could not read preference: " + key);
  }

  template <class T>
  T get(const std::string& key, T defaultValue) const {
    boost::optional<T> ret = PreferenceManager::getOptional<T>(key);
    if (ret)
      return ret.get();
    else
      return defaultValue;
  }


  /**
   * Calls the put function for UserPref PropertyFile
   *
   * @param key
   * @param value
   */
  template <class T>
  void put(const std::string& key, T value) {
    userPref_.put(key, value);
  }

  /**
   * Reloads user preference file
   */
  void load();

  /**
   * Writes stored user preference data to user preferences file.
   */
  bool save();

 private:
  Log log_;
  const FileInfo defaultPrefFileInfo_;
  PropertyFile defaultPref_;

  const FileInfo systemPrefFileInfo_;
  PropertyFile systemPref_;

  const FileInfo userPrefFileInfo_;
  PropertyFile userPref_;
};

#endif  // UTIL_PREFERENCEMANAGER_H
