#include "Color.hpp"

namespace prog {
    Color::Color() {
        red_ = 0;
        blue_ = 0;
        green_ = 0;
    }

    // Construtor de cópia.

    Color::Color(const Color& other) {
        red_ = other.red_;
        green_ = other.green_;
        blue_ = other.blue_;
    }

    // Construtor com parâmetros.
    
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        red_ = red;
        green_ = green;
        blue_ = blue;
    }
    rgb_value Color::red() const {
        // Retorna o valor da cor vermelha como uma cópia.
        return red_;
    }
    rgb_value Color::green() const {
        // Retorna o valor da cor verde como uma cópia.
        return green_;
    }
    rgb_value Color::blue() const {
        // Retorna o valor da cor azul como uma cópia.
        return blue_;
    }

    rgb_value& Color::red() {
        // Retorna uma referência ao valor da cor vermelha permitindo que seja modificado.
        return red_;
    }

    rgb_value& Color::green(){
        // Retorna uma referência ao valor da cor verde permitindo que seja modificado.
        return green_;
    }

    rgb_value& Color::blue(){
        // Retorna uma referência ao valor da cor azul permitindo que seja modificado.
        return blue_;
    }
}
