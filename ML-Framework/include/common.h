#ifndef COMMON_H
#define COMMON_H

#define STRINGIFY(v) _TO_STRING(v)
#define _TO_STRING(v) #v

#ifndef DEVELOPMENT_MODE
#define DEVELOPMENT_MODE 0
#endif

#define raise_ml_exception(msg) \
do { \
    if (DEVELOPMENT_MODE) \
    { \
        throw ml_exception(msg "(" __FILE__ ":" STRINGIFY(__LINE__) ")"); \
    } \
    else \
    { \
        throw ml_exception(msg); \
    } \
} while(0)

#endif
