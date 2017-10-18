#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mpl/mpl.hpp>
#include <vector>

// data type to store data an position of global minimum
typedef std::pair<double, int> pair_t;

int main() {
    const mpl::communicator& comm_world = mpl::environment::comm_world();
    // generate data
    std::srand(std::time(0) * comm_world.rank()); // random seed
    const int n = 8;
    // populate vector with random data
    std::vector<pair_t> v(n);
    std::generate(v.begin(), v.end(), [&]() -> auto { return std::make_pair(static_cast<double>(std::rand()) / RAND_MAX, comm_world.rank()); });
    // calculate minium and its location and send result to rank root
    int root = 0;
    mpl::contiguous_layout<pair_t> layout(n);
    if(comm_world.rank() == root) {
        std::vector<pair_t> result(n);
        // calculate minimum
        comm_world.reduce(mpl::min<pair_t>(), root, v.data(), result.data(), layout);
        // display data from all ranks
        std::cout << "arguments:\n";
        for(int r = 0; r < comm_world.size(); ++r) {
            if(r > 0) comm_world.recv(v.data(), layout, r);
            for(auto i : v) std::cout << std::fixed << std::setprecision(5) << i.first << ' ' << i.second << '\t';
            std::cout << '\n';
        }
        // display results of global reduction
        std::cout << "\nresults:\n";
        for(pair_t i : result) std::cout << std::fixed << std::setprecision(5) << i.first << ' ' << i.second << '\t';
        std::cout << '\n';
    } else {
        // calculate minium and its location and send result to rank 0
        comm_world.reduce(mpl::min<pair_t>(), root, v.data(), layout);
        // send data to rank 0 for display
        comm_world.send(v.data(), layout, root);
    }
    return EXIT_SUCCESS;
}
