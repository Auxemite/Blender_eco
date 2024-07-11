#include "env.hh"

Image *bg = load_image("../data/sunset.ppm");
int render_count = 0;

Env::Env() {
    image = Image(WIDTH, HEIGHT);
    scene = Scene(image.width, image.height);
    VBOs = std::vector<unsigned int>(1, 0);
    VAOs = std::vector<unsigned int>(1, 0);
    EBOs = std::vector<unsigned int>(1, 0);
    update_data(0);
//    render();
//    create_texture();
}

Env::Env(const char* filename) {
    image = *load_image(filename);
    scene = Scene(image.width, image.height);
    VBOs = std::vector<unsigned int>(1, 0);
    VAOs = std::vector<unsigned int>(1, 0);
    EBOs = std::vector<unsigned int>(1, 0);
    update_data(0);
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
//    render();
}

void Env::update_data(int mesh_index) {
    Mesh *mesh = scene.meshes[mesh_index];
    int point_nb = mesh->points.size() * 6;
    std::vector<float> vertices(point_nb, 0.0f);
    std::vector<int> inter_indices(mesh->points.size(), 0);
//        std::cout << "POINT NB = " << point_nb << "\n";
    for (int i = 0; i < mesh->points.size(); ++i)
    {
        int k = i * 6;
        if (k >= point_nb)
            break;
        int j = mesh->get_real_point_index(mesh->points[i]);
        inter_indices[j] = i;

        vertices[k] = mesh->points[i]->x;
        vertices[k+1] = mesh->points[i]->y;
        vertices[k+2] = mesh->points[i]->z;

        vertices[k+3] = mesh->texture.material.color.r;
        vertices[k+4] =  mesh->texture.material.color.g;
        vertices[k+5] =  mesh->texture.material.color.b;
    }
    int indice_nb = mesh->faces.size() * 3;
//        std::cout << "INDICE NB = " << indice_nb << "\n";
    std::vector<int> indices(indice_nb, 0);
    for (int i = 0; i < mesh->faces.size(); ++i)
    {
        int k = i * 3;
        if (k >= indice_nb)
            break;

        int j = mesh->get_real_point_index(mesh->faces[i]->a);
        indices[k] = inter_indices[j];
        j = mesh->get_real_point_index(mesh->faces[i]->b);
        indices[k+1] =  inter_indices[j];
        j = mesh->get_real_point_index(mesh->faces[i]->c);
        indices[k+2] =  inter_indices[j];
    }
    cleanup(mesh_index);
    load_data(mesh_index, vertices, indices);
}

void Env::render(int mesh_index) {

    render_count++;
    std::cout  << "Rendering " << render_count << "\n";
//    image.render(scene, bg, photorealist, fast_selection);
    if (mesh_index != -1)
        update_data(mesh_index);
    else if (scene.focus_mesh != nullptr) {
        for (int i = 0; i < scene.meshes.size(); ++i) {
            if (scene.meshes[i] == scene.focus_mesh) {
                update_data(i);
                break;
            }
        }
    }
}

void Env::save_mesh(const std::string& filename) const {
    if (scene.focus_mesh == nullptr)
        std::cerr << "Save Mesh Error: NO MESH SELECTED\n";
    else
        scene.focus_mesh->to_dot_obj(filename);
}

void Env::update_camera() {
    scene.camera.update_cam(Point3(cameraPos.x, cameraPos.y, cameraPos.z));
    std::cout << "Camera Center : " << scene.camera.center << "\n";
}

void Env::cleanup(int mesh_index) {
    glDeleteVertexArrays(1, &(VAOs[mesh_index]));
    glDeleteBuffers(1, &(VBOs[mesh_index]));
    glDeleteBuffers(1, &(EBOs[mesh_index]));
}

void Env::load_data(int mesh_index, std::vector<float> vertices, std::vector<int> indices) {
    glGenVertexArrays(1, &(VAOs[mesh_index]));
    glGenBuffers(1, &(VBOs[mesh_index]));
    glGenBuffers(1, &(EBOs[mesh_index]));

    glBindVertexArray(VAOs[mesh_index]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[mesh_index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[mesh_index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    checkOpenGLError("After loading data");
}

void Env::load_grid() {
    gridVertices = generateGrid(100);
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Env::draw_data(unsigned int shaderProgram, int mesh_index) {
    glUseProgram(shaderProgram);

    cameraPos.x = radius * cos(glm::radians(yaw));
    cameraPos.z = radius * sin(glm::radians(yaw));
    cameraPos.y = cameraDec.y;
    cameraFront = glm::normalize(-cameraPos);
    glm::vec3 center = glm::vec3(0.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos, center, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAOs[mesh_index]);
    glDrawElements(GL_TRIANGLES, scene.meshes[mesh_index]->faces.size() * 3, GL_UNSIGNED_INT, 0);
}

void Env::draw_grid(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(gridVAO);
    glDrawArrays(GL_LINES, 0, gridVertices.size() / 6);
}

std::vector<float> generateGrid(int gridSize) {
    std::vector<float> gridVertices;
    // Red Line for Z = 0
    gridVertices.push_back(-gridSize);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.3f);

    gridVertices.push_back(gridSize);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.3f);

    // Green Line for X = 0
    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(-gridSize);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);

    gridVertices.push_back(0.0f);
    gridVertices.push_back(0.0f);
    gridVertices.push_back(gridSize);
    gridVertices.push_back(0.3f);
    gridVertices.push_back(0.6f);
    gridVertices.push_back(0.3f);

    float grid_clr = 0.4f;
    for (int i = -gridSize; i <= gridSize; ++i) {
        // X
        gridVertices.push_back((float)i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)-gridSize);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        gridVertices.push_back((float)i);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)gridSize);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        // Z
        gridVertices.push_back((float)-gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)i);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);

        gridVertices.push_back((float)gridSize);
        gridVertices.push_back(0.0f);
        gridVertices.push_back((float)i);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
        gridVertices.push_back(grid_clr);
    }

    return gridVertices;
}