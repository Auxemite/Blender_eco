#version 450 core

layout(triangles, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float anim_time;
uniform vec3 wave_amplitude;
uniform vec3 wave_frequency;
uniform vec3 dep;
uniform vec3 dep2;

//in vec3 tesseval_color[];
//out vec3 frag_color;
out vec4 p;

void main() {
    vec3 barycentric = gl_TessCoord;
    p = barycentric.x * gl_in[0].gl_Position +
        barycentric.y * gl_in[1].gl_Position +
        barycentric.z * gl_in[2].gl_Position;

    float depx = dep.x * p.y + dep2.x * p.z;
    float depy = dep.y * p.x + dep2.y * p.z;
    float depz = dep.z * p.x + dep2.z * p.y;

    float sin_value_x = wave_amplitude.x * sin(wave_frequency.x * depx + anim_time);
    float sin_value_y = wave_amplitude.y * sin(wave_frequency.y * depy + anim_time);
    float sin_value_z = wave_amplitude.z * sin(wave_frequency.z * depz + anim_time);

    p += vec4(sin_value_x, sin_value_y, sin_value_z, 0.0f);

    gl_Position = projection * view * model * p;
//    frag_color = tesseval_color[0];
}