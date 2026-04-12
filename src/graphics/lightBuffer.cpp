#include "lightBuffer.hh"
#include "imgui.h"
#include "scene/modifier.hh"

LightBuffer::LightBuffer() {
    lights_.reserve(2);
    lightsNames_.reserve(2);
    addLight({5.0f,5.0f,5.0f},
             {1.0f,0.0f,1.0f},
             100.0f);
    addLight({-5.0f,-5.0f,-5.0f},
             {0.0f,1.0f,1.0f},
             50.0f);

    createVBO(getShaderLights());
}

LightBuffer::~LightBuffer() {
    if(id_)
        glDeleteBuffers(1, &id_);

    for (auto light : lights_) {
        delete light;
    }
    this->lights_.clear();
}

u32 LightBuffer::count() const {
    return vboSize_;
}

std::vector<Shader::PointLight> LightBuffer::getShaderLights() const {
    std::vector<Shader::PointLight> shaderLights;
    shaderLights.reserve(lights_.size());
    for (int i = 0; i < lights_.size(); ++i) {
        if (i != modifierLightIndex_)
            shaderLights.push_back(lights_[i]->getShaderStruct());
    }

    return shaderLights;
}

Light *LightBuffer::getModifyingLight() const {
    if (modifierLightIndex_ == -1)
        return nullptr;

    return lights_[modifierLightIndex_];
}

void LightBuffer::createVBO(const std::vector<Shader::PointLight>& shaderLights) {
    vboSize_ = shaderLights.size();
    glCreateBuffers(1, &id_);

    GLUsage_ = Graphics::bufferUsageToGL(BufferUsage::Storage);
    glNamedBufferData(id_, sizeof(Shader::PointLight) * vboSize_, shaderLights.data(), GL_STATIC_DRAW);
}

void LightBuffer::updateAndReallocVBO() {
    std::vector<Shader::PointLight> shaderLights = getShaderLights();

    vboSize_ = shaderLights.size();
    glNamedBufferData(id_, sizeof(Shader::PointLight) * vboSize_, shaderLights.data(), GL_STATIC_DRAW);
    Graphics::checkOpenGLError("Error LightBuffer : glNamedBufferSubData");
}

void LightBuffer::updateVBO() {
    std::vector<Shader::PointLight> shaderLights = getShaderLights();

    glNamedBufferSubData(id_, 0, sizeof(Shader::PointLight) * vboSize_, shaderLights.data());
    Graphics::checkOpenGLError("Error LightBuffer : glNamedBufferSubData");
}

void LightBuffer::bind(int index) {
    glBindBufferBase(GLUsage_, index, id_);
}

void LightBuffer::addLight(glm::vec3 position, glm::vec3 color, float intensity) {
    Light *light = new Light(LightType::PointLight, position, color, intensity);
    lights_.push_back(light);
    lightsNames_.push_back(light->name());
}

void LightBuffer::deleteLight(int index) {
    lights_.erase(lights_.begin() + index);
    lightsNames_.erase(lightsNames_.begin() + index);
}

void LightBuffer::lightModulator() {
    if (ImGui::Button("+##addNewLight")) {
        addLight({0.0f,5.0f,0.0f},
                 {1.0f,1.0f,1.0f},
                 50.0f);
        modifierLightIndex_ = static_cast<int>(lightsNames_.size() - 1);
        updateAndReallocVBO();
    }
    ImGui::SameLine();
    if (ImGui::Combo("Lights##listLights", &modifierLightIndex_,
                 lightsNames_.data(), static_cast<int>(lightsNames_.size())))
    {
        if (modifierLightIndex_ >= 0) {
            updateAndReallocVBO();
        }
    }
    if (modifierLightIndex_ >= 0) {
        lights_[modifierLightIndex_]->colorModulator();
        lights_[modifierLightIndex_]->positionModulator();
        lights_[modifierLightIndex_]->intensityModulator();

        if (ImGui::Button("Delete##deleteLight")) {
            deleteLight(modifierLightIndex_);
            modifierLightIndex_ = -1;
            updateAndReallocVBO();
        }
    }
}
