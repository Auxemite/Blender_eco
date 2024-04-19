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

        Image()= default;
        Image(int width_, int height_);

        void render(const Scene& scene, const bool& photorealist=false);
        void render_debug(const Scene& scene, const bool& photorealist=false);

        void save_as_ppm(const std::string& pathname);
};

void render_thread(std::vector<std::vector<Color>>& data, int width, const Scene& scene,
                   const bool& photorealist, int start, int end);
Image load_image(const string& path_name);