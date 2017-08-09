#pragma once

template <typename eT>
SymmMat<eT>::SymmMat()
    : Mat<eT>()
{
}

template <typename eT>
SymmMat<eT>::SymmMat(const uword in_rows, const uword in_cols)
    : Mat<eT>()
{
    access::rw(Mat<eT>::n_rows) = in_rows;
    access::rw(Mat<eT>::n_cols) = in_cols;
    access::rw(Mat<eT>::n_elem) = (in_rows + 1) * in_cols / 2;
    init_cold();
}

template <typename eT>
template <typename fill_type>
SymmMat<eT>::SymmMat(const uword in_rows,
    const uword in_cols,
    const fill::fill_class<fill_type>& f)
    : Mat<eT>()
{
    access::rw(Mat<eT>::n_rows) = in_rows;
    access::rw(Mat<eT>::n_cols) = in_cols;
    access::rw(Mat<eT>::n_elem) = (in_rows + 1) * in_cols / 2;
    init_cold();
    (*this).fill(f);
}

template <typename eT> eT& SymmMat<eT>::at(const uword in_row, const uword in_col)
{
    if(in_row > in_col)
        return access::rw(Mat<eT>::mem[in_col * Mat<eT>::n_rows -
            (in_col + in_col * in_col) / 2 + in_row]);

    return access::rw(
        Mat<eT>::mem[in_row * Mat<eT>::n_rows - (in_row + in_row * in_row) / 2 + in_col]);
}

template <typename eT>
const eT& SymmMat<eT>::at(const uword in_row, const uword in_col) const
{
    if(in_row > in_col)
        return Mat<eT>::mem[in_col * Mat<eT>::n_rows - (in_col + in_col * in_col) / 2 +
            in_row];

    return Mat<eT>::mem[in_row * Mat<eT>::n_rows - (in_row + in_row * in_row) / 2 +
        in_col];
}

template <typename eT> eT& SymmMat<eT>::operator()(const uword in_row, const uword in_col)
{
    arma_debug_check(((in_row >= Mat<eT>::n_rows) || (in_col >= Mat<eT>::n_cols)),
        "Mat::operator(): index out of bounds");
    return at(in_row, in_col);
}

template <typename eT>
const eT& SymmMat<eT>::operator()(const uword in_row, const uword in_col) const
{
    arma_debug_check(((in_row >= Mat<eT>::n_rows) || (in_col >= Mat<eT>::n_cols)),
        "Mat::operator(): index out of bounds");
    return at(in_row, in_col);
}

template <typename eT> void SymmMat<eT>::init_cold()
{
    arma_extra_debug_sigprint(
        arma_str::format("n_rows = %d, n_cols = %d") % Mat<eT>::n_rows % Mat<eT>::n_cols);

#if(defined(ARMA_USE_CXX11) || defined(ARMA_64BIT_WORD))
    auto error_message = "Mat::init(): requested size is too large";
#else
    const char* error_message = "Mat::init(): requested size is too large; suggest to "
                                "compile in C++11 mode or enable ARMA_64BIT_WORD";
#endif

    arma_debug_check(
        Mat<eT>::n_rows > ARMA_MAX_UHWORD || Mat<eT>::n_cols > ARMA_MAX_UHWORD ?
            double(Mat<eT>::n_elem) > double(ARMA_MAX_UWORD) :
            false,
        error_message);

    if(Mat<eT>::n_elem <= arma_config::mat_prealloc)
        if(Mat<eT>::n_elem == 0)
            access::rw(Mat<eT>::mem) = NULL;
        else {
            arma_extra_debug_print("Mat::init(): using local memory");
            access::rw(Mat<eT>::mem) = Mat<eT>::mem_local;
        }
    else {
        arma_extra_debug_print("Mat::init(): acquiring memory");
        access::rw(Mat<eT>::mem) = memory::acquire<eT>(Mat<eT>::n_elem);
    }
}

template <typename eT> void SymmMat<eT>::init_warm(uword in_n_rows, uword in_n_cols)
{
    arma_extra_debug_sigprint(
        arma_str::format("in_n_rows = %d, in_n_cols = %d") % in_n_rows % in_n_cols);

    if((Mat<eT>::n_rows == in_n_rows) && (Mat<eT>::n_cols == in_n_cols)) return;

    auto err_state = false;
    char* err_msg = nullptr;

    const uhword t_mem_state = Mat<eT>::mem_state;

    arma_debug_set_error(err_state, err_msg, t_mem_state == 3,
        "Mat::init(): size is fixed and hence cannot be changed");

#if(defined(ARMA_USE_CXX11) || defined(ARMA_64BIT_WORD))
    auto error_message = "Mat::init(): requested size is too large";
#else
    const char* error_message = "Mat::init(): requested size is too large; suggest to "
                                "compile in C++11 mode or enable ARMA_64BIT_WORD";
#endif

    arma_debug_set_error(err_state, err_msg,
        in_n_rows > ARMA_MAX_UHWORD || in_n_cols > ARMA_MAX_UHWORD ?
            .5 * (double(in_n_rows) + 1.) * double(in_n_cols) > double(ARMA_MAX_UWORD) :
            false,
        error_message);

    arma_debug_check(err_state, err_msg);

    const auto old_n_elem = Mat<eT>::n_elem;
    const auto new_n_elem = (in_n_rows + 1) * in_n_cols / 2;

    if(old_n_elem == new_n_elem) {
        arma_extra_debug_print("Mat::init(): reusing memory");
        access::rw(Mat<eT>::n_rows) = in_n_rows;
        access::rw(Mat<eT>::n_cols) = in_n_cols;
    } else // condition: old_n_elem != new_n_elem
    {
        arma_debug_check(t_mem_state == 2,
            "Mat::init(): mismatch between size of auxiliary memory and requested size");

        if(new_n_elem < old_n_elem) // reuse existing memory if possible
        {
            if(t_mem_state == 0 && new_n_elem <= arma_config::mat_prealloc) {
                if(old_n_elem > arma_config::mat_prealloc) {
                    arma_extra_debug_print("Mat::init(): releasing memory");
                    memory::release(access::rw(Mat<eT>::mem));
                }

                if(new_n_elem == 0)
                    access::rw(Mat<eT>::mem) = NULL;
                else {
                    arma_extra_debug_print("Mat::init(): using local memory");
                    access::rw(Mat<eT>::mem) = Mat<eT>::mem_local;
                }
            } else {
                arma_extra_debug_print("Mat::init(): reusing memory");
            }
        } else // condition: new_n_elem > old_n_elem
        {
            if(t_mem_state == 0 && old_n_elem > arma_config::mat_prealloc) {
                arma_extra_debug_print("Mat::init(): releasing memory");
                memory::release(access::rw(Mat<eT>::mem));
            }

            if(new_n_elem <= arma_config::mat_prealloc) {
                arma_extra_debug_print("Mat::init(): using local memory");
                access::rw(Mat<eT>::mem) = Mat<eT>::mem_local;
            } else {
                arma_extra_debug_print("Mat::init(): acquiring memory");
                access::rw(Mat<eT>::mem) = memory::acquire<eT>(new_n_elem);
            }

            access::rw(Mat<eT>::mem_state) = 0;
        }

        access::rw(Mat<eT>::n_rows) = in_n_rows;
        access::rw(Mat<eT>::n_cols) = in_n_cols;
        access::rw(Mat<eT>::n_elem) = new_n_elem;
    }
}
