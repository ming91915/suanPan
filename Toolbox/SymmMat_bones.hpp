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

    template <typename T1, typename op_type> SymmMat(const SmOp<T1, op_type>& X);

    template <typename T1, typename op_type>
    SymmMat& operator=(const SmOp<T1, op_type>& X);

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

    void print() const;
};

template <typename eT> SymmMat<eT> operator*(const eT& X, const SymmMat<eT>& A)
{
    A *= X;
    return A;
}

template <typename eT> Col<eT> operator*(const SymmMat<eT>& A, const Col<eT>& X)
{
    return sp_mv(A, X);
}

template <typename eT> Col<eT> sp_mv(const SymmMat<eT>& A, const Col<eT>& X)
{
    auto Y = X;

    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_size);
    eT ALPHA = 1.;
    auto INC = 1;
    eT BETA = 0.;

    const auto APTR = const_cast<eT*>(A.memptr());
    const auto XPTR = const_cast<eT*>(X.memptr());

    if(is_float<eT>::value) {
        using T = float;
        suanpan::sspmv_(&UPLO, &N, (T*)&ALPHA, (T*)APTR, (T*)XPTR, &INC, (T*)&BETA,
            (T*)Y.memptr(), &INC);
    } else if(is_double<eT>::value) {
        using T = double;
        suanpan::dspmv_(&UPLO, &N, (T*)&ALPHA, (T*)APTR, (T*)XPTR, &INC, (T*)&BETA,
            (T*)Y.memptr(), &INC);
    }

    return Y;
}

template <typename eT> int sp_inv(SymmMat<eT>& A)
{
    auto UPLO = 'L';
    auto N = static_cast<int>(A.n_size);
    const auto IPIV = new int[N];
    auto INFO = 0;

    if(is_float<eT>::value) {
        using T = float;
        suanpan::ssptrf_(&UPLO, &N, (T*)A.memptr(), IPIV, &INFO);
    } else if(is_double<eT>::value) {
        using T = double;
        suanpan::dsptrf_(&UPLO, &N, (T*)A.memptr(), IPIV, &INFO);
    }

    if(INFO != 0) return INFO;

    const auto WORK = new eT[N];

    if(is_float<eT>::value) {
        using T = float;
        suanpan::ssptri_(&UPLO, &N, (T*)A.memptr(), IPIV, (T*)WORK, &INFO);
    } else if(is_double<eT>::value) {
        using T = double;
        suanpan::dsptri_(&UPLO, &N, (T*)A.memptr(), IPIV, (T*)WORK, &INFO);
    }

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

    if(is_float<eT>::value) {
        using T = float;
        suanpan::sspsv_(
            &UPLO, &N, &NRHS, (T*)A.memptr(), IPIV, (T*)X.memptr(), &LDB, &INFO);
    } else if(is_double<eT>::value) {
        using T = double;
        suanpan::dspsv_(
            &UPLO, &N, &NRHS, (T*)A.memptr(), IPIV, (T*)X.memptr(), &LDB, &INFO);
    }

    delete[] IPIV;

    return INFO;
}
