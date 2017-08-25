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
