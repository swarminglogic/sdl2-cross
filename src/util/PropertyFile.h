#ifndef UTIL_PROPERTYFILE_H
#define UTIL_PROPERTYFILE_H

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

#include <util/File.h>
#include <util/FileInfo.h>


/**
 * PropertyFile class.
 *
 * Used to write and read data from a config/parameter file.
 * It works as a wrapper for Boost.PropertyTree, and uses
 * its own defined INFO format for storing files.
 *
 * At the moment it gives access to the PropertyTree container,
 * this is purely for temporary development purposes.
 * When the interface is more predictable, all such access should
 * go through functions defined here.
 *
 * In any case, loading and saving must be done through load() and save().
 *
 * If ever the need for Property management, without the implicit
 * interaction with a file, consider splitting this class into
 * PropertyTree, and PropertyTreeFile.
 *
 * @author SwarmingLogic
 */
class PropertyFile
{
 public:
  explicit PropertyFile(const FileInfo& fileInfo);
  virtual ~PropertyFile();

  /**
   * Call this function to read the config file
   * Throws exception if property file not found.
   */
  void load();


  /**
   * Writes the updated data to the file.
   * Returns true if successful, false otherwise.
   *
   * The file must be TYPE_WRITABLE (asserted).
   */
  bool save();

  /**
   * Checks to see if a key exists within the property file,
   * and that it can be converted to the templated type.
   */
  template <class T>
  bool hasEntry(const std::string& key) const {
    return properties_.get_optional<T>(key);
  }

  /**
   * Checks to see if a key exists within the property file,
   * regardless of what type it is intended to be used as.
   */
  bool hasEntry(const std::string& key) const;

  /**
   * Wrapper function to place a value in the parameter file.
   *
   * @param key    Key to update
   * @param value  Value to update with
   */
  template <class T>
  void put(const std::string& key, T value) {
    properties_.put(key, value);
  }
  template <class T>
  void add(const std::string& key, T value) {
    properties_.add(key, value);
  }

  template <class T>
  T get(const std::string& key) const {
    return properties_.get<T>(key);
  }

  template <class T>
  T get(const std::string& key, T defaultValue) const {
    return properties_.get(key, defaultValue);
  }

  template <class T>
  boost::optional<T> getOptional(const std::string& key) const {
    return properties_.get_optional<T>(key);
  }

 private:
  File file_;
  boost::property_tree::ptree properties_;
};

#endif  // UTIL_PROPERTYFILE_H
