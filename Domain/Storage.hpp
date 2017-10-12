/**
 * @class Storage
 * @brief A candidate Storage class that stores FEM objects.
 * @author T
 * @date 01/10/2017
 * @version 0.2.3
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

class Amplitude;
class Constraint;
class Converger;
class Criterion;
class Domain;
class DomainBase;
class Element;
class Integrator;
class Load;
class Material;
class Node;
class Recorder;
class Section;
class Solver;

template <typename T> const char* StorageType() { return "Unknown"; }
template <> inline const char* StorageType<Amplitude>() { return "Amplitude"; }
template <> inline const char* StorageType<Constraint>() { return "Constraint"; }
template <> inline const char* StorageType<Converger>() { return "Converger"; }
template <> inline const char* StorageType<Criterion>() { return "Criterion"; }
template <> inline const char* StorageType<Domain>() { return "Domain"; }
template <> inline const char* StorageType<DomainBase>() { return "Domain"; }
template <> inline const char* StorageType<Element>() { return "Element"; }
template <> inline const char* StorageType<Integrator>() { return "Integrator"; }
template <> inline const char* StorageType<Load>() { return "Load"; }
template <> inline const char* StorageType<Material>() { return "Material"; }
template <> inline const char* StorageType<Node>() { return "Node"; }
template <> inline const char* StorageType<Recorder>() { return "Recorder"; }
template <> inline const char* StorageType<Section>() { return "Section"; }
template <> inline const char* StorageType<Solver>() { return "Solver"; }

template <typename T> class Storage : public std::enable_shared_from_this<Storage<T>> {
    using const_iterator = typename unordered_map<unsigned, shared_ptr<T>>::const_iterator;
    using iterator = typename unordered_map<unsigned, shared_ptr<T>>::iterator;

    const char* type = StorageType<object_type>();

    vector<shared_ptr<T>> fish;                  /**< data storage */
    unordered_set<unsigned> bait;                /**< data storage */
    unordered_map<unsigned, shared_ptr<T>> pond; /**< data storage */
public:
    typedef T object_type;

    Storage();
    ~Storage();

    const_iterator cbegin() const;
    const_iterator cend() const;
    iterator begin();
    iterator end();

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

template <typename T> Storage<T>::Storage() { suanpan_debug("Storage of %s ctor() called.\n", type); }

template <typename T> Storage<T>::~Storage() { suanpan_debug("Storage of %s dtor() called.\n", type); }

template <typename T> typename Storage<T>::const_iterator Storage<T>::cbegin() const { return pond.cbegin(); }

template <typename T> typename Storage<T>::const_iterator Storage<T>::cend() const { return pond.cend(); }

template <typename T> typename Storage<T>::iterator Storage<T>::begin() { return pond.begin(); }

template <typename T> typename Storage<T>::iterator Storage<T>::end() { return pond.end(); }

template <typename T> bool Storage<T>::insert(const shared_ptr<T>& I) {
    auto flag = pond.insert({ I->get_tag(), I }).second;
    if(!flag) suanpan_warning("insert() fails to insert %s %u.\n", type, I->get_tag());
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

template <typename T> typename Storage<T>::const_iterator cbegin(const Storage<T>& S) { return S.cbegin(); }

template <typename T> typename Storage<T>::const_iterator cend(const Storage<T>& S) { return S.cend(); }

template <typename T> typename Storage<T>::iterator begin(Storage<T>& S) { return S.begin(); }

template <typename T> typename Storage<T>::iterator end(Storage<T>& S) { return S.end(); }

using AmplitudeStorage = Storage<Amplitude>;
using ConstraintStorage = Storage<Constraint>;
using ConvergerStorage = Storage<Converger>;
using CriterionStorage = Storage<Criterion>;
using DomainStorage = Storage<Domain>;
using DomainBaseStorage = Storage<DomainBase>;
using ElementStorage = Storage<Element>;
using IntegratorStorage = Storage<Integrator>;
using LoadStorage = Storage<Load>;
using MaterialStorage = Storage<Material>;
using NodeStorage = Storage<Node>;
using RecorderStorage = Storage<Recorder>;
using SectionStorage = Storage<Section>;
using SolverStorage = Storage<Solver>;

#endif

//! @}
