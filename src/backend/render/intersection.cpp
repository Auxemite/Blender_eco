#include "intersection.hh"

Color black = Color(0,0,0);
const Image& fond = load_image("../test/sunset.ppm");
//const Image& fond = load_image("./test/retro.ppm");

void Intersection::throw_ray(const Scene& scene) {
    for (auto new_sphere : scene.spheres) {
        auto inter_scal = new_sphere.ray_intersection(origin, dir);

        if (inter_scal > 0) {
            Point3 new_inter_loc = origin + dir * inter_scal;
            if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX)
                || (new_inter_loc - origin).norme() < (inter_loc - origin).norme()) {
                sphere = new_sphere;
                inter_loc = new_inter_loc;
            }
        }
    }
}

Color Intersection::bg_color() {
    auto dirz = dir.z;
    if (dir.z < 0)
        dirz = -dirz;

    dir = unit_vector(dir);
    auto w = fond.width;
    auto h = fond.height;
    auto theta =  static_cast<int>(acos(dir.y) * h / PI); // because dir is a unit vector
    int phi;
    if (dirz == 0)
        phi = w-1;
    else
        phi = static_cast<int>((atan(dir.x / dirz) + PI/2) * w / PI);

    return fond.data[phi][theta];
}

bool Intersection::inside_object(const Scene& scene, Point_Light light) {
    for (const auto& s : scene.spheres) {
        if ((light.center - s.center).norme() < s.radius)
            return true;
    }
    return false;
}

Color Intersection::fast_ray_color(const Scene& scene) {
    if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX))
        return {0.2, 0.2, 0.3};

    auto normale = sphere.normale(inter_loc);
    Color ray_color = 0.5
                      * Color(normale.x + 1, normale.y, normale.z + 1)
                      * sphere.get_material().color;
    auto light_ray = unit_vector(scene.lights[0].center - inter_loc); // Main light
    return ray_color;
}

Color Intersection::ray_color(const Scene& scene, int recursive) {
    if (recursive > 6)
        return black;

    if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX))
        return bg_color();

    Color diff_color = black;
    Color spec_color = black;
    Vector3 refraction;
    for (auto light : scene.lights) {
        if (inside_object(scene, light))
            continue;

        auto light_ray = unit_vector(light.center - inter_loc);
        auto inter_light = Intersection(light.center, -light_ray);
        inter_light.throw_ray(scene);
        if (inter_light.inter_loc != inter_loc) // Check shadows
            continue;

        auto normale = sphere.normale(inter_loc);
        refraction = unit_vector(2 * dot(normale, light_ray) * normale - light_ray);
        diff_color = diff_color + diffuse(light_ray, normale);
        spec_color = spec_color + specular(light, light_ray, refraction);
    }
    if (refraction == Vector3(0,0,0))
        return cap(spec_color + diff_color);

    update(inter_loc, refraction);
    throw_ray(scene);
    auto second = ray_color(scene, recursive + 1);
    return cap(spec_color + diff_color * second);
}

Color Intersection::diffuse(Vector3 light_ray, Vector3 normale) {
    auto diffuse = dot(normale, light_ray);
    if (diffuse < 0)
        return black;

    Color ray_color = 0.5
                      * Color(normale.x + 1, normale.y, normale.z + 1)
                      * sphere.get_material().color;
    return ray_color * (diffuse / (normale.norme() * light_ray.norme()));
}

Color Intersection::specular(Point_Light light, Vector3 light_ray, Vector3 refaction) {
    auto spec = dot(light_ray, refaction);
    if (spec <= 0)
        return black;

    auto dist = (light.center - inter_loc).norme(); // Not a unit vector is normal
    auto spec_color = cap(light.color
                          * sphere.get_material().ks
                          * pow(spec, sphere.get_material().ns)
                          * light.power / dist);
    return spec_color;
}


void Intersection::update(Point3 origin_, Vector3 dir_) {
    origin = origin_;
    dir = dir_;
    inter_loc = Point3(INT_MAX, INT_MAX, INT_MAX);
};

Intersection::Intersection(Point3 origin_, Vector3 dir_) {
    origin = origin_;
    dir = dir_;
    inter_loc = Point3(INT_MAX, INT_MAX, INT_MAX);
    sphere = Sphere();
}
