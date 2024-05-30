#include "image.hh"

Image::Image(int width_, int height_)
{
    width = width_;
    height = height_;
    data = std::vector<std::vector<Color>>(width,
                                           std::vector<Color>(height,
                                                              {0.0, 0.0, 0.0}));
    selected = std::vector<std::vector<bool>>(width,
                                              std::vector<bool>(height,
                                                                false));
};

void render_thread(std::vector<std::vector<Color>>& data, std::vector<std::vector<bool>>& selected, int width, const Scene& scene,
                   const bool& photorealist, int start, int end)
{
    Camera camera = scene.camera;
    for (int j = start; j < end; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            auto pixel_center = camera.pixel_loc + (i * camera.pixel_u) + (j * camera.pixel_v);
            auto dir = (pixel_center - camera.center).norm();
            auto intersection = Intersection(camera.center, dir);
            intersection.throw_ray(scene);
            
            if (photorealist)
                data[i][j] = intersection.ray_color(scene, 0);
            else
            {
                data[i][j] = intersection.fast_ray_color(scene);
                if (intersection.object != nullptr && intersection.object->selected)
                    selected[i][j] = true;
            }
        }
    }
}

void Image::render(const Scene& scene, const bool& photorealist)
{
    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    int batchSize = height / numThreads; // Height is data size
    int start = 0;
    int end = 0;

    for (int i = 0; i < numThreads; ++i) {
        end = (i == numThreads - 1) ? height : start + batchSize;
        threads.push_back(std::thread(render_thread, std::ref(data), std::ref(selected), width, scene, photorealist, start, end));
        start = end;
    }
    for (auto& t : threads)
        t.join();

    if (!photorealist) {
        for (size_t i = 1; i < width - 1; i++)
            for (size_t j = 1; j < height - 1; j++)
            {
                if (selected[i][j])
                    for (int x = 0; x < 3; x++)
                        for (int y = 0; y < 3; y++)
                        {
                            if (!selected[i + x - 1][j + y -1])
                                data[i + x - 1][j + y - 1] = Color(basic::color::cyan);
                        }
            }

        selected = std::vector<std::vector<bool>>(width,
                                              std::vector<bool>(height,
                                                                false));

        int mid_w = width / 2;
        int mid_h = height / 2;
        for (int i = 0; i < 11; ++i)
            data[mid_w - 5 + i][mid_h] = basic::color::red;
        for (int i = 0; i < 11; ++i)
            data[mid_w][mid_h - 5 + i] = basic::color::red;
    }
}

void Image::render_debug(const Scene& scene, const bool& photorealist) {

    render_thread(std::ref(data), std::ref(selected), width, scene, photorealist, 0, height);

    if (!photorealist) {
        int mid_w = width / 2;
        int mid_h = height / 2;
        for (int i = 0; i < 11; ++i)
            data[mid_w - 5 + i][mid_h] = basic::color::red;
        for (int i = 0; i < 11; ++i)
            data[mid_w][mid_h - 5 + i] = basic::color::red;
    }
}

void Image::save_as_ppm(const std::string& pathname)
{
    std::ofstream ofs;
    ofs.open(pathname, std::ifstream::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            auto c = data[i][j];
            ofs << static_cast<char>(255 * c.r)
                << static_cast<char>(255 * c.g)
                << static_cast<char>(255 * c.b);
        }
    }
    ofs.close();
};

Image load_image(const std::string& path_name) {
    std::string line;
    std::ifstream ifs(path_name, std::ifstream::binary);
    if (!ifs.is_open()) {
        std::cerr << "Error Image: Unable to open file." << std::endl;
        return {};
    }

    std::string magicNumber;
    int maxColor, width, height;
    ifs >> magicNumber >> width >> height >> maxColor;

    if (magicNumber != "P6" || maxColor != 255) {
        std::cerr << "Error Image: Invalid headers." << std::endl;
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