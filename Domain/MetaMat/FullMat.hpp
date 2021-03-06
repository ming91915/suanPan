/**
 * @class FullMat
 * @brief A FullMat class that holds matrices.
 *
 * @author T
 * @date 06/09/2017
 * @version 0.1.0
 * @file FullMat.hpp
 * @addtogroup MetaMat
 * @{
 */

#ifndef FULLMAT_HPP
#define FULLMAT_HPP

template <typename T> class FullMat : public MetaMat<T> {
public:
    FullMat();
    explicit FullMat(const unsigned&);
};

template <typename T> struct is_Full { static const bool value = false; };

template <typename T> struct is_Full<FullMat<T>> { static const bool value = true; };

template <typename T>
FullMat<T>::FullMat()
    : MetaMat<T>() {}

template <typename T>
FullMat<T>::FullMat(const unsigned& in_size)
    : MetaMat<T>(in_size, in_size, in_size * in_size) {}

#endif

//! @}
