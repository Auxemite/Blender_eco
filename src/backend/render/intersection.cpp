#include "intersection.hh"

void Intersection::update(const Point3& origin_, const Vector3& dir_) {
    origin = origin_;
    dir = dir_;
    inter_loc = null_point;
};

Intersection::Intersection(const Point3& origin_, const Vector3& dir_) {
    origin = origin_;
    dir = dir_;
    inter_loc = null_point;
    object = nullptr;
}

inline void Intersection::fast_throw_ray(Shape *shape)
{
    float inter_scal = shape->ray_intersection(origin, dir);
    if (inter_scal > 0)
    {
        Point3 new_inter_loc = origin + dir * inter_scal;
        if (inter_loc == null_point
            || (new_inter_loc - origin).length() < (inter_loc - origin).length())
        {
            object = shape;
            inter_loc = new_inter_loc;
        }
    }
}

inline void Intersection::throw_ray(Shape *shape)
{
    float inter_scal = shape->ray_intersection(origin, dir);
    if (inter_scal > 0)
    {
        Point3 new_inter_loc = origin + dir * inter_scal;
        if (inter_loc == null_point
            || (new_inter_loc - origin).length() < (inter_loc - origin).length())
        {
            object = shape;
            inter_loc = new_inter_loc;
        }
    }
}

void Intersection::fast_throw_ray(const Scene& scene)
{
    if (scene.activate_grid)
        for (auto obj : scene.objects)
            fast_throw_ray(obj);

    for (auto mesh : scene.meshes)
    {
        // Check hit_box
        if (mesh->hit_box.ray_intersection(origin, dir) <= 0)
            continue;

        for (auto face : mesh->faces)
        {
            // Check backface culling
            if (dot(dir, face->normal_) < 0)
                fast_throw_ray(face);
        }
    }
}

void Intersection::throw_ray(const Scene& scene)
{
    for (auto mesh : scene.meshes)
    {
        // Check hit_box
        if (mesh->hit_box.ray_intersection(origin, dir) <= 0)
            continue;

        for (auto face : mesh->faces)
        {
            // Check backface culling
            if (dot(dir, face->normal_) < 0) 
                throw_ray(face);
        }
    }
}

bool Intersection::inside_object(const Scene& scene, Light *light)
{
    // for (const auto& s : scene.spheres) {
    //     if ((light.center - s->).length() < s.radius)
    //         return true;
    // }
    return false;
}

Color Intersection::diffuse(Vector3 light_ray, Vector3 normal) const
{
    float diffuse = dot(normal, light_ray);
    if (diffuse < 0)
        return basic::color::black;

    Color ray_color = object->get_material(inter_loc).texture.kd
                      * object->texture.material.color;

    return ray_color * (diffuse / (normal.length() * light_ray.length()));
}

Color Intersection::specular(Light *light, const Vector3& light_ray, const Vector3& refaction) const {
    float spec = dot(light_ray, refaction);
    if (spec <= 0)
        return basic::color::black;

    float dist = (light->center - inter_loc).length(); // Not a unit vector is normal
    /* Color spec_color = cap(light->color
                          * sphere->texture.material.texture.ks
                          * pow(spec, sphere->texture.material.texture.ns)
                          * light->power / dist); */

    Color spec_color = light->color
                      * object->texture.material.texture.ks
                      * pow(spec, object->texture.material.texture.ns)
                      * light->power / dist;
    return spec_color;
}
