#pragma once

#include <vector>
#include <fstream>
#include "../render/intersection.hh"

class Image {
    public:
        int width = 0;
        int height = 0;
        std::vector<std::vector<Color>> data;

        Image()= default;
        Image(int width_, int height_);

        void render(const Scene& scene);

        void save_as_ppm(const std::string& pathname);
};

inline Image load_image(const string& path_name) {
    string line;
    ifstream ifs(path_name, ifstream::binary);
    if (!ifs.is_open()) {
        cerr << "Error Image: Unable to open file." << endl;
        return {};
    }

    string magicNumber;
    int maxColor, width, height;
    ifs >> magicNumber >> width >> height >> maxColor;

    if (magicNumber != "P6" || maxColor != 255) {
        cerr << "Error Image: Invalid headers." << endl;
        return {};
    }
    ifs.get();

    Image image = Image(width, height);
    for (int j = 0; j < height; ++j)
        for (int i = 0; i < width; ++i) {
            double r = ifs.get();
            double g = ifs.get();
            double b = ifs.get();
            image.data[i][j] = Color(r, g, b) / 255;
        }

    ifs.close();
    return image;
}