#pragma once

template <typename T> T sign(const T& I) { return (I > 0.) - (I < 0.); }
