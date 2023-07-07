/************************************************************************/
/**
 * @file chPath.h
 * @author AccelMR
 * @date 2022/06/23
 * @brief Path that is generic to platform so it can be used along the engine with
 *        no problem.
 */
 /************************************************************************/
#pragma once

/************************************************************************/
/*
 * Includes
 */                                                                     
/************************************************************************/
#include "chPrerequisitesUtilities.h"

namespace chEngineSDK{
namespace fs = std::filesystem;

class CH_UTILITY_EXPORT Path
{
 public:
 /*
  *   Default constructor
  */
  FORCEINLINE Path() = default;

  /** 
   *   Constructor from a simple string.
   * 
   * @param _path
   *  As string.
   **/
  FORCEINLINE Path(const String& _path);

  /** 
   *   Constructor from a char array.
   * 
   * @param _path
   *  As char array.
   **/
  FORCEINLINE Path(const char* _path);

#if USING(CH_PLATFORM_WIN32)
  /** 
   *   Constructor from a wide string. Windows Only constructor.
   * 
   * @param _path
   *  As Wide string.
   **/
  Path(const WString& _path);
#endif

 /*
  *   Default destructor
  */
  ~Path() = default;

  /** 
   *   True if path is relative, false for absolute path.
   **/
  FORCEINLINE bool 
  isRelative() const;

  /** 
   *   Constructs a string from this path.
   **/
  String
  toString() const;
  
  /** 
   *   Constructs a wide string from this path.
   **/
#if USING(CH_PLATFORM_WIN32)
  WString
#else
  String
#endif //#if USING(CH_PLATFORM_WIN32)
  getPlatformString() const;


  /** 
   *   Returns if this Path exists already in the system.
   **/
  bool
  exist() const;

  /** 
   *   True if is directory, false if file or not valid.
   **/
  FORCEINLINE bool
  isDirectory() const;

  /** 
   *   True if this path is file.
   **/
  FORCEINLINE bool
  isFile() const;

  /** 
   *   Changes internal path and sanitize it.
   * 
   * @param path
   *    The new string to be a path.
   **/
  FORCEINLINE void
  setPath(const String& path);

  /*
  *   Returns the file name of this path. 
  * 
  *  @param extension = true
  *   If the filename includes extension
  */
  String
  getFileName(bool extension = true) const;

  /*
  *   Returns extension of this path
  */
  String
  getExtension() const;

  /*
  *   Return the directory of this path. 
  */
  String
  getDirectory() const;

  FORCEINLINE Path
  join(const Path& rhs) const;

/************************************************************************/
/*
 * Static methods.
 */                                                                     
/************************************************************************/
 public:
  
  /** 
   *   Removes all illegal characters that cannot be in a path.
   * 
   * @param [OUT] path
   *    The path to be modified.
   **/
  static void 
  stripInvalid(String& path);

 private:

 /** 
  *   Cleans the path by removing any double '//' and replacing any '\' to '/'
  **/
  void 
  sanitize(const String& toSanitize);

  /** 
   *   Sanitize this path using platform specific files and helpers.
   **/
  void
  platformSanitize(const String& toSanitize);

#if USING(CH_PLATFORM_WIN32)
  /** 
   *   Returns the device associated with this path.
   **/
  String
  getDevice() const;
#endif

  /*
  * For Hash map to be used
  */
  bool
  operator<(const Path& other) const;

private:
  friend class FileSystem;

  Vector<String> m_directories;
  String m_filename;
  String m_extension;

#if USING(CH_PLATFORM_WIN32)
  String m_device; 
#endif

  mutable bool m_isDirectory;
  bool m_isAbsolute;

  bool m_exist;
};

/************************************************************************/
/*
 * Implementations.
 */                                                                     
/************************************************************************/

/*
*/
FORCEINLINE Path::Path(const String& _path) 
  : m_isAbsolute(false) {
  sanitize(_path);
}

/*
*/
FORCEINLINE Path::Path(const char* _path)
  : m_isAbsolute(false) {
  sanitize(_path);
}

/*
*/
FORCEINLINE bool
Path::isDirectory() const {
  return m_isDirectory;
}

/*
*/
FORCEINLINE bool
Path::isRelative() const{
  return !m_isAbsolute;
}

/*
*/
FORCEINLINE bool
Path::isFile() const {
  return !m_isDirectory && exist();
}
/*
*/
FORCEINLINE void
Path::setPath(const String& path) {
  m_isAbsolute = false;
  m_isDirectory = false;
  m_exist = false;
  sanitize(path);
}

/*
*/
FORCEINLINE bool 
Path::operator<(const Path& other) const {
  return toString() < other.toString();
}

/*
*/
FORCEINLINE Path
Path::join(const Path& rhs) const {
  return rhs.isRelative() ? Path(toString() + rhs.toString()) : Path(rhs);
}
}

