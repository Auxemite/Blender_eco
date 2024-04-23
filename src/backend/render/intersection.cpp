#include "intersection.hh"

const Image& fond = load_image("../test/sunset.ppm");

void Intersection::throw_ray(const Scene& scene)
{
    for (auto new_sphere : scene.spheres)
    {
        auto inter_scal = new_sphere->ray_intersection(origin, dir);
        if (inter_scal > 0)
        {
            Point3 new_inter_loc = origin + dir * inter_scal;
            if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX)
                || (new_inter_loc - origin).length() < (inter_loc - origin).length())
            {
                sphere = new_sphere;
                inter_loc = new_inter_loc;
            }
        }
    }
}

Color Intersection::bg_color()
{
    auto dirz = dir.z;
    if (dir.z < 0)
        dirz = -dirz;

    dir.normalize();
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

bool Intersection::inside_object(const Scene& scene, Light *light)
{
    // for (const auto& s : scene.spheres) {
    //     if ((light.center - s->).length() < s.radius)
    //         return true;
    // }
    return false;
}

Color Intersection::fast_ray_color(const Scene& scene)
{
    if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX))
        return basic::color::background_blue;

    Vector3 normal = sphere->normal(inter_loc);
    double dot_angle = dot(dir, normal);
    bool selected = true;
    if (abs_(dot_angle) <= 0.15 && selected)
        return basic::color::orange;
        
    return sphere->texture.mat.color * (1 - dot_angle);
}

Color Intersection::ray_color(const Scene& scene, int recursive)
{
    if (recursive > 2)
        return basic::color::black;

    if (inter_loc == Point3(INT_MAX, INT_MAX, INT_MAX))
        return bg_color();

    Color diff_color = basic::color::black;
    Color spec_color = basic::color::black;
    Vector3 normal = sphere->normal(inter_loc);
    Vector3 refraction = (2 * normal - dir).norm();
    for (auto light : scene.lights)
    {
        if (inside_object(scene, light))
            continue;

        auto light_ray = (light->center - inter_loc).norm();
        auto inter_light = Intersection(light->center, -light_ray);
        inter_light.throw_ray(scene);
        if (inter_light.sphere != sphere) // Check shadows
            continue;

        auto normal = sphere->normal(inter_loc);
        refraction = (2 * normal - dir).norm();
        // refraction = (2 * dot(normal, light_ray) * normal - light_ray).norm();
        diff_color = diff_color + diffuse(light_ray, normal);
        spec_color = spec_color + specular(light, light_ray, refraction);
    }
    update(inter_loc + (refraction * 0.01), refraction);
    throw_ray(scene);
    auto second = ray_color(scene, recursive + 1);

    if (refraction == Vector3(0,0,0))
        return cap(sphere->get_material(inter_loc).texture.ks * second);
        // return cap(spec_color + diff_color);

    
    return cap(diff_color + spec_color + sphere->get_material(inter_loc).texture.ks * second);
}

Color Intersection::diffuse(Vector3 light_ray, Vector3 normal)
{
    auto diffuse = dot(normal, light_ray);
    if (diffuse < 0)
        return basic::color::black;

    Color ray_color = sphere->get_material(inter_loc).texture.kd 
                      * sphere->texture.mat.color;

    return ray_color * (diffuse / (normal.length() * light_ray.length()));
}

Color Intersection::specular(Light *light, Vector3 light_ray, Vector3 refaction) {
    auto spec = dot(light_ray, refaction);
    if (spec <= 0)
        return basic::color::black;

    auto dist = (light->center - inter_loc).length(); // Not a unit vector is normal
    /* auto spec_color = cap(light->color
                          * sphere->texture.mat.texture.ks
                          * pow(spec, sphere->texture.mat.texture.ns)
                          * light->power / dist); */

    auto spec_color = light->color
                          * sphere->texture.mat.texture.ks
                          * pow(spec, sphere->texture.mat.texture.ns)
                          * light->power / dist;
    return spec_color;
}


void Intersection::update(Point3 origin_, Vector3 dir_) {
    origin = origin_;
    dir = dir_;
    inter_loc = Point3(INT_MAX, INT_MAX, INT_MAX);
};

Intersection::Intersection(const Point3& origin_, const Vector3& dir_) {
    origin = origin_;
    dir = dir_;
    inter_loc = Point3(INT_MAX, INT_MAX, INT_MAX);
    sphere = NULL;
}
