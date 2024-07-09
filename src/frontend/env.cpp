#include "env.hh"

inline float aspect_ratio = 16.0f / 9.0f;
inline int default_width = 1280;
inline int default_height = static_cast<int>(static_cast<float>(default_width) / aspect_ratio);
Image *bg = load_image("../data/sunset.ppm");
int render_count = 0;

Env::Env() {
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
    vertices = nullptr;
    indices = nullptr;
    update_data();
//    render();
//    create_texture();
}

Env::Env(const char* filename) {
    image = *load_image(filename);
    scene = Scene(image.width, image.height);
    vertices = nullptr;
    indices = nullptr;
    update_data();
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

void Env::update_data() {
    if (vertices != nullptr)
        free(vertices);
    if (indices != nullptr)
        free(indices);
    float svertices[] = {
            // positions          // colors
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    //        -3.0f,  3.0f,  3.0f,  0.0f, 0.0f, 0.0f
    };
    int sindices[] = {
            0, 1, 3,
            1, 2, 3,
            1, 5, 2,
            5, 6, 2,
            5, 4, 6,
            4, 7, 6,
            4, 0, 7,
            0, 3, 7,
            3, 2, 7,
            2, 6, 7,
            4, 5, 0,
            5, 1, 0,
//        0, 3, 8,
    };
    
//    vertices = (float *) calloc(48, sizeof(float));
//    for (int i = 0; i < 48; ++i) {
//        vertices[i] = svertices[i];
//    }
//    indices = (int *) calloc(36, sizeof(int));
//    for (int i = 0; i < 36; ++i) {
//        indices[i] = sindices[i];
//    }

//    for (auto mesh: scene.meshes)
//    {
//        int point_nb = mesh->points.size() * 6;
//        std::cout << "POINT NB = " << point_nb << "\n";
//        vertices = (float *) calloc(point_nb, sizeof(float));
//        for (int i = 0; i < mesh->points.size(); ++i)
//        {
//            int k = i * 6;
//            if (k >= point_nb)
//                break;
//
//            vertices[k] = mesh->points[i]->x;
//            std::cout << mesh->points[i]->x << " ";
//            vertices[k+1] = mesh->points[i]->y;
//            std::cout << mesh->points[i]->y << " ";
//            vertices[k+2] = mesh->points[i]->z;
//            std::cout << mesh->points[i]->z << " ";
//
//            vertices[k+3] = r;
//            std::cout << r << " ";
//            vertices[k+4] = g;
//            std::cout << g << " ";
//            vertices[k+5] = b;
//            std::cout << b << "\n";
//        }
//        int indice_nb = mesh->faces.size() * 3;
//        std::cout << "INDICE NB = " << indice_nb << "\n";
//        indices = (int *) calloc(indice_nb, sizeof(int));
//        for (int i = 0; i < mesh->faces.size(); ++i)
//        {
//            int k = i * 3;
//            if (k >= indice_nb)
//                break;
//
//            indices[k] = mesh->get_point_index(mesh->faces[i]->a);
//            std::cout << mesh->get_point_index(mesh->faces[i]->a) << " ";
//            indices[k+1] = mesh->get_point_index(mesh->faces[i]->b);
//            std::cout << mesh->get_point_index(mesh->faces[i]->b) << " ";
//            indices[k+2] = mesh->get_point_index(mesh->faces[i]->c);
//            std::cout << mesh->get_point_index(mesh->faces[i]->c) << "\n";
//        }
//    }
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

void Env::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Env::load_data() {
    if (vertices == nullptr || indices == nullptr) {
        std::cerr << "Load Data Error : vertices or indices is empty\n";
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    checkOpenGLError("After loading data");
}

void Env::draw_data(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    cameraPos.x = radius * cos(glm::radians(yaw));
    cameraPos.z = radius * sin(glm::radians(yaw));
    cameraFront = glm::normalize(-cameraPos);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}