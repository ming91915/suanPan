/**
 * @class Storage
 * @brief A candidate Storage class that stores FEM objects.
 * @author T
 * @date 05/09/2017
 * @version 0.2.1
 * @file Storage.hpp
 * @{
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <suanPan.h>
#include <unordered_map>
#include <unordered_set>

using std::unordered_map;
using std::unordered_set;
using std::vector;

template <typename T>
const char* StorageType() {
    return "Unknown";
}

class Constraint;
class Element;
class Load;
class Material;
class Node;
class Recorder;

template <>
inline const char* StorageType<Constraint>() {
    return "Constraint";
}
template <>
inline const char* StorageType<Element>() {
    return "Element";
}
template <>
inline const char* StorageType<Load>() {
    return "Load";
}
template <>
inline const char* StorageType<Material>() {
    return "Material";
}
template <>
inline const char* StorageType<Node>() {
    return "Node";
}
template <>
inline const char* StorageType<Recorder>() {
    return "Recorder";
}

template <typename T>
class Storage {
    typedef T object_type;

    const char* type_string = StorageType<object_type>();

    vector<shared_ptr<T>> fish;                  /**< data storage */
    unordered_set<unsigned> bait;                /**< data storage */
    unordered_map<unsigned, shared_ptr<T>> pond; /**< data storage */
public:
    Storage() { suanpan_debug("Storage of %s ctor() called.\n", type_string); }
    ~Storage() { suanpan_debug("Storage of %s dtor() called.\n", type_string); }

    typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cbegin() const { return pond.cbegin(); }
    typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cend() const { return pond.cend(); }
    typename unordered_map<unsigned, shared_ptr<T>>::iterator begin() { return pond.begin(); }
    typename unordered_map<unsigned, shared_ptr<T>>::iterator end() { return pond.end(); }

    bool insert(const shared_ptr<T>& I) {
        auto flag = pond.insert({ I->get_tag(), I }).second;
        if(!flag) suanpan_warning("insert() fails to insert %s %u.\n", type_string, I->get_tag());
        return flag;
    }
    shared_ptr<T>& operator[](const unsigned& L) { return pond[L]; }
    const shared_ptr<T>& at(const unsigned& L) const { return pond.at(L); }

    const vector<shared_ptr<T>>& get() const { return fish; }

    bool find(const unsigned& L) const { return pond.find(L) != pond.end(); }
    bool erase(const unsigned& L) { return pond.erase(L) == 1; }
    void enable(const unsigned& L) {
        if(find(L)) pond[L]->enable();
    }
    void disable(const unsigned& L) {
        if(find(L)) pond[L]->disable();
    }
    void update() {
        reset();
        fish.reserve(size());
        for(const auto& I : pond)
            if(I.second->is_active())
                fish.push_back(I.second);
            else
                bait.insert(I.second->get_tag());
    }
    void enable() {
        for(const auto& I : pond) I.second->enable();
    }
    void reset() {
        fish.clear();
        bait.clear();
    }
    void clear() {
        pond.clear();
        reset();
    }

    size_t size() const { return pond.size(); }
};

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cbegin(const Storage<T>& S) {
    return S.cbegin();
}

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cend(const Storage<T>& S) {
    return S.cend();
}

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::iterator begin(Storage<T>& S) {
    return S.begin();
}

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::iterator end(Storage<T>& S) {
    return S.end();
}

#endif

//! @}
