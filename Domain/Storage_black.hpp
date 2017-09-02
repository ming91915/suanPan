#pragma once

template <typename T> Storage<T>::Storage()
{
    suanpan_debug("Storage of %s ctor() called.\n", StorageType<object_type>());
}

template <typename T> Storage<T>::~Storage()
{
    suanpan_debug("Storage of %s dtor() called.\n", StorageType<object_type>());
}

template <typename T> shared_ptr<T>& Storage<T>::operator[](const unsigned& L)
{
    return pool[L];
}

template <typename T> void Storage<T>::insert(const shared_ptr<T>& I)
{
    pool.insert({ I->get_tag(), I });
}

template <typename T> const shared_ptr<T>& Storage<T>::at(const unsigned& L) const
{
    return pool.at(L);
}

template <typename T> const vector<shared_ptr<T>>& Storage<T>::get() const
{
    return fast_pool;
}

template <typename T> void Storage<T>::update()
{
    fast_pool.reserve(pool.size());
    for(const auto& I : pool)
        if(I->is_active())
            fast_pool.push_back(I);
        else
            silent_pool.insert(I->get_tag());
}

template <typename T> bool Storage<T>::find(const unsigned& L)
{
    return pool.find(L) != pool.end();
}

template <typename T> bool Storage<T>::erase(const unsigned& L)
{
    return pool.erase(L) == 1;
}

template <typename T> void Storage<T>::enable(const unsigned& L)
{
    if(find(L)) {
        pool[L]->enable();
        silent_pool.erase(L);
    }
}

template <typename T> void Storage<T>::disable(const unsigned& L)
{
    if(find(L)) {
        pool[L]->disable();
        silent_pool.insert(L);
    }
}

template <typename T> void Storage<T>::enable()
{
    for(const auto& I : silent_pool) pool[I]->enable();
    silent_pool.clear();
}

template <typename T> void Storage<T>::clear()
{
    pool.clear();
    silent_pool.clear();
    fast_pool.clear();
}

template <typename T> unsigned Storage<T>::size()
{
    return static_cast<unsigned>(pool.size());
}
