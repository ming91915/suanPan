
#ifndef FULLMAT_HPP
#define FULLMAT_HPP

template <typename T>
class FullMat : public MetaMat<T> {
public:
    explicit FullMat(const unsigned& in_size)
        : MetaMat<T>(in_size, in_size, in_size * in_size) {}
};

#endif
