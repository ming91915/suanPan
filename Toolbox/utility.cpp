#include "utility.h"
#include <cstring>
#include <suanPan.h>

bool if_equal(const string& A, const char* B) { return _strcmpi(A.c_str(), B) == 0; }

bool if_equal(const char* A, const char* B) { return _strcmpi(A, B) == 0; }

bool if_true(const string& S) { return if_true(S.c_str()); }

bool if_false(const string& S) { return if_false(S.c_str()); }

bool if_true(const char* S) { return if_equal(S, "On") || if_equal(S, "True") || if_equal(S, "1") || if_equal(S, "Yes"); }

bool if_false(const char* S) { return if_equal(S, "Off") || if_equal(S, "False") || if_equal(S, "0") || if_equal(S, "No"); }
