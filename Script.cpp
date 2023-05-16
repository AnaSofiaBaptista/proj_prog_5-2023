#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

namespace prog
{
    // Use to read color values from a script file.
    // Usado para ler valores de cores de um ficheiro script.

    istream &operator>>(istream &input, Color &c)
    {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    // Construtor da classe Script.

    Script::Script(const string &filename) : image(nullptr), input(filename)
    {
    }
    void Script::clear_image_if_any()
    {
        // Função usada para limpar a imagem atual se existir.

        if (image != nullptr)
        {
            delete image;
            image = nullptr;
        }
    }

    // Destruidor da classe Script.

    Script::~Script()
    {
        clear_image_if_any();
    }

    void Script::run()
    {
        string command;
        while (input >> command)
        {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open")
            {
                open();
                continue;
            }
            if (command == "blank")
            {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save")
            {
                save();
                continue;
            }
            if (command == "invert")
            {
                invert();
                continue;
            }
            if (command == "to_gray_scale")
            {
                to_gray_scale();
                continue;
            }
            if (command == "replace")
            {
                Color col1, col2;
                input >> col1;
                input >> col2;
                replace(col1.red(), col1.green(), col1.blue(), col2.red(), col2.green(), col2.blue());
                continue;
            }
            if (command == "fill")
            {
                int x, y, w, h;
                input >> x >> y >> w >> h;
                Color col1;
                input >> col1;
                fill(col1.red(), col1.green(), col1.blue(), w, h, x, y);
                continue;
            }
            if (command == "h_mirror")
            {
                h_mirror();
                continue;
            }
            if (command == "v_mirror")
            {
                v_mirror();
                continue;
            }
            if (command == "add")
            {
                string filename;
                input >> filename;
                Color col1;
                input >> col1;
                int x, y;
                input >> x >> y;
                add(filename, col1.red(), col1.green(), col1.blue(), x, y);
                continue;
            }
            if (command == "crop")
            {
                int x, y, w, h;
                input >> x >> y >> w >> h;
                crop(x, y, w, h);
                continue;
            }
            if (command == "rotate_left")
            {
                rotate_left();
                continue;
            }
            if (command == "rotate_right")
            {
                rotate_right();
                continue;
            }
            if (command == "median_filter")
            {
                int ws;
                input >> ws;
                median_filter(ws);
                continue;
            }
        }
    }
    void Script::open()
    {
        // Substitui a imagem atual (se existir) pela imagem lida do ficheiro PNG.

        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank()
    {
        // Substitui a imagem atual (se existir) por uma imagem em branco.

        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save()
    {
        // Guarda a imagem atual num ficheiro PNG.

        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert()
    {
        // Transforma cada pixel (r, g, b) para (255-r, 255-g, 255-b).
        // Inverte as cores.
        
        int width_ = image->width();       // Obtem a largura da imagem.
        int height_ = image->height();     // Obtem a altura da imagem.

        // Percorre cada pixel da imagem
        
        for (int i = 0; i < width_; i++)
        {
            for (int j = 0; j < height_; j++)
            {
                image->at(i, j).red() = 255 - image->at(i, j).red();
                image->at(i, j).green() = 255 - image->at(i, j).green();
                image->at(i, j).blue() = 255 - image->at(i, j).blue();
            }
        }
    }

    void Script::to_gray_scale()
    {
        // Transforma cada pixel (r, g, b) para (v, v, v) tal que v = (r + g + b)/3.
        // Converte a imagem para uma escala de cinza.

        int width_ = image->width();
        int height_ = image->height();

        for (int i = 0; i < width_; i++)
        {
            for (int j = 0; j < height_; j++)
            {
                rgb_value v = (image->at(i, j).red() + image->at(i, j).blue() + image->at(i, j).green()) / 3;
                image->at(i, j).red() = v;
                image->at(i, j).green() = v;
                image->at(i, j).blue() = v;
            }
        }
    }
    void Script::replace(rgb_value r1, rgb_value g1, rgb_value b1, rgb_value r2, rgb_value g2, rgb_value b2)
    {
        // Substitui uma cor por outra na imagem.

        int width_ = image->width();
        int height_ = image->height();

        for (int i = 0; i < width_; i++)
        {
            for (int j = 0; j < height_; j++)
            {
                if (image->at(i, j).red() == r1 && image->at(i, j).green() == g1 && image->at(i, j).blue() == b1)
                {
                    image->at(i, j).red() = r2;
                    image->at(i, j).green() = g2;
                    image->at(i, j).blue() = b2;
                }
            }
        }
    }
    void Script::fill(rgb_value r1, rgb_value g1, rgb_value b1, int w, int h, int x, int y)
    {
        // Preenche uma área retangular da imagem com uma cor.

        int width_ = image->width();
        int height_ = image->height();

        for (int i = x; i < width_ && i < x + w; i++)
        {
            for (int j = y; j < height_ && j < y + h; j++)
            {
                image->at(i, j).red() = r1;
                image->at(i, j).green() = g1;
                image->at(i, j).blue() = b1;
            }
        }
    }
    void Script::h_mirror()
    {
        // Espelha a imagem horizontalmente.

        int width_ = image->width();
        int height_ = image->height();
        int r, g, b;

        for (int i = 0; i < width_ / 2; i++)
        {
            for (int j = 0; j < height_; j++)
            {
                r = image->at(width_ - 1 - i, j).red();
                g = image->at(width_ - 1 - i, j).green();
                b = image->at(width_ - 1 - i, j).blue();
                image->at(width_ - 1 - i, j).red() = image->at(i, j).red();
                image->at(width_ - 1 - i, j).green() = image->at(i, j).green();
                image->at(width_ - 1 - i, j).blue() = image->at(i, j).blue();
                image->at(i, j).red() = r;
                image->at(i, j).green() = g;
                image->at(i, j).blue() = b;
            }
        }
    }
    void Script::v_mirror()
    {
        // Espelha a imagem verticalmente.

        int width_ = image->width();
        int height_ = image->height();
        int r, g, b;

        for (int i = 0; i < width_; i++)
        {
            for (int j = 0; j < height_ / 2; j++)
            {
                r = image->at(i, height_ - 1 - j).red();
                g = image->at(i, height_ - 1 - j).green();
                b = image->at(i, height_ - 1 - j).blue();
                image->at(i, height_ - 1 - j).red() = image->at(i, j).red();
                image->at(i, height_ - 1 - j).green() = image->at(i, j).green();
                image->at(i, height_ - 1 - j).blue() = image->at(i, j).blue();
                image->at(i, j).red() = r;
                image->at(i, j).green() = g;
                image->at(i, j).blue() = b;
            }
        }
    }

    void Script::add(const string &filename, rgb_value r, rgb_value g, rgb_value b, int x, int y)
    {
        // Adiciona outra imagem à imagem atual.

        Image *new_image = loadFromPNG(filename);
        int nw = new_image->width();
        int nh = new_image->height();

        for (int i = 0; i < nw; i++)
        {
            for (int j = 0; j < nh; j++)
            {
                int tempw = x + i;
                int temph = y + j;
                if (new_image->at(i, j).red() != r || new_image->at(i, j).green() != g || new_image->at(i, j).blue() != b)
                {
                    image->at(tempw, temph).red() = new_image->at(i, j).red();
                    image->at(tempw, temph).green() = new_image->at(i, j).green();
                    image->at(tempw, temph).blue() = new_image->at(i, j).blue();
                }
            }
        }

        delete new_image;
    }

    void Script::crop(int x, int y, int w, int h)
    {
        // Corta uma área retangular da imagem.
        // Cria uma nova imagem para guardar os pixeis cortados.

        Image *croppedImage = new Image(w, h);

        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {

                // Copia os pixeis da imagem original para a imagem cortada.

                croppedImage->at(i, j) = image->at(x + i, y + j);
            }
        }

        // Elimina a imagem anterior se existir.
        delete image;

        // Atribui a imagem cortada à imagem atual.
        image = croppedImage;
    }
    void Script::rotate_left()
    {
        // Roda a imagem para a esquerda.

        int width_ = image->width();
        int height_ = image->height();

        // Cria uma nova imagem para guardar a rotação da imagem para a esquerda.

        Image *left_image = new Image(height_, width_);

        for (int j = 0; j < height_; j++)
        {
            for (int i = 0; i < width_; i++)
            {
                left_image->at(j, width_ - i - 1) = image->at(i, j);
            }
        }
        
        delete image;
        image = left_image;
    }
    void Script::rotate_right()
    {
        // Roda a imagem para a direita.
        
        int width_ = image->width();
        int height_ = image->height();

        // Cria uma nova imagem para guardar a rotação da imagem para a direita.
        
        Image *right_image = new Image(height_, width_);

        for (int j = 0; j < height_; j++)
        {
            for (int i = 0; i < width_; i++)
            {
                right_image->at(height_ - j - 1, i) = image->at(i, j);
            }
        }

        delete image;
        image = right_image;
    }
    void Script::median_filter(int ws)
    {
        int width_ = image->width();
        int height_ = image->height();

        int edge_x = ws / 2;
        int edge_y = ws / 2;

        for (int x = edge_x; x < width_ - edge_x; x++) {
            for (int y = edge_y; y < height_ - edge_y; y++)
            {
                vector<int> red_values;
                vector<int> green_values;
                vector<int> blue_values;

                for (int fx = 0; fx < ws; fx++) {
                    for (int fy = 0; fy < ws; fy++) {
                        int pixel_x = x + fx - edge_x;
                        int pixel_y = y + fy - edge_y;

                        red_values.push_back(image->at(pixel_x, pixel_y).red());
                        green_values.push_back(image->at(pixel_x, pixel_y).green());
                        blue_values.push_back(image->at(pixel_x, pixel_y).blue());
                    }
                }

                sort(red_values.begin(), red_values.end());
                sort(green_values.begin(), green_values.end());
                sort(blue_values.begin(), blue_values.end());

                int median_index = ws * ws / 2;
                int median_red = red_values[median_index];
                int median_green = green_values[median_index];
                int median_blue = blue_values[median_index];

                image->at(x, y).red() = median_red;
                image->at(x, y).green() = median_green;
                image->at(x, y).blue() = median_blue;
            }
        }
    }
}
