#include "render.hh"
#include "src/utils.hh"

void bounce(const vector<Sphere>& s, const vector<Point_Light>& lights);
void around(const vector<Sphere>& s, const vector<Point_Light>& lights);
void around2(const vector<Point_Light>& lights);
void sunset(const vector<Sphere>& s, Point3 cam_loc, Color sun_color);

int main() {
    vector<Sphere> s;
    vector<Point_Light> l;
    s.push_back(Sphere({0,-100.5,3},100,u::basic(u::gray_sky)));
    s.push_back(Sphere({0,0,8},u::simple(u::white)));
    s.push_back(Sphere({2,0,7},u::plastic(u::magenta)));
    s.push_back(Sphere({-2,0,6},u::metal(u::purple)));
    l.push_back(Point_Light({0,5,1}, 10, u::orange));
//    l.push_back(Point_Light({0,100,0}, 10, u::orange));


    around2(l);
//    render_image(s, l, u::p0, {0,0,1}, "../test/aaaaaa.ppm");
//    render_image(s, l, u::p0, {0,0,-1}, "../test/aaaaaa2.ppm");
//    sunset(s, u::p0, u::purple);
    return 0;
}

void bounce(const vector<Sphere>& s, const vector<Point_Light>& lights) {
    vector<Point3> cam_loc{
            {0,0,0},
            {0,0.2,0},
            {0,0.4,0},
            {0,0.6,0},
            {0,0.8,0},
            {0,1,0}
    };
    render_all_cams(s, lights, cam_loc, {0,0,1},"../test/bource/bounce");
}

void around(const vector<Sphere>& s, const vector<Point_Light>& lights) {
    vector<Point3> cam_loc;
    Point3 p(0,10,0);
    double angle = PI / 8;
    for (int i = 0; i < 16; ++i) {
        p = Point3(cos(angle)*p.x - sin(angle)*p.y, sin(angle)*p.x+cos(angle)*p.y, 0) + Point3(0,5,-20);
        cam_loc.push_back(p);
    }
    render_all_cams(s, lights, cam_loc, {0,0,1}, "../test/around/around");
}

void around2(const vector<Point_Light>& lights) {
    vector<Sphere> s;
//    s.push_back(Sphere({25,-50.5,0},50,u::basic(u::gray_sky)));
//    s.push_back(Sphere({-25,-50.5,0},50,u::basic(u::gray_sky)));
    s.push_back(Sphere({0,0,0},u::simple(u::white)));
    s.push_back(Sphere({2,0,0},u::plastic(u::magenta)));
    s.push_back(Sphere({-2,0,0},u::metal(u::purple)));
    vector<Point3> cam_loc;
    Point3 p(0,0,-5);
    double angle = PI / 8;
    for (int i = 0; i < 16; ++i) {
        p = Point3(cos(angle)*p.x - sin(angle)*p.z, 0, sin(angle)*p.x+cos(angle)*p.z) + Point3(0,5,0);
        cam_loc.push_back(p);
    }
    render_all_cams(s, lights, cam_loc, {0,0,0}, "../test/around/around");
}

void sunset(const vector<Sphere>& s, Point3 cam_loc, Color sun_color) {
    auto power = 20;
    vector<Point3> light_loc{};
    Point3 p(0,-150,0);
    double angle = PI / 8;
    for (int i = 0; i < 16; ++i) {
        p = Point3(cos(angle)*p.x - sin(angle)*p.y, sin(angle)*p.x+cos(angle)*p.y, 0);
//            cout << p.x << " " << p.y << "\n";
        light_loc.push_back(p);
    }
    render_all_lights(s, light_loc, power, cam_loc, sun_color, "../test/sunset/sunset");
}
