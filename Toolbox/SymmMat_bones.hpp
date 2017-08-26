#pragma once

template <typename eT> class SymmMat : public Mat<eT>
{
public:
    typedef eT elem_type; //!< the type of elements stored in the matrix
    typedef typename get_pod_type<eT>::result
        pod_type; //!< if eT is std::complex<T>, pod_type is T; otherwise pod_type is eT

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

    auto APTR = const_cast<double*>(A.memptr());
    auto XPTR = const_cast<double*>(X.memptr());

    suanPan::dspmv_(&UPLO, &N, &ALPHA, APTR, XPTR, &INC, &BETA, Y.memptr(), &INC);

    return Y;
}

inline vec operator*(const symm_mat& A, const vec& X) { return spmv(A, X); }

template <typename eT> struct is_Mat<SymmMat<eT>> {
    static const bool value = false;
};

// template <typename T> struct is_SymmMat {
//	static const bool value = false;
//};
//
// template <typename eT> struct is_SymmMat<SymmMat<eT>> {
//	static const bool value = true;
//};
//
// class glue_spmv_times
//{
// public:
//	template <typename T1, typename T2>
//	arma_hot static void apply(Mat<typename T1::elem_type>& out,
//		const Glue<T1, T2, glue_spmv_times>& X)
//	{
//		arma_extra_debug_sigprint();
//
//		out = spmv(X.A, X.B);
//	}
//};
//
// template <typename T1, typename T2> typename enable_if2<
//	is_SymmMat<T1>::value && is_Col<T2>::value &&
//	is_same_type<typename T1::elem_type, typename T2::elem_type>::value,
//	const Glue<T1, T2, glue_spmv_times> >::result
//	operator*(const T1& X, const T2& Y)
//{
//	arma_extra_debug_sigprint();
//
//	return Glue<T1, T2, glue_spmv_times>(X, Y);
//}
