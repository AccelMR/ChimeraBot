/************************************************************************/
/**
 * @file chDebug.cpp
 * @author AccelMR
 * @date 2022/06/15
 *   
 */
 /************************************************************************/

/************************************************************************/
/*
 * Includes
 */                                                                     
/************************************************************************/
#include "chDebug.h"

#include "chStringUtils.h"
#include "chFileSystem.h"

#if USING(CH_COMPILER_MSVC)
# include "chWindows.h"
# include <iostream>

void
logIDEConsole(const chEngineSDK::String& msg) {
  OutputDebugString(msg.c_str());
  OutputDebugString("\n");

  std::cout << msg << std::endl;
}
#else
void
logIDEConsole( const chEngineSDK::String &msg ) {
  OutputDebugString( msg.c_str() );
  OutputDebugString( "\n" );

  std::cout << msg <<std::endl;
}
#endif

namespace chEngineSDK{
const Array<String, static_cast<SIZE_T>(LOG_LEVEL::kCOUNT)> LOG_LEVEL_NAMES = {
  "FATAL",
  "ERROR",
  "WARN",
  "INFO",
  "DEBUG"
};

/*
*/
void
Debug::logMessage(const String& msg, LOG_LEVEL level) {
  m_log.logMsg(msg, level);
  //auto logMsg = StringUtils::format("[{}] {}", LOG_LEVEL_NAMES[static_cast<int32>(level)], msg);
  auto logMsg = String("[" + LOG_LEVEL_NAMES[static_cast<int32>(level)] + "]\t" + msg);
  logIDEConsole(logMsg);
}

/*
*/
void
Debug::saveLog(const Path& path) const {
  auto fileStream = FileSystem::createAndOpenFile(path);
  CH_ASSERT(fileStream);
  for (const LogEntry& logEntry : m_log.getAllEntries()) {
    fileStream << LOG_LEVEL_NAMES[static_cast<int32>(logEntry.getLevel())];
    fileStream << logEntry.getLogMessage() << "\n";
  }
  fileStream->close();
}

/*
*/
CH_UTILITY_EXPORT Debug&
g_Debug() {
  static Debug debug;
  return debug;
}

}


