// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    29.04.2014 19:39:54 CEST
// File:    micro_bench.hpp

#ifndef __MICRO_BENCH_HEADER
#define __MICRO_BENCH_HEADER

#include "performance.hpp"
#include "accum.hpp"

#include <map>
#include <vector>
#include <set>
#include <tuple>
#include <iomanip>
#include <utility>

namespace addon {
    addon::performance_class clock2; // use another clock in order not to create conflicts with addon::clock
    class timer_class {
    public:
        timer_class() {
            
        }
        void start(std::string const & key) {
            double cyc = addon::clock2.cycles(false);
            addon::clock2.start();
            addon::clock2.stop();
            cyc -= addon::clock2.cycles(false);
            
            for(auto const & k : stack_) {
                std::get<0>(tel_.at(k)) += cyc;
            }
            tel_[key];
            if(std::get<2>(tel_.at(key)) == "") {
                if(stack_.size() == 0) {
                    std::get<2>(tel_.at(key)) = "super_secret_root_name";
                }
                else
                    std::get<2>(tel_.at(key)) = stack_.back();
            }
                
            stack_.push_back(key);
        }
        void stop() {
            double cyc = addon::clock2.cycles(false);
            addon::clock2.start();
            addon::clock2.stop();
            cyc -= addon::clock2.cycles(false);
            
            std::string key = stack_.back();
            for(auto const & k : stack_)
                std::get<0>(tel_.at(k)) += cyc;
            
            std::get<1>(tel_.at(key)) << std::get<0>(tel_.at(key));
            std::get<0>(tel_.at(key)) = 0;
            stack_.pop_back();
        }
        void print() {
            std::set<std::string> parents;
            
            std::cout << GREENB << "Microbenchmarks:" << NONE << std::endl;
            for(auto const & p : tel_) {
                std::cout << YELLOW << std::setw(12) << p.first << NONE << " used " << YELLOWB << std::get<1>(p.second) << NONE << " cycles and was called " << std::get<1>(p.second).count() << " times" << std::endl;
                parents.insert(std::get<2>(p.second));
            }
            
            for(auto const & p : parents) {
                double time = 0;
                if(p == "super_secret_root_name") {
                    std::cout << GREENB << "Performance for all:" << NONE << std::endl;
                    for(auto const & t : tel_) {
                    if(std::get<2>(t.second) == p)
                        time += std::get<1>(t.second).mean() * std::get<1>(t.second).count();
                    }
                }
                else {
                    std::cout << GREENB << "Performance for " << p << ":" << NONE << std::endl;
                    time = std::get<1>(tel_.at(p)).mean() * std::get<1>(tel_.at(p)).count();
                }
                     
                for(auto const & t : tel_) {
                    if(std::get<2>(t.second) == p)
                        std::cout << YELLOW << std::setw(12) << t.first << NONE << " uses " << YELLOWB <<int((std::get<1>(tel_.at(t.first)).mean() * std::get<1>(tel_.at(t.first)).count() / time) * 100) << "%" << NONE << std::endl;
                }
            }
            
        }
        void clear() {
            tel_.clear();
            stack_.clear();
        }
        std::map<std::string, std::pair<int, double>> get(std::string const & p) const {
            std::map<std::string, std::pair<int, double>> res;
            
            double time = 0;
            if(p == "super_secret_root_name") {
                for(auto const & t : tel_) {
                if(std::get<2>(t.second) == p)
                    time += std::get<1>(t.second).mean() * std::get<1>(t.second).count();
                }
            }
            else {
                time = std::get<1>(tel_.at(p)).mean() * std::get<1>(tel_.at(p)).count();
            }
                 
            for(auto const & t : tel_) {
                if(std::get<2>(t.second) == p)
                    res[t.first] = std::make_pair((std::get<1>(tel_.at(t.first)).mean() * std::get<1>(tel_.at(t.first)).count() / time) * 100, std::get<1>(tel_.at(t.first)).mean());
            }
            return res;
        }
    private:
        std::map<std::string, std::tuple<double, accum_class<int64_t>, std::string>> tel_;
        std::vector<std::string> stack_;
    } timer;
}//end namespace addon

#define START_MICRO(name)           \
addon::clock2.stop();               \
addon::timer.start(name);           \
addon::clock2.start();              //
#define STOP_MICRO()                \
addon::clock2.stop();               \
addon::timer.stop();                \
addon::clock2.start();              //
#define NEXT_MICRO(name)            \
STOP_MICRO()                        \
addon::clock2.stop();               \
addon::timer.start(name);           \
addon::clock2.start();              //
#define P_MICRO()                   \
addon::timer.print();               //
#define CLEAR_MICRO()               \
addon::timer.clear();               //
#define RES_MICRO(parent) addon::timer.get(parent)//

#endif //__MICRO_BENCH_HEADER
