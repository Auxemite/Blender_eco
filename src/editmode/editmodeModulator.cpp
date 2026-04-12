#include "editmode/editmodeScene.hh"

void EditMode::EditModeScene::editModeModulator(Scene& scene) {
    if (ImGui::Checkbox("Editmode##toggleEditmode", &Env::editmode)) {
        toggleEditMode(scene, *this);
    }
    if (Env::editmode) {
        ImGui::SameLine();
        ImGui::Text(" | Selection Mode : ");
        ImGui::SameLine();
        ImGui::RadioButton("Face##editmodeFace", reinterpret_cast<int *>(&editmodeType_), 0);
        ImGui::SameLine();
        ImGui::RadioButton("Edge##editmodeEdge", reinterpret_cast<int *>(&editmodeType_), 2);
        ImGui::SameLine();
        ImGui::RadioButton("Vertex##editmodeVertex", reinterpret_cast<int *>(&editmodeType_), 1);
    }
}

void EditMode::EditModeScene::modifierModulator() {
    modifier_.positionModulator();
    modifier_.rotationModulator();
    modifier_.scaleModulator();
}