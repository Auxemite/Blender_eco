#include "mesh_action.hh"

void move_mesh(Env& env) {
    if (env.scene.editmode) {
        static float v1 = 1.00f;
        if (ImGui::Button("Move X")) {
            env.scene.move_x(v1);
            env.render();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("X", &v1, -5, 5);

        static float v2 = 1.00f;
        if (ImGui::Button("Move Y")) {
            env.scene.move_y(v2);
            env.render();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Y", &v2, -5, 5);

        static float v3 = 1.00f;
        if (ImGui::Button("Move Z")) {
            env.scene.move_z(v3);
            env.render();
        }
        ImGui::SameLine();
        ImGui::SliderFloat("Z", &v3, -5, 5);
    }
    else {
        ImGui::SliderFloat("X", &env.scene.dec_x, -5, 5);
        ImGui::SliderFloat("Y", &env.scene.dec_y, -5, 5);
        ImGui::SliderFloat("Z", &env.scene.dec_z, -5, 5);
    }
}

void scale_mesh(Env& env) {
    static float scale = 1.00f;
    if (ImGui::Button("Scale")) {
        env.scene.scale(scale);
        env.render();
    }
    ImGui::SameLine();
    ImGui::SliderFloat("Value", &scale, 0.1f, 3);
}

void rotate_mesh(Env& env) {
    static float a1 = 0;
    ImGui::SliderAngle("RX", &a1, -90, 90);
    static float a2 = 0;
    ImGui::SliderAngle("RY", &a2, -90, 90);
    static float a3 = 0;
    ImGui::SliderAngle("RZ", &a3, -90, 90);
    if (ImGui::Button("Rotate Y")) {
        env.scene.rotate_xyz(a1, a2, a3);
        env.render();
    }
}

void extrude_mesh(Env& env) {
    static float move[3] = { 0, 0, 0};
    ImGui::SliderFloat3("X Y Z", move, -2, 2);
    if (ImGui::Button("Extrude")) {
        env.scene.extrude(move[0], move[1], move[2]);
        env.render();
    }
    static float thickness = 0;
    ImGui::SliderFloat("Thickness", &thickness, -2, 2);
    if (ImGui::Button("Extrude Along Normal")) {
        env.scene.extrude_along_normal(thickness);
        env.render();
    }
    if (ImGui::Button("Extrude Along Point")) {
        env.scene.extrude_along_points_normalized(thickness);
        env.render();
    }
}