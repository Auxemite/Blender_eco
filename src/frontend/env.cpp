#include "env.hh"

inline float aspect_ratio = 16.0f / 9.0f;
inline int default_width = 1280;
inline int default_height = static_cast<int>(static_cast<float>(default_width) / aspect_ratio);
Image *bg = load_image("../data/sunset.ppm");
int render_count = 0;

Env::Env() {
    image = Image(default_width, default_height);
    scene = Scene(image.width, image.height);
    vertices = std::vector<float>(100, 0.0);
    indices = std::vector<int>(100, 0);
    update_data();
//    render();
//    create_texture();
}

Env::Env(const char* filename) {
    image = *load_image(filename);
    scene = Scene(image.width, image.height);
    vertices = std::vector<float>(1, 0.0);
    indices = std::vector<int>(1, 0);
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

    for (auto mesh: scene.meshes)
    {
        int point_nb = mesh->points.size() * 6;
        vertices = std::vector<float>(point_nb, 0.0f);
        std::vector<int> inter_indices(mesh->points.size(), 0);
        std::cout << "POINT NB = " << point_nb << "\n";
        for (int i = 0; i < mesh->points.size(); ++i)
        {
            int k = i * 6;
            if (k >= point_nb)
                break;
            int j = mesh->get_real_point_index(mesh->points[i]);
            inter_indices[j] = i;

            vertices[k] = mesh->points[i]->x;
            std::cout << vertices[k] << " ";
            vertices[k+1] = mesh->points[i]->y;
            std::cout << vertices[k+1] << " ";
            vertices[k+2] = mesh->points[i]->z;
            std::cout << vertices[k+2] << " ";

            vertices[k+3] = static_cast<float>(i)/mesh->points.size();
            std::cout << vertices[k+3] << " ";
            vertices[k+4] = static_cast<float>(i)/mesh->points.size();
            std::cout << vertices[k+3] << " ";
            vertices[k+5] = static_cast<float>(i)/mesh->points.size();
            std::cout << vertices[k+3] << "\n";
        }
        int indice_nb = mesh->faces.size() * 3;
        std::cout << "INDICE NB = " << indice_nb << "\n";
        indices = std::vector<int>(indice_nb, 0.0f);
        for (int i = 0; i < mesh->faces.size(); ++i)
        {
            int k = i * 3;
            if (k >= indice_nb)
                break;

            int j = mesh->get_real_point_index(mesh->faces[i]->a);
            indices[k] = inter_indices[j];
            std::cout << mesh->get_point_index(mesh->faces[i]->a) << " ";

            j = mesh->get_real_point_index(mesh->faces[i]->b);
            indices[k+1] =  inter_indices[j];
            std::cout << mesh->get_point_index(mesh->faces[i]->b) << " ";

            j = mesh->get_real_point_index(mesh->faces[i]->c);
            indices[k+2] =  inter_indices[j];
            std::cout << mesh->get_point_index(mesh->faces[i]->c) << "\n";
        }
    }
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
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

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
    cameraPos.y = cameraDec.y;
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
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}