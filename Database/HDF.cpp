#include "HDF.h"
#include <hdf5.h>

HDF::HDF() {}

int HDF::save(const char* N) {
    auto& D = getDomain();

    const auto file_id = H5Fcreate(N, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    return H5Fclose(file_id);
}
