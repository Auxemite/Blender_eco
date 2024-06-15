#include "image.hh"

Image::Image(int width_, int height_)
{
    width = width_;
    height = height_;
    unsigned int size = width * height * 3;
    char_data = (unsigned char *)calloc(size + 1, sizeof(unsigned char));
    char_data[size] = '\0';
    data = std::vector<std::vector<Color>>(width,
                                           std::vector<Color>(height,
                                                              {0.0, 0.0, 0.0}));
    shapes = std::vector<std::vector<Shape*>>(width,
                                              std::vector<Shape*>(height,
                                                                nullptr));
    selected = std::vector<std::vector<bool>>(width,
                                              std::vector<bool>(height,
                                                                false));
}

Color Image::bg_color(Image *bg, Vector3 dir)
{
    float dirz = dir.z;
    if (dir.z < 0)
        dirz = -dirz;

    dir.normalize();
    int w = bg->width;
    int h = bg->height;
    int theta = static_cast<int>(acos(dir.y) * h / PI); // because dir is a unit vector
    int phi;
    if (dirz == 0)
        phi = w-1;
    else
        phi = static_cast<int>((atan(dir.x / dirz) + PI/2) * w / PI);

    return bg->data[phi][theta];
}

Color Image::fast_ray_color(const Scene& scene, const Intersection& inter)
{
    Point3 inter_loc = inter.inter_loc;
    if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX))
        return basic::color::background_blue;

    Vector3 normal = inter.object->normal(inter_loc);
    float dot_angle = dot((scene.camera.lookat - scene.camera.center).norm(), normal);
    dot_angle = 1.0f - (dot_angle / 2.0f + 0.5f);
    if (inter.object->selected)
        return Color(2, 2, 0) * dot_angle;

    if (scene.activate_grid && inter.object->get_obj_type() == "Plane") {
        float diff_1 = abs_(round(inter_loc.x) - inter_loc.x);
        float diff_2 = abs_(round(inter_loc.z) - inter_loc.z);
        if (diff_1 <= 0.015 && round(inter_loc.x) == 0 && (round(inter_loc.z) != 0 || inter_loc.z < 0.5))
            return {0.5, 0.2, 0.2};
        if (diff_2 <= 0.015 && round(inter_loc.z) == 0 && (round(inter_loc.x) != 0 || inter_loc.x < 0.5))
            return {0.2, 0.5, 0.2};
        return {0.3, 0.3, 0.3};
    }

    return Color(0.7, 0.7, 0.7) * dot_angle;
}

Color Image::ray_color(const Scene& scene, Image *bg, Intersection inter, int recursive)
{
    if (recursive > 3)
        return basic::color::black;

    Point3 inter_loc = inter.inter_loc;
    if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX))
        return bg_color(bg, inter.dir);

    Color diff_color = basic::color::black;
    Color spec_color = basic::color::black;
    Vector3 normal = inter.object->normal(inter_loc);
    Vector3 refraction = (2.0f * normal - inter.dir).norm();
    for (auto light : scene.lights)
    {
//        if (intersection.inside_object(scene, light))
//            continue;

        auto light_ray = (light->center - inter_loc).norm();
        auto inter_light = Intersection(light->center, -light_ray);
        inter_light.throw_ray(scene);
        if (inter_light.object != inter.object) // Check shadows
            continue;

        normal = inter.object->normal(inter_loc);
        refraction = (2.0f * normal - inter.dir).norm();
        // refraction = (2 * dot(normal, light_ray) * normal - light_ray).norm();
        diff_color = diff_color + inter.diffuse(light_ray, normal);
        spec_color = spec_color + inter.specular(light, light_ray, refraction);
    }
    inter.update(inter_loc + (refraction * 0.01f), refraction);
    inter.throw_ray(scene);
    Color second = ray_color(scene, bg, inter, recursive + 1);

    if (refraction == Vector3(0.0f,0.0f,0.0f))
        return cap(inter.object->get_material(inter_loc).texture.ks * second);
    // return cap(spec_color + diff_color);

    return cap(diff_color + spec_color + inter.object->get_material(inter_loc).texture.ks * second);
}

void Image::update_char_data(unsigned int i, unsigned int j) {
    unsigned int k = (j * width + i) * 3;
    char_data[k] = static_cast<unsigned char>(data[i][j].r * 255);
    char_data[k+1] = static_cast<unsigned char>(data[i][j].g * 255);
    char_data[k+2] = static_cast<unsigned char>(data[i][j].b * 255);
}

