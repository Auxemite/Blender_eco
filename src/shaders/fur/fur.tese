#version 450 core

layout(triangles, equal_spacing, ccw) in;

//in vec3 tesseval_color[];
//out vec3 frag_color;
out vec3 position;

void main() {
    vec3 barycentric = gl_TessCoord;
    vec4 p = barycentric.x * gl_in[0].gl_Position +
        barycentric.y * gl_in[1].gl_Position +
        barycentric.z * gl_in[2].gl_Position;

//    float sin_value_x = wave_amplitude.x * sin(wave_frequency.x * (p.z) + anim_time);
//    float sin_value_y = wave_amplitude.y * sin(wave_frequency.y * (p.x) + anim_time);
//    float sin_value_z = wave_amplitude.z * sin(wave_frequency.z * (p.y) + anim_time);

//    float sin_value_x = wave_amplitude.x * sin(wave_frequency.x * (p.y) + anim_time);
//    float sin_value_y = wave_amplitude.y * sin(wave_frequency.y * (p.z) + anim_time);
//    float sin_value_z = wave_amplitude.z * sin(wave_frequency.z * (p.x) + anim_time);
//
//    p += vec4(sin_value_x, sin_value_y, sin_value_z, 0.0f);

    position = vec3(p.x, p.y, p.z);
    gl_Position = p;
//    frag_color = tesseval_color[0];
}