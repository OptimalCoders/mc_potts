// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.03.2014 17:51:26 CET
// File:    ppm_picture.hpp

#ifndef __PPM_PICTURE_HEADER
#define __PPM_PICTURE_HEADER

#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

namespace addon {
    class ppm_picture_class {
        using color_type = std::tuple<uint8_t, uint8_t, uint8_t>;
    public:
        ppm_picture_class(std::string const & outfile = "output"): cellsize_(1)
                                                             , fname_(outfile) {
            init(-1);
        }
        void init(uint const & n) {
            //------------------- used hardcoded colors (works for 8 max) -------------------
            if(n == uint(-1)) {
                color_[0] = color_type(255,   0,   0);
                color_[1] = color_type(  0, 255,   0);
                color_[2] = color_type(  0,   0, 255);
                color_[3] = color_type(255, 255,   0);
                color_[4] = color_type(255,   0, 255);
                color_[5] = color_type(  0, 255, 255);
                color_[6] = color_type(255, 255, 255);
                color_[7] = color_type(  0,   0,   0);
            }
            //------------------- create color gradient -------------------
            else
                if(n%2 == 1) {
                    for(uint i = 0; i < n/2; ++i)
                        color_[i] = color_type(int(255 * i / (double(n) / 2)), 255, 0);
                    
                    color_[n/2] = color_type(255, 255, 0);
                    
                    for(uint i = 0; i < n/2; ++i)
                        color_[n - 1 - i] = color_type(255, int(255 * i / (double(n) / 2)), 0);
                }
                else {
                    for(uint i = 0; i < n/2; ++i)
                        color_[i] = color_type(int(255 * i / (double(n-1) / 2)), 255, 0);
                    
                    for(uint i = 0; i < n/2; ++i)
                        color_[n - 1 - i] = color_type(255, int(255 * i / (double(n-1) / 2)), 0);
                }
        }
        
        template<typename T>
        void print(T const & t, int const & nr = -1) const {
            std::stringstream ss;
            ss << fname_;
            if(nr != -1) {
                ss << "_" << std::setw(4) << std::setfill('0') << nr << std::setfill(' ');
            }
            ss << ".ppm";
            
            std::ofstream out(ss.str().c_str(), std::ios_base::binary);
            out << "P6" << std::endl
            << t[0].size()*cellsize_ << " " << t.size()*cellsize_ << std::endl
            << "255" << std::endl;
            
            for(int i = 0; i < t.size(); ++i)
                for(int ch = 0; ch < cellsize_; ++ch)
                    for(int j = 0; j < t[0].size(); ++j)
                        for(int cw = 0; cw < cellsize_; ++cw) {
                            auto & col = color_.at(t[i][j]);
                            out << std::get<0>(col) << std::get<1>(col) << std::get<2>(col);
                        }
            out.close();
            std::cout << "printed " << ss.str() << std::endl;
        }
        void set_cellsize(uint const & s) {
            cellsize_ = s;
        }
        void set_color(uint const & n) {
            init(n);
        }
    private:
        std::map<int, color_type> color_;
        uint cellsize_;
        std::string fname_;
        
    };
}//end namespace addon

#endif //__PPM_PICTURE_HEADER
