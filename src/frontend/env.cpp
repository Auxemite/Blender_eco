#include "env.hh"

Image *bg = load_image("../data/sunset.ppm");
int render_count = 0;

Env::Env() {
    image = Image(TEMP_WIDTH, TEMP_HEIGHT);
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
    if (mesh_index == -1)
        return;
    Mesh *mesh = scene.meshes[mesh_index];
    std::vector<float> vertices(0, 0.0f);

    std::vector<int> indices(0, 0);
    for (int i = 0; i < mesh->faces.size(); ++i)
    {
        Point3 *p_a = mesh->faces[i]->a;
        Point3 *p_b = mesh->faces[i]->b;
        Point3 *p_c = mesh->faces[i]->c;
        Color color(1.0f, 1.0f, 1.0f);
        if (render_mode == 0 || render_mode == 1) {
            if (mesh->faces[i]->selected)
                color = Color(0.0f, 2.0f, 2.0f);
        }
        else
            color = Color(mesh->faces[0]->texture.material.color);

        vertices.push_back(p_a->x);
        vertices.push_back(p_a->y);
        vertices.push_back(p_a->z);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
        indices.push_back(i * 3);

        vertices.push_back(p_b->x);
        vertices.push_back(p_b->y);
        vertices.push_back(p_b->z);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
        indices.push_back(i * 3 + 1);

        vertices.push_back(p_c->x);
        vertices.push_back(p_c->y);
        vertices.push_back(p_c->z);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
        indices.push_back(i * 3 + 2);
    }
    // std::cout << "POINT NB = " << vertices.size() << "\n";
    // std::cout << "INDICE NB = " << indices.size() << "\n";
//    std::cout << "{ ";
//    for (int i = 0; i < vertices.size(); i+=6) {
//        if (i != 0 && i%18 == 0)
//            std::cout << "\n";
//        std::cout << vertices[i] << " ";
//        std::cout << vertices[i+1] << " ";
//        std::cout << vertices[i+2] << ";  ";
//    }
//    std::cout << " }\n";
    cleanup(mesh_index);
    load_data(mesh_index, vertices, indices);
}

void Env::render(int mesh_index) {
    render_count++;
    std::cout  << "Rendering " << render_count << "\n";
//    image.render(scene, bg, photorealist, fast_selection);
    if (mesh_index != -1)
        update_data(mesh_index);
    else if (scene.focus_mesh != nullptr)
        update_data(scene.focus_index);
}

void Env::render_all() {
    for (int i = 0; i < scene.meshes.size(); ++i)
        render(i);
}

void Env::edit_mode() {
    if (scene.focus_mesh != nullptr) {
        scene.editmode = true;
        render();
    }
}

void Env::normal_mode() {
    scene.editmode = false;
    scene.selected_mode = 0;
    scene.update_selection_mode();
    int index = scene.focus_index;
    scene.change_focus(scene.focus_mesh, scene.focus_index);
    render(index);
}

void Env::add_mesh(const std::string& name) {
    unsigned int VBO, VAO, EBO;
    VBOs.push_back(VBO);
    VAOs.push_back(VAO);
    EBOs.push_back(EBO);
    scene.add_mesh(name);
    render(scene.meshes.size()-1);
}

void Env::delete_mesh() {
    VBOs.erase(VBOs.begin()+scene.focus_index);
    VAOs.erase(VAOs.begin()+scene.focus_index);
    EBOs.erase(EBOs.begin()+scene.focus_index);
    scene.delete_mesh();
    render_all();
}

void Env::save_mesh(const std::string& filename) const {
    if (scene.focus_mesh == nullptr)
        std::cerr << "Save Mesh Error: NO MESH SELECTED\n";
    else
        scene.focus_mesh->to_dot_obj(filename);
}

void Env::update_camera() {
    scene.camera.update_cam(Point3(cameraPos.x, cameraPos.y, cameraPos.z));
    //std::cout << "Camera Center : " << scene.camera.center << "\n";
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

void Env::draw_data(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection, int mesh_index) {
    glUseProgram(shaderProgram);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int positionDec = glGetUniformLocation(shaderProgram, "positionDec");
    if (scene.focus_index == mesh_index)
        glUniform3f(positionDec, scene.dec_x, scene.dec_y, scene.dec_z);
    else
        glUniform3f(positionDec, 0, 0, 0);

    unsigned int cameraPosLoc = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

    unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    auto light_center = scene.lights[0]->center;
    glUniform3f(lightPosLoc, light_center.x, light_center.y, light_center.z);

    unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    auto light_color = scene.lights[0]->color;
    glUniform3f(lightColorLoc, light_color.r, light_color.g, light_color.b);

    unsigned int lightPower = glGetUniformLocation(shaderProgram, "lightPower");
    auto light_power = scene.lights[0]->power;
    glUniform1f(lightPower, light_power);

    // ka kd ks shininess
    unsigned int materialAttrLoc = glGetUniformLocation(shaderProgram, "materialAttr");
    if (scene.focus_index != mesh_index) {
        Texture material = scene.meshes[mesh_index]->faces[0]->texture.material.texture;
        glUniform3f(materialAttrLoc, material.ns, material.kd, material.ks);
    }
    else
        glUniform3f(materialAttrLoc, scene.ns, scene.kd, scene.ks);

    glBindVertexArray(VAOs[mesh_index]);
    glDrawElements(GL_TRIANGLES, scene.meshes[mesh_index]->faces.size() * 3, GL_UNSIGNED_INT, 0);
}

void Env::draw_grid(unsigned int shaderProgram, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    glUseProgram(shaderProgram);

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
    // Red Line for X
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

    // Green Line for Z
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