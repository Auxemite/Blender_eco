#pragma once

#include <vector>
#include <fstream>
#include <thread>
#include "../render/intersection.hh"
#include "../utils/utils.hh"

class Image {
    public:
        int width = 0;
        int height = 0;
        std::vector<std::vector<Color>> data;
        std::vector<std::vector<Shape*>> shapes;
        unsigned char* char_data;
        std::vector<std::vector<bool>> selected;

        Image()= default;
        Image(int width_, int height_);

        void render(const Scene& scene, const bool& photorealist=false);
        void render_debug(const Scene& scene, const bool& photorealist=false);
        void render_thread(const Scene& scene, const bool& photorealist, int start, int end);

        void save_as_ppm(const std::string& pathname);
};

Image load_image(const std::string& path_name);