void Image::render_thread(const Scene& scene, Image *bg, const bool& photorealist, int start, int end)
{
    Camera camera = scene.camera;
    for (unsigned int j = start; j < end; ++j)
    {
        for (unsigned int i = 0; i < width; ++i)
        {
            auto pixel_center = camera.pixel_loc + (i * camera.pixel_u) + (j * camera.pixel_v);
            auto dir = (pixel_center - camera.center).norm();
            auto intersection = Intersection(camera.center, dir);

            if (photorealist) {
                intersection.throw_ray(scene);
                data[i][j] = ray_color(scene, bg, intersection, 0);
            }
            else
            {
                intersection.fast_throw_ray(scene);
                data[i][j] = fast_ray_color(scene, intersection);

                if (intersection.object != nullptr)
                    shapes[i][j] = intersection.object;
//                if (intersection.object != nullptr && intersection.object->selected)
//                    selected[i][j] = true;

            }
            update_char_data(i, j);
        }
    }
}

void Image::render(const Scene& scene, Image *bg, const bool& photorealist, const bool& fast_selection)
{
    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    int batchSize = height / numThreads; // Height is data size
    int start = 0;
    int end = 0;

    for (int i = 0; i < numThreads; ++i) {
        end = (i == numThreads - 1) ? height : start + batchSize;
        threads.emplace_back(&Image::render_thread, this, scene, bg, photorealist, start, end);
        start = end;
    }
    for (auto& t : threads)
        t.join();

    if (!photorealist)
        postprocess(fast_selection);
}

void Image::render_debug(const Scene& scene, Image *bg, const bool& photorealist, const bool& fast_selection) {
    render_thread(scene, bg, photorealist, 0, height);
    if (!photorealist)
        postprocess(fast_selection);
}

void Image::postprocess(const bool& fast_selection) {
    if (!fast_selection) {
        for (unsigned int j = 1; j < height - 1; j++)
            for (unsigned int i = 1; i < width - 1; i++)
                if (shapes[i][j] != nullptr &&
                    ((shapes[i - 1][j - 1] != shapes[i][j]) && (shapes[i - 1][j - 1] != nullptr) ||
                    (shapes[i + 1][j - 1] != shapes[i][j]) && (shapes[i + 1][j - 1] != nullptr) ||
                    (shapes[i - 1][j + 1] != shapes[i][j]) && (shapes[i - 1][j + 1] != nullptr) ||
                    (shapes[i + 1][j + 1] != shapes[i][j]) && (shapes[i + 1][j + 1] != nullptr)))
                {
                    data[i][j] = Color(0.1, 0.1, 0.1);
                    update_char_data(i, j);
                }

        for (unsigned int j = 1; j < height - 1; j++)
            for (unsigned int i = 1; i < width - 1; i++)
                if (shapes[i][j] != nullptr && shapes[i][j]->selected)
                    for (int x = 0; x < 3; x++)
                        for (int y = 0; y < 3; y++)
                            if (!shapes[i + x - 1][j + y -1] || !shapes[i + x - 1][j + y - 1]->selected) {
                                data[i + x - 1][j + y - 1] = Color(basic::color::cyan);
                                update_char_data(i, j);
                            }

        shapes = std::vector<std::vector<Shape*>>(width,
                                                  std::vector<Shape*>(height,
                                                                      nullptr));
    }
    int mid_w = width / 2;
    int mid_h = height / 2;
    for (int i = 0; i < 11; ++i)
        update_char_data(mid_h, mid_w - 5 + i);
    for (int i = 0; i < 11; ++i)
        update_char_data(mid_h - 5 + i, mid_w);
}

void Image::save_as_ppm(const std::string& pathname)
{
    std::ofstream ofs;
    ofs.open(pathname, std::ifstream::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Color c = data[i][j];
            ofs << static_cast<char>(255 * c.r)
                << static_cast<char>(255 * c.g)
                << static_cast<char>(255 * c.b);
        }
    }
    ofs.close();
}

Image *load_image(const std::string& path_name) {
    std::string line;
    std::ifstream ifs(path_name, std::ifstream::binary);
    if (!ifs.is_open()) {
        std::cerr << "Error Image: Unable to open file." << std::endl;
        return nullptr;
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
            float r = ifs.get();
            float g = ifs.get();
            float b = ifs.get();
            image.data[i][j] = Color(r, g, b) / 255;
        }

    ifs.close();
    return new Image(image);
}