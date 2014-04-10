// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    10.04.2014 10:40:19 CEST
// File:    accum.hpp

#ifndef __ACCUM_HEADER
#define __ACCUM_HEADER

#include <iostream>
#include <cmath>

template<typename T>
class accum_class {
        using mean_type = decltype((T() + T()) / double());
        using count_type = uint64_t;
    public:
        accum_class(): n_(0), sum_(0), sum2_(0) {};
        inline void operator<<(T const & val) {
            ++n_;
            sum_ += val;
            sum2_ += val * val;
        }
        inline mean_type mean() const {
            return mean_type(sum_) / n_;
        }
        inline mean_type deviation() const {
            mean_type sc0 = n_ - 1;
            return sqrt(sum2_ / sc0 - sum_ * sum_ / sc0 / n_);
        }
        inline mean_type error() const {
            using std::sqrt;
            return deviation() / sqrt(n_);
        }
        inline count_type const & count() const {
            return n_;
        }
        inline void clear() {
            n_ = 0;
            sum_ = 0;
            sum2_ = 0;
        }
    private:
        count_type n_;
        T sum_;
        T sum2_;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const accum_class<T> &d) {
    os << d.mean() << " +/- " << d.error();
    return os;
}
#endif //__ACCUM_HEADER
