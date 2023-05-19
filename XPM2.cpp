#include "XPM2.hpp"
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

namespace prog {
    Image* loadFromXPM2(const string& file) 
    {
        ifstream input(file);
        int w, h, n, c;
        vector<Color> colors;
        vector<char> characters;
        string header;
        getline(input, header);
        input >> w >> h >> n >> c >> ws;

        // Afirma que c = 1, ou seja, haverá sempre apenas uma cor por pixel.

        assert(c == 1);
        
        for (int count = 0; count < n; count++) 
        {
            string line;
            getline(input, line);
            istringstream ml(line);
            string color;
            char character, letter;
            ml >> character >> letter >> color;

            // Converte a sub string num inteiro de base 16.

            rgb_value red = stoi(color.substr(1, 2), nullptr, 16);
            rgb_value green = stoi(color.substr(3, 2), nullptr, 16);
            rgb_value blue = stoi(color.substr(5, 2), nullptr, 16);

            // Guarda os valores RGB.

            colors.push_back({red, green, blue});

            // Guarda o caractere correspondente a essa cor.

            characters.push_back(character);
        }

        Image* load_image = new Image(w, h);

        for (int j = 0; j < h; j++) 
        {
            for (int i = 0; i < w; i++) 
            {
                char pixel_enc;
                input >> pixel_enc;

                // Encontra a cor correspondente ao caractere.

                auto pixel = find(characters.begin(), characters.end(), pixel_enc);
                if (pixel != characters.end()) 
                {
                     // Se encontrar a cor, encontra a posição correspondete e aplica-lhe a cor correspondente.

                    int pos = pixel - characters.begin();
                    load_image->at(i, j) = colors.at(pos);
                }
            }
        }
        return load_image;
    }

    void saveToXPM2(const string& file, const Image* img) 
    {
        int w = img->width();      // Obtem a largura da imagem.
        int h = img->height();     // Obtem a altura da imagem.
        ofstream output(file);

        // Escreve o "header" de XPM2.

        output << "! XPM2" << endl;
        rgb_value pixel_enc = 65;
        vector<Color> colors;
        vector<rgb_value> characters;

        for (int j = 0; j < h; j++) 
        {
            for (int i = 0; i < w; i++) 
            {

                // Encontra a cor correspondente ao caractere.

                auto pixel = find_if(colors.begin(), colors.end(), [&](const Color& c) 
                {
                    return (c.red() == img->at(i, j).red() &&
                           c.blue() == img->at(i, j).blue() &&
                           c.green() == img->at(i, j).green());
                });

                // Se a cor não for encontrada, acrescentamos ao vetor.

                if (pixel == colors.end()) 
                {
                    cout << pixel_enc;
                    characters.push_back(pixel_enc++);
                    cout << img->at(i, j).red() << " " << img->at(i, j).green() << img->at(i, j).blue() << endl;
                    colors.push_back(img->at(i, j));
                }
            }
        }

        // Escreve as dimenções, o número de cores, e o número de cores por pixel = 1.

        output << w << " " << h << " " << characters.size() << " " << 1 << endl;

        // Escreve o caractere e o seu valor.

        for (size_t s = 0; s < characters.size(); s++) 
        {
            output << characters.at(s) << " c #"
                   << setfill('0')
                   << hex << setw(2) << static_cast<int>(colors.at(s).red())
                   << hex << setw(2) << static_cast<int>(colors.at(s).green())
                   << hex << setw(2) << static_cast<int>(colors.at(s).blue()) << endl;
        }

        for (int j = 0; j < h; j++) 
        {
            for (int i = 0; i < w; i++) 
            {
                // Procura outra vez pela cor correspondente ao caractere.

                auto pixel = find_if(colors.begin(), colors.end(), [&](const Color& c)
                {
                    return (c.red() == img->at(i, j).red() && 
                           c.blue() == img->at(i, j).blue() &&
                            c.green() == img->at(i, j).green());
                });

                // Se a cor for encontrada, escreve o caractere correspondente.
                if (pixel != colors.end()) 
                {
                    int pos = pixel - colors.begin();
                    output << characters.at(pos);
                }
            }
        }
    }
}
