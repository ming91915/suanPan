#pragma once

template <typename eT> class SymmMat : public Mat<eT>
{
public:
    typedef eT elem_type;
    typedef typename get_pod_type<eT>::result pod_type;

    SymmMat();

    explicit SymmMat(const uword& in_size);

    template <typename fill_type>
    SymmMat(const uword& in_size, const fill::fill_class<fill_type>& f);

    eT& at(const uword& in_row, const uword& in_col);
    const eT& at(const uword& in_row, const uword& in_col) const;
    eT& operator()(const uword& in_row, const uword& in_col);
    const eT& operator()(const uword& in_row, const uword& in_col) const;

protected:
    void init_cold();
    void init_warm(const uword& in_size);
};

using symm_mat = SymmMat<double>;

inline vec spmv(const symm_mat& A, const vec& X)
{
    auto Y = X;

    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_cols);
    auto ALPHA = 1.;
    auto INC = 1;
    auto BETA = 0.;

    const auto APTR = const_cast<double*>(A.memptr());
    const auto XPTR = const_cast<double*>(X.memptr());

    suanPan::dspmv_(&UPLO, &N, &ALPHA, APTR, XPTR, &INC, &BETA, Y.memptr(), &INC);

    return Y;
}

inline vec operator*(const symm_mat& A, const vec& X) { return spmv(A, X); }

namespace arma
{
template <typename eT> struct is_Mat<SymmMat<eT>> {
    static const bool value = false;
};
}
