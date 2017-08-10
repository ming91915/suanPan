/**
 * @class Storage
 * @brief A candidate Storage class that stores FEM objects.
 * @author T
 * @date 02/08/2017
 * @version 0.1.1
 * @file Storage.h
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <suanPan.h>
#include <unordered_map>
using std::unordered_map;

template <typename T> class Storage
{
    unordered_map<unsigned int, shared_ptr<T>> DATA; /**< The map stores the pointer. */
public:
    Storage() {}
    ~Storage()
    {
#ifdef SUANPAN_DEBUG
        printf("Storage dtor() Called.\n");
#endif
    }

    void insert(const shared_ptr<T>&);

    void erase(const unsigned int&);

    const shared_ptr<T>& getItem(const unsigned int&);

    unsigned size();
};

#endif
