#ifndef ACC_DOUBLE_HEADER
#define ACC_DOUBLE_HEADER

#include <iostream>
#include <cmath>

using namespace std;

class accumulator_double {
    public:
        accumulator_double(): n_(0), sum_(0), sum2_(0) {};
        inline void operator<<(double const & val) {
            sum_ += val;
            sum2_ += val * val;
            ++n_;
        }
        inline double mean() const {
            return sum_ / n_;
        }
        inline double deviation() const {
            return sqrt(sum2_ / (n_ - 1) - sum_ *sum_ / n_ / (n_ - 1));
        }
        inline double error() const {
            return deviation() / sqrt(n_);
        }
        inline uint64_t const & count() const {
            return n_;
        }
        inline void reset() {
            n_ = 0;
            sum_ = 0;
            sum2_ = 0;
        }
    private:
        uint64_t n_;
        double sum_;
        double sum2_;
};

ostream& operator<<(ostream& os, const accumulator_double &d) {
    os << d.mean() << " +/- " << d.error();
    return os;
}
#endif
