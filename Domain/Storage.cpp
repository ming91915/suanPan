#include "Storage.h"

template <typename T> void Storage<T>::insert(const shared_ptr<T>& ITEM)
{
    DATA[ITEM->getTag()] = ITEM;
}

template <typename T> void Storage<T>::erase(const unsigned& TAG) { DATA.erase(TAG); }

template <typename T> const shared_ptr<T>& Storage<T>::getItem(const unsigned& TAG)
{
    return DATA.at(TAG);
}

template <typename T> unsigned Storage<T>::size() { return DATA.size(); }
