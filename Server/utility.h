#ifndef UTILITY_H
#define UTILITY_H

#undef  MAX
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#undef  MIN
#define MIN(x,y) ((x) > (y) ? (y) : (x))

#define ZERO    1e-10
#define IS_ZERO(A) ( (A < ZERO) && (A > -ZERO) )
#define IS_SAME(A, B) ( ((A-B) < ZERO) && ((A-B) > -ZERO) )
#define IS_BETWEEN(A, B, C) ( ((A-B) > -ZERO) && ((A-C) < ZERO) )

#define SERVER_REVIEW_TIME_MS       1000
#define BUF_SIZE                    512
#define PORT                        3333

#endif // UTILITY_H
