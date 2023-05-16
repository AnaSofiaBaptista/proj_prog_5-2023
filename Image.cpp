#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    width_ = w;
    height_ = h;

    // Guarda na memória as colunas de pixeis.
    pixels_ = new Color*[width_];

    // Itera sobre as colunas.
    for (int i = 0; i < width_; i++)
    {
      // Guarda na memória as linhas de pixeis.
      pixels_[i] = new Color[height_]; 

      // Itera sobre as linhas.
      for (int j = 0; j < height_; j++)
      {
        // Preenche a imagem com a cor especificada.
        pixels_[i][j] = fill; 
      }
    }
  }

  Image::~Image()
  {
    for (int i = 0; i < width_; i++)
    {
      // Liberta a memória alocada para as linhas de pixeis.
      delete[] pixels_[i];
    }

    // Liberta a memória alocada para as colunas de pixeis.
    delete[] pixels_;
  }

  int Image::width() const
  {
    // Retorna a largura da imagem.
    return width_;
  }

  int Image::height() const
  {
    // Retorna a altura da imagem.
    return height_;
  }

  Color& Image::at(int x, int y)
  {
    // Retorna uma referência à cor do pixel na posição (x,y), permitindo que seja modificada.
    return pixels_[x][y];
  }

  const Color& Image::at(int x, int y) const
  {
    // Retorna uma referência constante à cor do pixel na posição (x,y), permitindo apenas leitura.
    return pixels_[x][y];
  }

}
