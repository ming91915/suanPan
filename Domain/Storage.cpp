#include "Storage.h"

template <typename T> void Storage<T>::insert(const shared_ptr<T>& I)
{
    DATA[I->getTag()] = I;
}

template <typename T> void Storage<T>::erase(const unsigned& L) { DATA.erase(L); }

template <typename T> const shared_ptr<T>& Storage<T>::getItem(const unsigned& L)
{
    return DATA.at(L);
}

template <typename T> unsigned Storage<T>::size() { return DATA.size(); }
