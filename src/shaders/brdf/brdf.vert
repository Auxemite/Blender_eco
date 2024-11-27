// Attributes (vertex shader inputs)
in vec3 in_position;
in vec3 in_normal;
#ifdef USE_UV
  in vec2 in_uv;
#endif

// Varyings (vertex shader outputs)
out vec3 vNormalWS;
out vec3 ViewDirectionWS;
out vec3 in_positionWS;

#ifdef USE_UV
  out vec2 vUv;
#endif
float pi = 3.14159265359;

// Uniforms
struct Camera
{
  mat4 WS_to_CS; // World-Space to Clip-Space (view * proj)
  vec3 positionWS;
};
uniform Camera uCamera;

struct Model
{
  mat4 LS_to_WS; // Local-Space to World-Space
};
uniform Model uModel;

// Uniforms
struct Material
{
  vec3 albedo;
  vec2 properties; // metaless, roughness
};
uniform Material uMaterial;

void main()
{
  vec4 positionLocal = vec4(in_position, 1.0);
  gl_Position = uCamera.WS_to_CS * uModel.LS_to_WS * positionLocal;
  vNormalWS = normalize(mat3(uModel.LS_to_WS) * in_normal);
  in_positionWS = (uModel.LS_to_WS * positionLocal).xyz;
  ViewDirectionWS = normalize(uCamera.positionWS - in_positionWS);
  // ViewDirectionWS = normalize(uLights[0].position - (uModel.LS_to_WS * positionLocal).xyz);
}