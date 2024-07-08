#include "env.hh"

inline float aspect_ratio = 16.0f / 9.0f;
inline int default_width = 1280;
inline int default_height = static_cast<int>(static_cast<float>(default_width) / aspect_ratio);
Image *bg = load_image("../data/sunset.ppm");
int render_count = 0;

Env::Env() {
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
//    render();
//    create_texture();
}

Env::Env(const char* filename) {
    image = *load_image(filename);
    scene = Scene(image.width, image.height);
//    render();
//    create_texture();
}

void Env::create_texture() {
    unsigned char* image_data = image.char_data;
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

    glDeleteTextures(1, &render_image);
    render_image = image_texture;
}

void Env::change_bg(const std::string& name) {
    Image *img = load_image("../data/" + name + ".ppm");
    if (img == nullptr) {
        return;
    }
    delete bg;
    bg = img;
    std::cout << "Changed Background Image";
    render();
}

void Env::render() {
    render_count++;
    std::cout  << "Rendering " << render_count << "\n";
    image.render(scene, bg, photorealist, fast_selection);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, image.char_data);
}

void Env::save_mesh(const std::string& filename) const {
    if (scene.focus_mesh == nullptr)
        std::cerr << "Save Mesh Error: NO MESH SELECTED\n";
    else
        scene.focus_mesh->to_dot_obj(filename);
}
