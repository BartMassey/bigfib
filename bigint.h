#include <stdint.h>

/* Type of a bucket. Each bucket contains a piece of the
   integer, in least-significant to most-significant
   order. This type can be any unsigned integer type. */
typedef uint64_t bucket_t;

struct bigint {
    int nbuckets;
    bucket_t *buckets;
};

extern struct bigint *bigint_new(int nbuckets);
extern void bigint_free(struct bigint *b);
extern struct bigint *bigint_read(char *s);
extern void bigint_print(struct bigint *b);
struct bigint *bigint_add(struct bigint *b1, struct bigint *b2);
