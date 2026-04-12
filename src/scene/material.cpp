#include "material.hh"

Material::Material(MATERIAL_TYPE materialType, const glm::vec3& color, const glm::vec2& pbrFactor) {
    static int materialNumber = 0;
    color_ = ImVec4(color.x, color.y, color.z, 1.0);
//    materialType = _materialType;
//    phong_factor = glm::vec3(0.0, 0.0, 0.0);
    pbrFactor_ = pbrFactor;
    name_ = "material_" + std::to_string(materialNumber++);
    materialType_ = materialType;
}

const char* Material::name() const {
    return name_.c_str();
}

glm::vec3 Material::color() const {
    return {color_.x, color_.y, color_.z};
}

glm::vec2 Material::pbrFactor() const {
    return pbrFactor_;
}

void Material::colorModulator() {
    static bool alpha_preview = true;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = true;
    static bool options_menu = true;
    static bool hdr = false;
    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0)
            | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop)
            | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0))
            | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

    static bool side_preview = true;
    static int display_mode = 0;
    static int picker_mode = 0;
    ImGui::Checkbox("With Side Preview", &side_preview);

//    ImGui::Combo("Display Mode", &display_mode, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
    ImGuiColorEditFlags flags = misc_flags;
    if (!side_preview)     flags |= ImGuiColorEditFlags_NoSidePreview;
    if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
    if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
//    if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;
//    if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;
//    if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
    flags |= ImGuiColorEditFlags_DisplayHSV;
//    if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;
    ImGui::ColorPicker4("Albedo##materialColor", (float*)&color_, flags);
}

void Material::pbrFactorModulator() {
    ImGui::SliderFloat("Roughness##materialRoughness", &pbrFactor_.x, 0, 1);
    ImGui::SliderFloat("Metalness##materialMetalness", &pbrFactor_.y, 0, 1);
}