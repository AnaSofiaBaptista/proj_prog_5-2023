#ifndef __prog_Script_hpp__
#define __prog_Script_hpp__

#include <string>
#include <fstream>
#include "Image.hpp"

namespace prog
{
  class Script
  {
  public: 
    Script(const std::string &filename);
    ~Script();
    void run();
  private:
    // Current image.
    Image *image;
    // Input stream for reading script commands.
    std::ifstream input;
  private:
    // Private functions
    void clear_image_if_any();
    void open();
    void blank();
    void save();
    void invert();
    void to_gray_scale();
    void replace();
    void replace(rgb_value r1, rgb_value g1, rgb_value b1, rgb_value r2, rgb_value g2, rgb_value b2);
    void fill(rgb_value r1, rgb_value g1, rgb_value b1, int w, int h, int x, int y);
    void h_mirror();
    void v_mirror();
    void add(const std::string &filename, rgb_value r, rgb_value g, rgb_value b, int x, int y);
  };
}
#endif
