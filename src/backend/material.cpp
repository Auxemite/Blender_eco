#include "material.hh"

Material::Material(MATERIAL_TYPE _materialType) {
    color = glm::vec3(1.0, 1.0, 1.0);
//    materialType = _materialType;
//    phong_factor = glm::vec3(0.0, 0.0, 0.0);
    pbrFactor = glm::vec2(0.0, 0.0);
}