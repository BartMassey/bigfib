#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Type of a bucket. Each bucket contains a piece of the
   integer, in least-significant to most-significant
   order. */
typedef uint64_t bucket_t;

struct bigint {
    int nbuckets;
    bucket_t *buckets;
};

/* Returns a new bigint structure with nbuckets
   buckets. Buckets are uninitialized */
struct bigint *bigint_new(int nbuckets) {
    struct bigint *b = malloc(sizeof *b);
    assert(b);
    b->buckets = malloc(nbuckets * sizeof(bucket_t));
    assert(b->buckets);
    b->nbuckets = nbuckets;
    return b;
}

bucket_t hex_digit_value(char d) {
    if (d >= '0' && d <= '9')
        return d - '0';
    d = tolower(d);
    assert(d >= 'a' && d <= 'f');
    return d - 'a' + 10;
}

void bigint_free(struct bigint *b) {
    free(b->buckets);
    free(b);
}

/* Read a bigint from the given hex string (with no leading "0x").
   Returns null on incorrectly formatted input.*/
struct bigint *bigint_read(char *s) {
    int ns = strlen(s);
    /* Empty string is invalid. */
    if (ns == 0)
        return 0;
    /* Number of digits per bucket. */
    int ds_per_b = 2 * sizeof(bucket_t);
    /* 0..16 digits = 1 bucket; 17-32 digits = 2 buckets ... */
    int nb = (ns + ds_per_b - 1) / ds_per_b;
    struct bigint *b = bigint_new(nb);
    /* Index of current bucket. */
    int di = 0;
    for (int i = 0; i < nb; i++) {
        bucket_t d = 0L;
        for (int j = di; j < di + ds_per_b; j++) {
            if (j >= ns) {
                assert(i == nb - 1);
                break;
            }
            char c = s[j];
            if (!isxdigit(c)) {
                bigint_free(b);
                return 0;
            }
            bucket_t k = hex_digit_value(c);
            d = (d << 4) | k;
        }
        b->buckets[i] = d;
        di += ds_per_b;
    }
    return b;
}

void bigint_print(struct bigint *b) {
    for (int i = b->nbuckets - 1; i >= 0; --i)
        printf("%016lx", b->buckets[i]);
    printf("\n");
}

int main(int argc, char **argv) {
    assert(argc == 2);
    struct bigint *b = bigint_read(argv[1]);
    assert(b);
    bigint_print(b);
    return 0;
}