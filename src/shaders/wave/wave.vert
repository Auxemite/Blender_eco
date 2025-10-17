#version 450 core

in vec3 aPos;
in vec3 aColor;

uniform vec3 positionDec;
//out vec3 tessctrl_color;

void main() {
  gl_Position = vec4(aPos[0] + positionDec[0], aPos[1] + positionDec[1], aPos[2] + positionDec[2], 1.0f);
//  tessctrl_color = aColor;
}
