/************************************************************************/
/**
 * @file chPath.h
 * @author AccelMR
 * @date 2022/06/23
 * @brief Path file that implements all windows specific functions.
 */
 /************************************************************************/

/************************************************************************/
/*
 * Includes
 */                                                                     
/************************************************************************/
#include "chPath.h"
#include "chUnicode.h"
#include "chStringUtils.h"

#include "chWindows.h"

namespace chEngineSDK {
namespace fs = std::filesystem;
/*
*/
Path::Path(const WString& _path)
  : m_isAbsolute( false ) {
  String strPath = UTF8::fromWide(_path);
  sanitize(strPath);
}

/*
*/
String
Path::getDevice() const {
  return m_device;
}
}
