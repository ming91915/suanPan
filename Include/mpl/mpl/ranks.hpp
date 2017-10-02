#if !(defined MPL_RANKS_HPP)

#define MPL_RANKS_HPP

#include <cstddef>
#include <vector>
#include <utility>

namespace mpl {

class ranks : private std::vector<int> {
    typedef std::vector<int> base;

public:
    typedef base::size_type size_type;
    typedef base::value_type value_type;
    typedef base::iterator iterator;
    typedef base::const_iterator const_iterator;
    explicit ranks(size_type n = 0)
        : base(n, 0) {
    }
    ranks(std::initializer_list<int> init)
        : base(init) {
    }
    ranks(const ranks& other)
        : base(other) {
    }
    ranks(ranks&& other)
        : base(std::move(other)) {
    }
    using base::operator=;
    using base::begin;
    using base::cbegin;
    using base::cend;
    using base::end;
    using base::operator[];
    using base::push_back;
    using base::size;
    const int* operator()() const {
        return base::data();
    }
    int* operator()() {
        return base::data();
    }
};
}

#endif
