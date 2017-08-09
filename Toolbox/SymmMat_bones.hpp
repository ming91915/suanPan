#pragma once

template <typename eT> class SymmMat : public Mat<eT>
{
public:
    typedef eT elem_type; //!< the type of elements stored in the matrix
    typedef typename get_pod_type<eT>::result
        pod_type; //!< if eT is std::complex<T>, pod_type is T; otherwise pod_type is eT

    SymmMat();

    explicit SymmMat(const uword in_rows, const uword in_cols);

    template <typename fill_type>
    SymmMat(const uword in_rows,
        const uword in_cols,
        const fill::fill_class<fill_type>& f);

    arma_warn_unused eT& at(const uword in_row, const uword in_col);
    arma_warn_unused const eT& at(const uword in_row, const uword in_col) const;
    arma_warn_unused eT& operator()(const uword in_row, const uword in_col);
    arma_warn_unused const eT& operator()(const uword in_row, const uword in_col) const;

protected:
    void init_cold();

    void init_warm(uword in_rows, uword in_cols);
};
