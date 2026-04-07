#pragma once

#include "graphics/ray.hh"

namespace EditMode {
    class EditModeRay : Ray {
        public:
            explicit EditModeRay(glm::vec3 cameraPos);

            void rayCasting(EditMode::EditModeScene& editModeScene, float width, float height);

        private:
    };
}