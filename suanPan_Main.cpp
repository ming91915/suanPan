#include <suanPan>

int main(int argc, char** argv) {
    wall_clock T;
    T.tic();

    argument_parser(argc, argv);

    cout << "Finished: " << T.toc() << "\n";
    return 0;
}
