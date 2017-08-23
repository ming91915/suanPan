#include "Storage.h"

template <typename T> void Storage<T>::insert(const shared_ptr<T>& I)
{
    pool[I->get_tag()] = I;
}

template <typename T> void Storage<T>::erase(const unsigned& L) { pool.erase(L); }

template <typename T> const shared_ptr<T>& Storage<T>::get(const unsigned& L)
{
    return pool.at(L);
}

template <typename T> unsigned Storage<T>::size() { return pool.size(); }
