/**
* @class Storage
* @brief A candidate Storage class that stores FEM objects.
* @author T
* @date 02/09/2017
* @version 0.2.0
* @file Storage_white.hpp
* @{
*/

#ifndef STORAGE_H
#define STORAGE_H

#include <suanPan.h>

class Constraint;
class Element;
class Load;
class Material;
class Node;
class Recorder;

template <typename T> const char* StorageType() { return "Unknown"; }

template <> inline const char* StorageType<Constraint>() { return "Constraint"; }

template <> inline const char* StorageType<Element>() { return "Element"; }

template <> inline const char* StorageType<Load>() { return "Load"; }

template <> inline const char* StorageType<Material>() { return "Material"; }

template <> inline const char* StorageType<Node>() { return "Node"; }

template <> inline const char* StorageType<Recorder>() { return "Recorder"; }

#include <unordered_map>
#include <unordered_set>

using std::vector;
using std::unordered_set;
using std::unordered_map;

template <typename T> class Storage
{
    typedef T object_type;

    vector<shared_ptr<T>> fast_pool;             /**< data storage */
    unordered_set<unsigned> silent_pool;         /**< data storage */
    unordered_map<unsigned, shared_ptr<T>> pool; /**< data storage */
public:
    Storage();
    ~Storage();

    shared_ptr<T>& operator[](const unsigned&);
    void insert(const shared_ptr<T>&);
    const shared_ptr<T>& at(const unsigned&) const;
    const vector<shared_ptr<T>>& get() const;

    bool find(const unsigned&);
    bool erase(const unsigned&);
    void enable(const unsigned&);
    void disable(const unsigned&);
    void update();
    void enable();
    void clear();

    unsigned size();
};

#endif

//! @}
