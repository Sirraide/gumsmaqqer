#ifndef GUMSMAQQER_UTILS_H
#define GUMSMAQQER_UTILS_H

#define STR(what)  #what
#define CAT_(X, Y) X##Y
#define CAT(X, Y)  CAT_(X, Y)

#define repeat(N_) for(size_t iter__ = 0, N__ = N_; iter__ < N__; iter__++)

#endif // GUMSMAQQER_UTILS_H
