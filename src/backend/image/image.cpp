#include "image.hh"

Image::Image(int width_, int height_) {
    width = width_;
    height = height_;
    vector<vector<Color>> data_(
            width,
            vector<Color>(height, {0.0, 0.0, 0.0})
    );

    data = data_;
};

void Image::render(const Scene& scene, const bool& photorealist) {
    const int numThreads = 4; // Number of threads to use
    std::vector<std::thread> threads;

    int batchSize = height / numThreads; // Height is data size
    int start = 0;
    int end = 0;

    for (int i = 0; i < numThreads; ++i) {
        end = (i == numThreads - 1) ? height : start + batchSize;
        threads.push_back(std::thread(render_thread, std::ref(data), width, scene, photorealist, start, end));
        start = end;
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }
}

void Image::save_as_ppm(const std::string& pathname)
{
    std::ofstream ofs(pathname, ifstream::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            auto c = data[i][j];
            ofs << static_cast<char>(static_cast<int>(255.999 * c.r))
                << static_cast<char>(static_cast<int>(255.999 * c.g))
                << static_cast<char>(static_cast<int>(255.999 * c.b));
        }
    }
    ofs.close();
};

void render_thread(std::vector<std::vector<Color>>& data, int width, const Scene& scene,
                   const bool& photorealist, int start, int end)
{
    Camera camera = scene.camera;
    for (int j = start; j < end; ++j) {
        for (int i = 0; i < width; ++i) {
            auto pixel_center = camera.pixel_loc + (i * camera.pixel_u) + (j * camera.pixel_v);
            auto dir = unit_vector(pixel_center - camera.center);
            auto intersection = Intersection(camera.center, dir);
            intersection.throw_ray(scene);
            if (photorealist)
                data[i][j] = intersection.ray_color(scene, 0);
            else
                data[i][j] = intersection.fast_ray_color(scene);
        }
    }
}

Image load_image(const string& path_name) {
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