#include "light.hh"
#include "imgui.h"

Light::Light(LightType lightType, glm::vec3 position, glm::vec3 color, float intensity) :
    lightType_(lightType),
    position_(position),
    color_(color),
    intensity_(intensity)
{}

glm::vec3 Light::position() const {
    return position_;
}

glm::vec3 Light::color() const {
    return color_;
}

float Light::intensity() const {
    return intensity_;
}

void Light::colorModulator() {
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
    ImGui::ColorPicker4("Albedo##4", (float*)&color_, flags);
}

void Light::positionModulator() {
    ImGui::SliderFloat("Light X", &position_.x, -5.0, 5.0);
    ImGui::SliderFloat("Light Y", &position_.y, -5.0, 5.0);
    ImGui::SliderFloat("Light Z", &position_.z, -5.0, 5.0);
}

void Light::intensityModulator() {
    ImGui::SliderFloat("Power", &intensity_, 0, 200);
}