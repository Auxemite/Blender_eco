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

void Image::render(const Scene& scene, bool photorealist) {
    Camera camera = scene.camera;
    for (int j = 0; j < height; ++j)
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