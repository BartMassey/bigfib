#include <assert.h>
#include "bigint.h"

int main(int argc, char **argv) {
    assert(argc == 2);
    struct bigint *b = bigint_read(argv[1]);
    assert(b);
    bigint_print(b);
    return 0;
}
