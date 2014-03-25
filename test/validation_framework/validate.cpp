// Author:  Dominik Gresch <greschd@phys.ethz.ch>
// Date:    25.03.2014 03:00:33 CET
// File:    validate.cpp

#include <iostream>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/int.hpp>

struct foo_wrap {
    template<int N, int A = 2>
    class impl {
    public:
        void sim() {
        }
        int val() const {
            return N;
        }
        void print() const {
            std::cout << "foo " << N << std::endl;
        }
        alignas(A) double m[N];
    };
};

struct bar_wrap {
    template<int N, int A = 2>
    class impl {
    public:
        void sim() {
        }
        int val() const {
            return N;
        }
        void print() const {
            std::cout << "bar " << N << std::endl;
        }
        alignas(A) double m[N];
    };
};

using namespace boost::mpl;
typedef vector<int_<1>, int_<2>, int_<4>>::type vec_type;

template<typename T, typename S>
struct compare {
    template< typename U > void operator()(U) {
        typename T::template impl<U::value, U::value, U::value, 4, > t;
        typename S::template impl<U::value, U::value, U::value, 4, > s;
        t.print();
        s.print();
    }
};

int main(int argc, char* argv[]) {
    
    foo_wrap::impl<2> f;
    bar_wrap::impl<3> b;
    
    
    for_each<vec_type>(compare<foo_wrap, bar_wrap>());
    
    return 0;
}
