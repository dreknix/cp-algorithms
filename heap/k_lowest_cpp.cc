#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << "n k\n";
        return EXIT_FAILURE;
    }

    int64_t n = atol(argv[1]);
    if ((n < 1) || (n > 2e10)) {
        std::cerr << "error: n out of range [1..2*10^10]\n";
        return EXIT_FAILURE;
    }
    int32_t k = atol(argv[2]);
    if ((k < 1) || (k > 2e9)) {
        std::cerr << "error: k out of range [1..2*10^9]\n";
        return EXIT_FAILURE;
    }

    srand((unsigned)time(NULL));

    std::priority_queue<int64_t, std::vector<int64_t>, std::less<int64_t>> heap;

    for (int64_t x = 0; x < n; x++) {
        int64_t e = rand() % 100000;
        if (heap.size() == k) {
            if (heap.top() > e) {
                heap.pop();
                heap.push(e);
            }
        } else {
            heap.push(e);
        }
        // std::cout << "-> inserting " << e << ": " << heap.top() << '\n';
    }

    std::cout << "sorted content of heap(" << heap.size() << "/"
              << k << ") after " << n << " insertions:\n";
    while (heap.size() > 0) {
        std::cout << heap.top() << ' ';
        heap.pop();
    }
    std::cout << '\n';

    return EXIT_SUCCESS;
}
