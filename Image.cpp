#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    width_ = w;
    height_ = h;

    pixels_ = new Color*[width_];  //guarda na memória a(s) coluna(s) de pixeis

    for (int i = 0; i < width_; i++) {    //itera sobre a(s) coluna(s)
      pixels_[i] = new Color[height_](); 

      for (int j = 0; j < height_; j++) {    //itera sobre a(s) linha(s)
        pixels_[i][j] = fill;
      }
    }
  }

  Image::~Image()
  {
    for (int i = 0; i < width_; i++) {
      delete[] pixels_[i];
    }

    delete[] pixels_;
  }

  int Image::width() const
  {
    return width_;
  }

  int Image::height() const
  {
    return height_;
  }

  Color& Image::at(int x, int y){
    return pixels_[x][y];          //serve para modificar cada pixel
  }

  const Color& Image::at(int x, int y) const{
    return pixels_[x][y];         //apenas para leitura da imagem final
  }

}
