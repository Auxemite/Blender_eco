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
uniform float anim_time;
uniform vec3 wave_amplitude;
uniform vec3 wave_frequency;
uniform vec3 dep;
uniform vec3 dep2;

out float fragStatus;

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

      float depx = dep.x * source.y + dep2.x * source.z;
      float depy = dep.y * source.x + dep2.y * source.z;
      float depz = dep.z * source.x + dep2.z * source.y;

      float sin_value_x = wave_amplitude.x * depx * sin(wave_frequency.x * depx + anim_time);
      float sin_value_y = wave_amplitude.y * depy * sin(wave_frequency.y * depy + anim_time);
      float sin_value_z = wave_amplitude.z * depz * sin(wave_frequency.z * depz + anim_time);

      fragStatus = 1.0f;
      if (wave_amplitude.y * depy * sin(wave_frequency.y * depy + (anim_time + 0.5f)) < sin_value_y)
            fragStatus = 1.2f;
      source += vec3(sin_value_x, sin_value_y, sin_value_z);
      vec3 depvector = vec3(dep.y + dep.z, dep.x + dep2.z, dep2.x + dep2.y);
      source += (-0.2f) * depvector;

      gl_Position = projection * view * model * vec4(source, 1.0);
      EmitVertex();
      float gravity = -1.0/2.0 * i * i * 9.81 * 9.8;

      source = normal * i + origin.xyz + gravity * depvector;
  }
  EndPrimitive();
}
