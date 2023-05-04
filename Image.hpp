#ifndef __prog_Image_hpp__
#define __prog_Image_hpp__
#include "Color.hpp"

namespace prog
{
  class Image
  {
  private:
    int width_;
    int height_;
    Color **pixels_;     //guardar matriz de pixeis na memória
    
  public:
    Image(int w, int h, const Color &fill = {255, 255, 255});
    ~Image();
    int width() const;
    int height() const;
    Color &at(int x, int y);
    const Color &at(int x, int y) const;
  };
}
#endif
