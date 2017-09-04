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
#include <unordered_map>
#include <unordered_set>

using std::vector;
using std::unordered_map;
using std::unordered_set;

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

template <typename T> class Storage
{
    typedef T object_type;

    vector<shared_ptr<T>> fish;                  /**< data storage */
    unordered_set<unsigned> silence;             /**< data storage */
    unordered_map<unsigned, shared_ptr<T>> pool; /**< data storage */
public:
    Storage()
    {
        suanpan_debug("Storage of %s ctor() called.\n", StorageType<object_type>());
    }
    ~Storage()
    {
        suanpan_debug("Storage of %s dtor() called.\n", StorageType<object_type>());
    }

    typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cbegin() const
    {
        return pool.cbegin();
    }
    typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cend() const
    {
        return pool.cend();
    }
    typename unordered_map<unsigned, shared_ptr<T>>::iterator begin()
    {
        return pool.begin();
    }
    typename unordered_map<unsigned, shared_ptr<T>>::iterator end() { return pool.end(); }

    bool insert(const shared_ptr<T>& I)
    {
        auto flag = pool.insert({ I->get_tag(), I });
        if(flag.second == false)
            suanpan_warning("insert() fails to insert %s %u.\n",
                StorageType<object_type>(), I->get_tag());
        return flag.second;
    }
    shared_ptr<T>& operator[](const unsigned& L) { return pool[L]; }
    const shared_ptr<T>& at(const unsigned& L) const { return pool.at(L); }

    const vector<shared_ptr<T>>& get() const { return fish; }

    bool find(const unsigned& L) const { return pool.find(L) != pool.end(); }
    bool erase(const unsigned& L) { return pool.erase(L) == 1; }
    void enable(const unsigned& L)
    {
        if(find(L)) pool[L]->enable();
    }
    void disable(const unsigned& L)
    {
        if(find(L)) pool[L]->disable();
    }
    void update()
    {
        reset();
        fish.reserve(size());
        for(const auto& I : pool)
            if(I.second->is_active())
                fish.push_back(I.second);
            else
                silence.insert(I.second->get_tag());
    }
    void enable()
    {
        for(const auto& I : pool) I.second->enable();
    }
    void reset()
    {
        fish.clear();
        silence.clear();
    }
    void clear()
    {
        pool.clear();
        reset();
    }

    size_t size() const { return pool.size(); }
};

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cbegin(
    const Storage<T>& S)
{
    return S.cbegin();
}

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::const_iterator cend(const Storage<T>& S)
{
    return S.cend();
}

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::iterator begin(const Storage<T>& S)
{
    return S.begin();
}

template <typename T>
typename unordered_map<unsigned, shared_ptr<T>>::iterator end(const Storage<T>& S)
{
    return S.end();
}

#endif

//! @}
