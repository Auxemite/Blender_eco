#pragma once

#include <vector>
#include "glm/glm.hpp"

class Triangle {
    public:
        Triangle(glm::ivec3 vertexIndexes, glm::vec3 _normal);
        Triangle(glm::ivec3 vertexIndexes, const std::vector<glm::vec3 *>& points);

        glm::ivec3 vertexIndexes() const;
        glm::vec3 normal() const;

        float rayTriangleIntersection(std::vector<glm::vec3 *> points,
                              const glm::vec3& cameraPos,
                              const glm::vec3& rayDir);
    //    glm::vec3 normal(const glm::vec3& point);
        void normalize();

        // Usefull for mesh transformation
    //    void scale(float size) const;
    //    void scale(float size, const glm::vec3& from) const;
    //
    //    void update_normal();
    private:
        int ia_, ib_, ic_;
        glm::vec3 normal_;
};