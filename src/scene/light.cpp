#include "light.hh"
#include "imgui.h"
#include "gui/guiUtils.hh"

Light::Light(LightType lightType, glm::vec3 position, glm::vec3 color, float intensity) :
    lightType_(lightType),
    position_(position),
    color_(color),
    intensity_(intensity)
{
    static int lightNumber = 0;
    name_ = "Light" + std::to_string(lightNumber++);
}

const char* Light::name() const {
    return name_.c_str();
}

glm::vec3 Light::position() const {
    return position_;
}

glm::vec3 Light::color() const {
    return color_;
}

float Light::intensity() const {
    return intensity_;
}

Shader::PointLight Light::getShaderStruct() const {
    return {position_, intensity_, color_, 0};
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
//    ImGui::Checkbox(Gui::getLabel("With Side Preview##", name_), &side_preview);

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
    const std::string label = "Albedo##albedo" + name_;
    ImGui::ColorPicker4(label.c_str(), (float*)&color_, flags);
}

void Light::positionModulator() {
    const std::string labelPosX = "X##positionX" + name_;
    const std::string labelPosY = "Y##positionY" + name_;
    const std::string labelPosZ = "Z##positionZ" + name_;
    ImGui::SliderFloat(labelPosX.c_str(), &position_.x, -5.0, 5.0);
    ImGui::SliderFloat(labelPosY.c_str(), &position_.y, -5.0, 5.0);
    ImGui::SliderFloat(labelPosZ.c_str(), &position_.z, -5.0, 5.0);
}

void Light::intensityModulator() {
    const std::string label = "Power##power" + name_;
    ImGui::SliderFloat(label.c_str(), &intensity_, 0, 200);
}