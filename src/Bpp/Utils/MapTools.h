// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_UTILS_MAPTOOLS_H
#define BPP_UTILS_MAPTOOLS_H

#include <map>
#include <vector>


namespace bpp
{
/**
 * @brief A fiew tools working on map objects.
 */
class MapTools
{
public:
  /**
   * @brief Get a vector of all keys in a map.
   *
   * @param myMap the map to check.
   * @return a vector of all keys.
   */
  template<class Key, class T, class Cmp>
  static std::vector<Key> getKeys(const std::map<Key, T, Cmp>& myMap)
  {
    std::vector<Key> keys;
    for (typename std::map<Key, T>::const_iterator i = myMap.begin(); i != myMap.end(); i++)
    {
      keys.push_back(i->first);
    }
    return keys;
  }

  /**
   * @brief Get a vector of all keys in a map.
   *
   * @param myMap the map to check.
   * @return a vector of all keys.
   */
  template<class Key, class T>
  static std::vector<Key> getKeys(const std::map<Key, T>& myMap)
  {
    std::vector<Key> keys;
    for (typename std::map<Key, T>::const_iterator i = myMap.begin(); i != myMap.end(); i++)
    {
      keys.push_back(i->first);
    }
    return keys;
  }

  /**
   * @brief Get a vector of all values in a map.
   *
   * @param myMap the map to check.
   * @return a vector of all values.
   */
  template<class Key, class T, class Cmp>
  static std::vector<T> getValues(const std::map<Key, T, Cmp>& myMap)
  {
    std::vector<T> values;
    for (typename std::map<Key, T>::const_iterator i = myMap.begin(); i != myMap.end(); i++)
    {
      values.push_back(i->second);
    }
    return values;
  }

  /**
   * @brief Get a vector of all values in a map.
   *
   * @param myMap the map to check.
   * @return a vector of all values.
   */
  template<class Key, class T>
  static std::vector<T> getValues(const std::map<Key, T>& myMap)
  {
    std::vector<T> values;
    for (typename std::map<Key, T>::const_iterator i = myMap.begin(); i != myMap.end(); i++)
    {
      values.push_back(i->second);
    }
    return values;
  }
};
} // end of namespace bpp.
#endif // BPP_UTILS_MAPTOOLS_H
