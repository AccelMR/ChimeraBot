/************************************************************************/
/**
 * @file chFileSystem.cpp
 * @author AccelMR
 * @date 2022/06/27
 * @brief File system that is platform specific.
 *   
 */
 /************************************************************************/

/************************************************************************/
/*
 * Includes
 */                                                                     
/************************************************************************/
#include "chFileSystem.h"

#include "chPath.h"
#include "chStringUtils.h"

namespace chEngineSDK {
namespace fs = std::filesystem;/*
*/
/*
UnqPtr<FileStream>
FileSystem::openFile(const Path& path, bool readOnly) {
  if (path.isDirectory()) {
    String msg("Can't create file in directory path. <" + path.getString() +">");
    UTIL_ERROR(msg);
    return nullptr;
  }

  return ch_new_unique(FileStream, path, readOnly);
}*/

/*
*/
bool
FileSystem::createDirectory(const Path& path) {
  fs::path fsPath(path.toString());
  if (!fs::create_directory(fsPath)) {
    return false;
  }
  path.m_isDirectory = true;
  return true;
}

/*
*/
bool
FileSystem::createDirectories(const Path& path) {
  fs::path fsPath(path.toString());
  if (!fs::create_directories(fsPath)) {
    return false;
  }
  path.m_isDirectory = true;
  return true;
}

/*
*/
bool
FileSystem::exists(const Path& path) {
  fs::path fsPath(path.toString());
  return fs::exists(fsPath);
}

/*
*/
SPtr<DataStream>
FileSystem::openFile(const Path& path, bool readOnly /*= true*/) {
  const Path fullPath = path.isRelative() ? 
    fs::absolute(path.getPlatformString()).generic_string() : 
    path;

  if (!fullPath.isFile()) {
    return nullptr;
  }

  AccesModeFlag accessMode(ACCESS_MODE::kREAD);
  if (!readOnly) {
    accessMode.set(ACCESS_MODE::kWRITE);
  }

  return ch_shared_ptr_new<FileDataStream>(fullPath, accessMode, true);
}

/*
*/
SPtr<DataStream>
FileSystem::createAndOpenFile(const Path& path) {
  return ch_shared_ptr_new<FileDataStream>(path, ACCESS_MODE::kWRITE, true);
}

/*
*/
void
FileSystem::dumpMemStreamIntoFile(const SPtr<DataStream>& memStream, const Path& path) {
  auto fileStream = ch_shared_ptr_new<FileDataStream>(path, memStream);
}

/*
*/
bool
FileSystem::remove(const Path &path) {
  return fs::remove(path.getPlatformString());
}

/*
*/
bool
FileSystem::removeAll(const Path& path) {
  return fs::remove_all(path.getPlatformString());
}

/*
*/
Vector<uint8>
FileSystem::fastReadFile(const Path& path) {
  Vector<uint8> ret;

  auto fileData = FileSystem::openFile(path);
  auto size = fileData->size();

  ret.resize(size);

  fileData->read(&ret[0], size);

  fileData->close();

  return ret;
}

/*
*/
bool 
FileSystem::isPathRelative(const Path& basePath, const Path& targetPath) {
  // Both paths should be absolute, or the comparison won't make sense
  if (!basePath.isRelative() && !targetPath.isRelative()) {
    const auto& basePathDirs = basePath.getDirectory();
    const auto& targetPathDirs = targetPath.getDirectory();

    // Check if the basePath is a prefix of the targetPath
    if (basePathDirs.size() <= targetPathDirs.size()) {
      return std::equal(basePathDirs.begin(), basePathDirs.end(), targetPathDirs.begin());
    }
  }

  return false;
}

/*
*/
void
FileSystem::getChildren(const Path& path, Vector<Path>& files, Vector<Path>& directories) {
  if (!path.exist()) {
    return;
  }

  std::filesystem::path fsPath(path.toString());

  if (!std::filesystem::is_directory(fsPath)) {
    return;
  }

  for (const auto& entry : std::filesystem::directory_iterator(fsPath)) {
    Path childPath(entry.path().string());
    if (entry.is_directory()) {
      directories.push_back(childPath);
    }
    else {
      files.push_back(childPath);
    }
  }
}
}
