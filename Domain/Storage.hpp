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

template <typename T> const char* StorageType() { return "Unknown"; }

class Constraint;
class Element;
class Load;
class Material;
class Node;
class Recorder;

template <> inline const char* StorageType<Constraint>() { return "Constraint"; }
template <> inline const char* StorageType<Element>() { return "Element"; }
template <> inline const char* StorageType<Load>() { return "Load"; }
template <> inline const char* StorageType<Material>() { return "Material"; }
template <> inline const char* StorageType<Node>() { return "Node"; }
template <> inline const char* StorageType<Recorder>() { return "Recorder"; }

template <typename T> class Storage {
    using const_storage_iterator = typename unordered_map<unsigned, shared_ptr<T>>::const_iterator;
    using storage_iterator = typename unordered_map<unsigned, shared_ptr<T>>::iterator;

    typedef T object_type;

    const char* type_string = StorageType<object_type>();

    vector<shared_ptr<T>> fish;                  /**< data storage */
    unordered_set<unsigned> bait;                /**< data storage */
    unordered_map<unsigned, shared_ptr<T>> pond; /**< data storage */
public:
    Storage();
    ~Storage();

    const_storage_iterator cbegin() const;
    const_storage_iterator cend() const;
    storage_iterator begin();
    storage_iterator end();

    bool insert(const shared_ptr<T>&);
    shared_ptr<T>& operator[](const unsigned&);
    const shared_ptr<T>& at(const unsigned&) const;

    const vector<shared_ptr<T>>& get() const;

    bool find(const unsigned&) const;
    bool erase(const unsigned&);
    void enable(const unsigned&);
    void disable(const unsigned&);

    void update();
    void enable();
    void reset();
    void clear();

    size_t size() const;
};

template <typename T> Storage<T>::Storage() { suanpan_debug("Storage of %s ctor() called.\n", type_string); }

template <typename T> Storage<T>::~Storage() { suanpan_debug("Storage of %s dtor() called.\n", type_string); }

template <typename T> typename Storage<T>::const_storage_iterator Storage<T>::cbegin() const { return pond.cbegin(); }

template <typename T> typename Storage<T>::const_storage_iterator Storage<T>::cend() const { return pond.cend(); }

template <typename T> typename Storage<T>::storage_iterator Storage<T>::begin() { return pond.begin(); }

template <typename T> typename Storage<T>::storage_iterator Storage<T>::end() { return pond.end(); }

template <typename T> bool Storage<T>::insert(const shared_ptr<T>& I) {
    auto flag = pond.insert({ I->get_tag(), I }).second;
    if(!flag) suanpan_warning("insert() fails to insert %s %u.\n", type_string, I->get_tag());
    return flag;
}

template <typename T> shared_ptr<T>& Storage<T>::operator[](const unsigned& L) { return pond[L]; }

template <typename T> const shared_ptr<T>& Storage<T>::at(const unsigned& L) const { return pond.at(L); }

template <typename T> const vector<shared_ptr<T>>& Storage<T>::get() const { return fish; }

template <typename T> bool Storage<T>::find(const unsigned& L) const { return pond.find(L) != pond.end(); }

template <typename T> bool Storage<T>::erase(const unsigned& L) { return pond.erase(L) == 1; }

template <typename T> void Storage<T>::enable(const unsigned& L) {
    if(find(L)) pond[L]->enable();
}

template <typename T> void Storage<T>::disable(const unsigned& L) {
    if(find(L)) pond[L]->disable();
}

template <typename T> void Storage<T>::update() {
    reset();
    fish.reserve(size());
    for(const auto& I : pond)
        if(I.second->is_active())
            fish.push_back(I.second);
        else
            bait.insert(I.second->get_tag());
}

template <typename T> void Storage<T>::enable() {
    for(const auto& I : pond) I.second->enable();
}

template <typename T> void Storage<T>::reset() {
    fish.clear();
    bait.clear();
}

template <typename T> void Storage<T>::clear() {
    pond.clear();
    reset();
}

template <typename T> size_t Storage<T>::size() const { return pond.size(); }

template <typename T> typename Storage<T>::const_storage_iterator cbegin(const Storage<T>& S) { return S.cbegin(); }

template <typename T> typename Storage<T>::const_storage_iterator cend(const Storage<T>& S) { return S.cend(); }

template <typename T> typename Storage<T>::storage_iterator begin(Storage<T>& S) { return S.begin(); }

template <typename T> typename Storage<T>::storage_iterator end(Storage<T>& S) { return S.end(); }

#endif

//! @}
