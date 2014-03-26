// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    24.03.2014 22:12:10 CET
// File:    performance.hpp

#ifndef __PERFORMACE_HEADER
#define __PERFORMACE_HEADER

#include <chrono>
#include "debug.hpp"

namespace addon {
    
    namespace detail {
        struct tsc_counter {
            double cycle() const {
                return (uint64_t(hi) << 32) + lo;
            };
            void clear() {
                hi = 0;
                lo = 0;
            }
            uint32_t hi;
            uint32_t lo;
        };

        double operator-(tsc_counter const & end, tsc_counter const & begin) {
            return end.cycle() - begin.cycle();
        }
        
    }//end namespace detail

    
    class performance_class {
        using chrono_clock = std::chrono::high_resolution_clock;
    public:
        performance_class() {
            clear();
        }
        //------------------- methods -------------------
        void start() {
            start_time_ = chrono_clock::now();
            __asm __volatile__ ("cpuid" : : "a" (0) : "bx", "cx", "dx" ); //CPUID
            __asm__ __volatile__ ("rdtsc" : "=a" ((start_cyc_).lo), "=d"((start_cyc_).hi)); //RDTSC
        }
        void stop() {
            __asm__ __volatile__ ("rdtsc" : "=a" ((end_cyc_).lo), "=d"((end_cyc_).hi)); //RDTSC
            __asm__ __volatile__ ("cpuid" : : "a" (0) : "bx", "cx", "dx" ); //CPUID
            end_time_ = chrono_clock::now();
        }
        void clear() {
            start_cyc_.clear();
            end_cyc_.clear();
            start_time_ = chrono_clock::now();
            end_time_ = chrono_clock::now();
        }
        //------------------- const methods -------------------
        void print() const {
            std::cout << "cycles measured with rdtsc:      " << cycles() << " cycles" << std::endl;
            std::cout << "time measured with hi res clock: " << sec() << " seconds" << std::endl;
        }
        
        double cycles() const {
            auto cyc = end_cyc_ - start_cyc_;
            if(cyc < 1e9)
                WARNING("measure more cycles (>1e9)");
            return end_cyc_ - start_cyc_;
        }
        double sec() const {
            using namespace std::chrono;
            duration<double, std::ratio<1, 1>> res = duration_cast<duration<double>>(end_time_ - start_time_);
            return res.count();
        }
        double msec() const {
            using namespace std::chrono;
            duration<double, std::ratio<1, 1000>> res = duration_cast<duration<double>>(end_time_ - start_time_);
            return res.count();
        }
        double usec() const {
            using namespace std::chrono;
            duration<double, std::ratio<1, 1000000>> res = duration_cast<duration<double>>(end_time_ - start_time_);
            return res.count();
        }
    private:
        detail::tsc_counter start_cyc_, end_cyc_;
        chrono_clock::time_point start_time_, end_time_;
    } clock; //<- one instance named clock
    //-------------- stream operator (only works for std::cout...) --------------
    std::ostream & operator<<(std::ostream & os, performance_class const & arg) {
        arg.print();
        return os;
    }
    
}//end namespace addon

#endif //__PERFORMACE_HEADER
