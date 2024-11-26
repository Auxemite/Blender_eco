#version 450 core

layout(quads, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float anim_time;

//in vec3 tesseval_color[];
//out vec3 frag_color;
out vec4 p;
out float delta;

void main() {
    //vec3 barycentric = gl_TessCoord;
    //p = barycentric.x * gl_in[0].gl_Position +
    //    barycentric.y * gl_in[1].gl_Position +
    //    barycentric.z * gl_in[2].gl_Position;
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[0].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    p = mix(p1, p2, gl_TessCoord.y);

    float waveAmplitude = 0.4;
    float waveFrequency = 5.0;

    float sin_value = waveAmplitude * sin(waveFrequency * (p.x) + anim_time);
    float y1 = waveAmplitude * sin(waveFrequency * (p.x - 0.2) + anim_time);
    float y2 = waveAmplitude * sin(waveFrequency * (p.x + 0.2) + anim_time);

    p.y += sin_value;

    delta = abs(y2 - y1) * 0.5 + 0.5;

    gl_Position = projection * view * model * p;
//    frag_color = tesseval_color[0];
}