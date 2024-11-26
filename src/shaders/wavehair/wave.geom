#version 450 core
layout(triangles) in;
layout(line_strip, max_vertices = 50) out;

in vec3 position[];
//in vec3 normalSmoothG[3];
//out vec3 color_frag;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 positionDec;
uniform int fur_length;
uniform float fur_size;

void main() {
  vec3 v1 = position[1] - position[0];
  vec3 v2 = position[2] - position[0];
  vec3 normal = normalize(cross(v1, v2));
  //vec3 normal = (normalSmoothG[0] + normalSmoothG[1] + normalSmoothG[2]) / 3.0;
  vec4 origin = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
  vec3 source = vec3(origin);
//  vec3 color_mean =  (color[0] + color[1] + color[2]) / 3.0;

  float step = fur_size / fur_length;
  for(float i = 0.0; i < fur_size; i += step) {
//      color_frag = color_mean;
      gl_Position = projection * view * model * vec4(source, 1.0);
      EmitVertex();
      source = normal * i + origin.xyz + vec3(0.0, -1.0/2.0 * i * i * 9.81 * 9.8, 0.0);
  }
  EndPrimitive();
}
