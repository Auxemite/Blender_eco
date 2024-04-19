#include "src/backend/render/intersection.hh"
#include "src/backend/image/image.hh"

int main() {
    double aspect_ratio = 16.0 / 9.0;
    int default_width = 1280;
    int default_height = static_cast<int>(default_width / aspect_ratio);

    Image image = Image(default_width, default_height);
    Scene scene = Scene(image.width, image.height);
    image.render_debug(scene);
}