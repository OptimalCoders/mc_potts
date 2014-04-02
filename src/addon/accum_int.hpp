// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    02.04.2014 22:29:33 CEST
// File:    accum_int.hpp

#ifndef __ACCUM_INT_HEADER
#define __ACCUM_INT_HEADER

#include <iostream>
#include <cmath>

using namespace std;

class accumulator_int {
    public:
        accumulator_int(): n_(0), sum_(0), sum2_(0) {};
        inline void operator<<(uint64_t const & val) {
            sum_ += val;
            sum2_ += val * val;
            ++n_;
        }
        inline double mean() const {
            return double(sum_) / n_;
        }
        inline double deviation() const {
            return sqrt(double(sum2_) / (n_ - 1) - sum_ * double(sum_) / n_ / (n_ - 1.0));
        }
        inline double error() const {
            return deviation() / sqrt(n_);
        }
        inline uint64_t const & count() const {
            return n_;
        }
        inline void clear() {
            n_ = 0;
            sum_ = 0;
            sum2_ = 0;
        }
    private:
        uint64_t n_;
        int64_t sum_;
        int64_t sum2_;
};

ostream& operator<<(ostream& os, const accumulator_int &d) {
    os << d.mean() << " +/- " << d.error();
    return os;
}
#endif //__ACCUM_INT_HEADER
