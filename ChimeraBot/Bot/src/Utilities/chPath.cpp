/************************************************************************/
/**
 * @file chPath.cpp
 * @author AccelMR
 * @date 2022/06/23
 * @brief Path that is generic to platform so it can be used along the engine with
 *        no problem.
 *   
 */
 /************************************************************************/

/************************************************************************/
/*
 * Includes
 */                                                                     
/************************************************************************/
#include "chPath.h"

#include "chUnicode.h"
#include "chFileSystem.h"
#include "chStringUtils.h"

namespace chEngineSDK{

/*
*/
void
Path::stripInvalid(String& path) {
  const String ILLEGAL_CHARACTERS = "\\/:?\"<>|";
  for (auto character : path) {
    if (ILLEGAL_CHARACTERS.find(character) != String::npos) {
      character = ' '; 
    }
  }
}

/*
*/
void
Path::sanitize(const String& toSanitize) {
  //Make a copy
  String EndPath = toSanitize;

  //remove invalid characters.
  stripInvalid(EndPath);

  //Check for most common errors when getting a path.
  EndPath = StringUtils::replaceAllSubStr(toSanitize, "\\\\", "/");
  EndPath = StringUtils::replaceAllChars(toSanitize, '\\', '/');

  platformSanitize(EndPath);
}

/*
*/
String
Path::toString() const {
  String Ret = m_directories.empty() ? "" : StringUtils::join(m_directories, "/");
  if (m_isAbsolute) {
#if USING(CH_PLATFORM_WIN32)
    Ret.insert(0, m_device + ":/");
#else
    Ret.insert(0, "/");
#endif //#if USING(CH_PLATFORM_WIN32)
  }
  if (m_isDirectory) {
    Ret.push_back('/');
  }
  else {
    Ret += "/" + m_filename + m_extension;
  }
  return Ret;
}

/*
*/
void
Path::platformSanitize(const String& toSanitize) {
  fs::path winPath(toSanitize);
  m_exist = fs::exists(winPath);

  m_directories = StringUtils::splitString(toSanitize, '/');

  if (winPath.is_absolute()) {
    m_isAbsolute = true;

#if USING(CH_PLATFORM_WIN32)
    m_device = *m_directories.begin();
    m_device.pop_back();
#endif //#if USING(CH_PLATFORM_WIN32)

    m_directories.erase(m_directories.begin());
  }

  m_isDirectory = fs::is_directory(winPath);
  //if we send only a file this the m_directories will only have 1 size
  if ((!m_isDirectory && m_exist) || m_directories.size() == 1) {
    m_filename = m_directories.back();
    m_directories.pop_back();
  }

  //Remove ext
  SIZE_T pos = m_filename.rfind('.');
  if (pos != std::string::npos) {
    m_filename = m_filename.substr(0, pos);
    //set internally
  }
  m_extension = winPath.extension().string();

}
/*
*/
#if USING(CH_PLATFORM_WIN32)
  WString
#else
  String
#endif //#if USING(CH_PLATFORM_WIN32)
Path::getPlatformString() const {
#if USING(CH_PLATFORM_WIN32)
  return UTF8::toWide(toString());
#else
  return toString();
#endif //#if USING(CH_PLATFORM_WIN32)
}

/*
*/
bool
Path::exist() const {
  return FileSystem::exists(*this);
}

/*
*/
String
Path::getFileName(bool extension) const {
  return extension ? m_filename + m_extension : m_filename;
}

/*
*   Returns the file name of this path.
*/
String
Path::getExtension() const{
  return m_extension;
}


/*
*/
String
Path::getDirectory() const {
  String Ret = StringUtils::join(m_directories, "/");
  if (m_isAbsolute) {
#if USING(CH_PLATFORM_WIN32)
    Ret.insert(0, m_device + ":/");
#else
    Ret.insert(0, "/");
#endif //#if USING(CH_PLATFORM_WIN32)
  }
  Ret.push_back('/');

  return Ret;
}
}


