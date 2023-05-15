#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert") {
                invert();
                continue;
            }
            if(command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::invert() {
        int width_ = image->width();
        int height_ = image->height();
        for (int i = 0; i < width_; i++){
            for (int j = 0; j < height_; j++){
                image->at(i, j).red() = 255 - image->at(i, j).red();
                image->at(i, j).green() = 255 - image->at(i, j).green();
                image->at(i, j).blue() = 255 - image->at(i, j).blue();
            }
        }
    }

    void Script::to_gray_scale() {
        int width_ = image->width();
        int height_ = image->height();
        for (int i = 0; i < width_; i++){
            for (int j = 0; j < height_; j++){
                int v = (image->at(i, j).red() + image->at(i, j).blue() + image->at(i, j).green())/3;
                image->at(i, j).red() = v;
                image->at(i, j).green() = v;
                image->at(i, j).blue() = v;
            }
        }
    }
}
