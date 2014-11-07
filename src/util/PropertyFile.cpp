#include <util/PropertyFile.h>

#include <sstream>

#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/info_parser.hpp>

#include <util/Exception.h>


PropertyFile::PropertyFile(const FileInfo & fileInfo)
    : file_(fileInfo)
{
}

PropertyFile::~PropertyFile()
{
}


void PropertyFile::load()
{
  if (!file_.exists())
    throw Exception("Trying to load property file that doesn't exist");
  std::stringstream ss(file_.read());
  boost::property_tree::read_info(ss, properties_);
}


bool PropertyFile::save()
{
  assert(file_.getFileInfo().getFiletype() == FileInfo::TYPE_WRITABLE);
  std::stringstream data;
  try {
    boost::property_tree::write_info(data, properties_);
  } catch (const boost::property_tree::ptree_error& e) {
    return false;
  }

  return file_.safeWrite(data.str());
}


bool PropertyFile::hasEntry(const std::string& key) const
{
  bool isValidPath = true;
  try {
    properties_.get<int>(key);
  } catch (const boost::property_tree::ptree_bad_path& e) {
    isValidPath = false;
  } catch (const boost::property_tree::ptree_bad_data&) {
    // Do nothing in this catch (just supress the exception)
    // The conversion failed, but that doesn't matter.
  }

  return isValidPath;
}
