/************************************************************************/
/**
 * @file chStdHeaders.h
 * @author AccelMR <accel.mr@gmail.com>
 * @date 2021/09/10
 * @brief Header for the STD libraries.
 * 
 * This file includes the most common and required STL objects.
 * It takes account of the operating system of the build to modify
 * the required object.
 *
 * @bug No bug known.
 */
 /************************************************************************/
#pragma once

/************************************************************************/
/*
 * C type objects
 */
/************************************************************************/
#include <cassert>
#include <cmath>
#include <limits>

/************************************************************************/
/*
 * Types.
 */
/************************************************************************/
#include <type_traits>

/************************************************************************/
/*
 * STL Containers.
 */
/************************************************************************/
#include <array>
#include <vector>
#include <queue>

/************************************************************************/
/**
 * Standard Containers defined as own
 */
 /************************************************************************/
#include <set>
#include <map>

 /************************************************************************/
 /*
  * C++ Stream Stuff
  */                                                                     
 /************************************************************************/
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>

#include <functional>

#include <locale>
#include <codecvt>

#include <filesystem>

/************************************************************************/
/*
 * Threading
 */                                                                     
/************************************************************************/
#include <mutex>
#include <thread>

#include <execution>

#include "chPrerequisitesUtilities.h"
#include "chUsing.h"


/*****************************************************************************/
/**
 * Windows specifics
 */
/*****************************************************************************/
#if USING(CH_PLATFORM_WIN32)
  //Undefine min & max
# undef min
# undef max

# if !defined(NOMINMAX) && defined(_MSC_VER)
#   define NOMINMAX     //Required to stop windows.h messing up std::min
# endif
# if defined( __MINGW32__ )
#   include <unistd.h>
# endif
#endif //#if USING(CH_PLATFORM_WIN32)


namespace chEngineSDK {
using std::char_traits;
using std::basic_string;
using std::basic_stringstream;
using std::min;
using std::forward;
using std::ios;

/** 
 *   Std alias allocator.
 **/
template<typename T>
using Alloc = std::allocator<T>;

/**
*  @brief Fixed-size array sequence container class, holds its elements in a strict linear
*         sequence
*/
template<typename T, SIZE_T size>
using Array = std::array<T, size>;

/*
 *   Vector wrapper to use along the engine.
 **/
template <typename T, class A = Alloc<T>>
using Vector = std::vector<T, A>;

/**
 * @brief An associative container containing an ordered set of elements.
 */
template<typename T, typename P = std::less<T>, typename A = Alloc<T>>
using Set = std::set<T, P, A>;

/**
 * @brief An associative container containing an ordered set of key-value
 *        pairs.
 */
template<typename K, 
         typename T, 
         typename Compare = std::less<K>,
         typename A = std::allocator<std::pair<const K, T>>>
using Map = std::map<K, T, Compare, A>;

/** 
 *   Queue wrapper.
 **/
template< typename T, class Container = std::deque<T>> 
using Queue =  std::queue<T, Container>;

/************************************************************************/
/*
 * Smart pointers
 */                                                                     
/************************************************************************/

/** 
 *   Shared pointer that will be used for Chimera.
 **/
template <typename T>
using SPtr = std::shared_ptr<T>;

/** 
 *   Weak pointer used along Chimera.
 **/
template<typename T>
using WPtr = std::weak_ptr<T>;

/** 
 *   Unique pointer used along Chimera.
 **/
template<class T, typename A = std::default_delete<T>>
using UnqPtr = std::unique_ptr<T, A>;

/**
 * @brief Create a new shared pointer using a custom allocator category.
 */
template<class T, class... Args>
SPtr<T>
ch_shared_ptr_new(Args&&... args) {
  return std::allocate_shared<T>(Alloc<T>(),
                                 std::forward<Args>(args)...);
}

/**
 * @brief Create a new shared pointer using a custom allocator category.
 */
template<class T, class... Args>
UnqPtr<T>
ch_unique_ptr_new(Args&&... args) {
  return UnqPtr<T>(new T(std::forward<Args>(args)...));
}

/************************************************************************/
/*
 * String related
 */                                                                     
/************************************************************************/
/**
 * @brief Wide string stream used for primarily for constructing strings
 *        consisting of ASCII text.
 */
using StringStream = std::stringstream;

/**
 * @brief Basic string that uses geEngine memory allocators.
 */
template<typename T>
using BasicString = basic_string<T, char_traits<T>, std::allocator<T>>;

/**
 * @brief Basic string stream that uses geEngine memory allocators.
 */
template<typename T>
using BasicStringStream = basic_stringstream<T, char_traits<T>, std::allocator<T>>;

/**
 * @brief Wide string used primarily for handling Unicode text.
 */
using WString = BasicString<UNICHAR>;

/**
 * @brief Narrow string used primarily for handling ASCII text.
 */
using String = std::string;//= BasicString<ANSICHAR>;

/**
 * @brief Wide string used UTF-16 encoded strings.
 */
using U16String = BasicString<char16_t>;

/**
 * @brief Wide string used UTF-32 encoded strings.
 */
using U32String = BasicString<char32_t>;

/************************************************************************/
/*
 * Threading
 */                                                                     
 /************************************************************************/

/**
 * @brief Wrapper for the C++ std::recursive_mutex.
 */
using RecursiveMutex = std::recursive_mutex;

/**
 * @brief Wrapper for the C++ std::unique_lock<std::recursive_mutex>.
 */
using RecursiveLock = std::unique_lock<RecursiveMutex>;
}
