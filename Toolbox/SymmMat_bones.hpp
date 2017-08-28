#pragma once

template <typename eT> class SymmMat : public Base<eT, SymmMat<eT>>
{
public:
    typedef eT elem_type;
    typedef typename get_pod_type<eT>::result pod_type;

    static const bool is_row = false;
    static const bool is_col = false;

    const uword n_size;
    const uword n_elem;
    const uhword mem_state;

    arma_aligned const eT* const mem;

    arma_align_mem eT mem_local[arma_config::mat_prealloc];

    ~SymmMat();
    SymmMat();

    explicit SymmMat(const uword& in_size);
    explicit SymmMat(const SizeMat& s);

    template <typename fill_type>
    SymmMat(const uword& in_size, const fill::fill_class<fill_type>& f);
    template <typename fill_type>
    SymmMat(const SizeMat& s, const fill::fill_class<fill_type>& f);

    SymmMat(const Mat<eT>& in_mat);

    SymmMat& operator=(const eT& val);
    SymmMat& operator+=(const eT& val);
    SymmMat& operator-=(const eT& val);
    SymmMat& operator*=(const eT& val);
    SymmMat& operator/=(const eT& val);

    SymmMat(const SymmMat& m);
    SymmMat& operator=(const SymmMat& m);
    SymmMat& operator+=(const SymmMat& m);
    SymmMat& operator-=(const SymmMat& m);
    SymmMat& operator*=(const SymmMat& m) = delete;
    SymmMat& operator%=(const SymmMat& m);
    SymmMat& operator/=(const SymmMat& m);

    template <typename T1, typename op_type> SymmMat(const Op<T1, op_type>& X);

    template <typename T1, typename op_type> SymmMat& operator=(const Op<T1, op_type>& X);

    eT& at(const uword& in_row, const uword& in_col);
    const eT& at(const uword& in_row, const uword& in_col) const;
    eT& operator()(const uword& in_row, const uword& in_col);
    const eT& operator()(const uword& in_row, const uword& in_col) const;

    eT* memptr();
    const eT* memptr() const;

    void set_size(const uword in_size);

    arma_hot const SymmMat& fill(const eT val);
    template <typename fill_type>
    arma_hot const SymmMat& fill(const fill::fill_class<fill_type>&);

    const SymmMat& zeros();
    const SymmMat& zeros(const uword in_size);

    const SymmMat& ones();
    const SymmMat& ones(const uword in_size);

    const SymmMat& eye();
    const SymmMat& eye(const uword in_size);

    void reset();

    void init_cold();
    void init_warm(const uword& in_size);
};

namespace arma
{
template <typename T> struct is_SymmMat {
    static const bool value = false;
};
template <typename eT> struct is_SymmMat<SymmMat<eT>> {
    static const bool value = true;
};
template <typename eT> struct is_Mat<SymmMat<eT>> {
    static const bool value = false;
};
}

class op_inv_sp
{
public:
    template <typename T1>
    static void apply(SymmMat<typename T1::elem_type>& out, const Op<T1, op_inv_sp>& in)
    {
        arma_extra_debug_sigprint();

        out = in.m;

        if(sp_inv(out) != 0) {
            out.reset();
            arma_stop_runtime_error("sp_inv(): matrix seems singular");
        }
    }
};

template <typename T1>
arma_warn_unused
    typename enable_if2<is_SymmMat<T1>::value, const Op<T1, op_inv_sp>>::result
    inv_sp(const Base<typename T1::elem_type, T1>& X)
{
    arma_extra_debug_sigprint();

    return Op<T1, op_inv_sp>(X.get_ref());
}

template <typename eT> SymmMat<eT> operator*(const eT& X, const SymmMat<eT>& A)
{
    return A *= X;
}

template <typename eT> Col<eT> operator*(const SymmMat<eT>& A, const Col<eT>& X)
{
    return sp_mv(A, X);
}

template <typename eT1, typename eT2>
arma_hot void
arma_assert_same_size(const SymmMat<eT1>& A, const SymmMat<eT2>& B, const char* x)
{
    const uword A_n_size = A.n_size;
    const uword B_n_size = B.n_size;

    if(A_n_size != B_n_size)
        arma_stop_logic_error(
            arma_incompat_size_string(A_n_size, A_n_size, B_n_size, B_n_size, x));
}

template <typename eT> Col<eT> sp_mv(const SymmMat<eT>& A, const Col<eT>& X)
{
    auto Y = X;

    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_size);
    auto ALPHA = 1.;
    auto INC = 1;
    auto BETA = 0.;

    const auto APTR = const_cast<eT*>(A.memptr());
    const auto XPTR = const_cast<eT*>(X.memptr());

    suanpan::dspmv_(&UPLO, &N, &ALPHA, APTR, XPTR, &INC, &BETA, Y.memptr(), &INC);

    return Y;
}

template <typename eT> int sp_inv(SymmMat<eT>& A)
{
    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_size);
    const auto IPIV = new int[N];
    auto INFO = 0;

    suanpan::dsptrf_(&UPLO, &N, A.memptr(), IPIV, &INFO);

    if(INFO != 0) return INFO;

    const auto WORK = new double[N];

    suanpan::dsptri_(&UPLO, &N, A.memptr(), IPIV, WORK, &INFO);

    delete[] WORK;
    delete[] IPIV;

    return INFO;
}

template <typename eT> int sp_solve(Col<eT>& X, SymmMat<eT>& A, const Col<eT>& B)
{
    X = B;

    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_size);
    auto NRHS = 1;
    const auto IPIV = new int[N];
    auto LDB = N;
    auto INFO = 0;

    suanpan::dspsv_(&UPLO, &N, &NRHS, A.memptr(), IPIV, X.memptr(), &LDB, &INFO);

    delete[] IPIV;

    return INFO;
}
