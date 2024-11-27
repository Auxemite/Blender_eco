// Fragment shader output
out vec4 outFragColor;
in vec3 vNormalWS;
in vec3 ViewDirectionWS;
in vec3 in_positionWS;

// Colors Textures
uniform sampler2D uTextureBRDF;
uniform sampler2D uTextureDiffuse;
uniform sampler2D uTextureSpec; 
uniform bool set_ibl;

// Uniforms
struct Material
{
  vec3 albedo;
  vec2 properties; // metaless, roughness
};
uniform Material uMaterial;

// Lights
struct Light
{
  vec3 position;
  vec3 color;
  float intensity;
};
uniform int lightCount;
uniform Light uLights[4];
float pi = 3.14159265359;

// From three.js
vec4 sRGBToLinear( in vec4 value ) {
	return vec4( mix( pow( value.rgb * 0.9478672986 + vec3( 0.0521327014 ), vec3( 2.4 ) ), value.rgb * 0.0773993808, vec3( lessThanEqual( value.rgb, vec3( 0.04045 ) ) ) ), value.a );
}

vec3 Aces(vec3 x)
{
  float a = 2.51f;
  float b = 0.03f;
  float c = 2.43f;
  float d = 0.59f;
  float e = 0.14f;
  return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

// From three.js
vec4 LinearTosRGB( in vec4 value ) {
	return vec4( mix( pow( value.rgb, vec3( 0.41666 ) ) * 1.055 - vec3( 0.055 ), value.rgb * 12.92, vec3( lessThanEqual( value.rgb, vec3( 0.0031308 ) ) ) ), value.a );
}

float rdot(vec3 a, vec3 b) {
  return max(0.0, dot(a, b));
}

float distributionGGX(vec3 vNormalWS, vec3 lightDir, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float dotNL = rdot(lightDir, vNormalWS);
  float dotNL2 = dotNL * dotNL;
  
  float denom = (dotNL2 * (a2 - 1.0) + 1.0);
  return a2 / (pi * denom * denom);
}

float subGeometrySchlickGGX(float dotNV, float roughness)
{
  float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
  return dotNV / (dotNV * (1.0 - k) + k);
}

float geometrySchlickGGX(vec3 vNormalWS, vec3 lightDir, vec3 ViewDirectionWS, float roughness)
{
  float dotNL = rdot(vNormalWS, lightDir);
  float dotNV = rdot(vNormalWS, ViewDirectionWS);
  return subGeometrySchlickGGX(dotNL, roughness) * subGeometrySchlickGGX(dotNV, roughness);
}

float fresnelSchlick(float dotVH, float f0)
{
  return f0 + (1.0 - f0) * pow(1.0 - dotVH, 5.0);
}

vec3 RGBMDecode(vec4 rgbm) {
  return 6.0 * rgbm.rgb * rgbm.a;
}

vec2 cartesianToPolar(vec3 cartesian) {
    // Compute azimuthal angle, in [-PI, PI]
    float phi = atan(cartesian.z, cartesian.x);
    // Compute polar angle, in [-PI/2, PI/2]
    float theta = asin(cartesian.y);
  
    return vec2(phi, theta);
}

vec2 normalizeUV(vec2 uv) {
  return vec2(uv.x / (2.0 * pi) + 0.5, uv.y / pi + 0.5);
}

vec3 getTextureDiffuseColor(vec3 normal, sampler2D uTexture) {
  vec2 vTextureCoord = normalizeUV(cartesianToPolar(normal).xy);
  return RGBMDecode(texture(uTexture, vTextureCoord));
}

vec3 getTextureSpecColor(vec3 normal, sampler2D uTexture) {
  vec2 coord = normalizeUV(cartesianToPolar(normal).xy);

  float roughness = float(int(uMaterial.properties[1] * 6.0)); // 2 <- 2.7
  float alpha_low = uMaterial.properties[1] * 6.0 - roughness; // 0.7
  float alpha_high = 1.0 - alpha_low; // 0.3

  float scale = pow(0.5, roughness);
  vec2 coord_low = vec2(coord.x * scale, coord.y * 0.5 * scale + (1.0 - scale));

  scale = pow(0.5, roughness + 1.0);
  vec2 coord_high = vec2(coord.x * scale, coord.y * 0.5 * scale + (1.0 - scale));

  return RGBMDecode(texture(uTexture, coord_low) * alpha_high + texture(uTexture, coord_high) * alpha_low);
}

vec3 brdf(vec3 normal, float roughness, float metaless) 
{
  vec3 irradiance = vec3(0.0);
  for (int i = 0; i < lightCount; i++) {
    // float ambient = uLights[i].intensity;
    vec3 lightDir = normalize(uLights[i].position - in_positionWS);

    //! specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 h = normalize(lightDir + ViewDirectionWS);
    float D = distributionGGX(normal, h, roughness);
    // float D = distributionGGX(normal, lightDir, roughness);
    float G = geometrySchlickGGX(normal, lightDir, ViewDirectionWS, roughness);
    float ks = fresnelSchlick(rdot(h, ViewDirectionWS), 0.04 + metaless);
    // float ks = fresnelSchlick(rdot(reflectDir, ViewDirectionWS), 0.04);
    float spec = G * D * ks / (4.0 * dot(normal, ViewDirectionWS) * dot(normal, lightDir) + 1.0);

    //! diffuse
    vec3 diffuse = (1.0 - ks) * (uMaterial.albedo) * rdot(normal, lightDir) / pi;
    diffuse *= (1.0 - metaless);
    
    vec3 lightSample = uLights[i].color * uLights[i].intensity;
    irradiance += (diffuse + vec3(spec)) * lightSample * dot(normal, lightDir);
  }
  return irradiance;
}

vec3 ibl(vec3 normal, float roughness, float metaless)
{
  vec3 reflectDir = -reflect(ViewDirectionWS, normal);
  // texture
  vec3 vTextureDiffuseColor = getTextureDiffuseColor(normal, uTextureDiffuse);
  vec3 vTextureSpecColor = getTextureSpecColor(reflectDir, uTextureSpec);
  vec3 vTextureBRDFColor = texture(uTextureBRDF, vec2(rdot(normal, ViewDirectionWS), 1.0 - roughness)).rgb;

  vec3 h = normalize(reflectDir + ViewDirectionWS);
  float ks = fresnelSchlick(rdot(h, ViewDirectionWS), 0.04 + metaless);
  vec3 spec = (ks * vTextureBRDFColor.r + vTextureBRDFColor.g) * vTextureSpecColor;

  vec3 diffuse = (1.0 - ks) * vTextureDiffuseColor; // * rdot(vNormalWS, lightDir) / pi;
  diffuse *= (1.0 - metaless);
  return diffuse * uMaterial.albedo + spec;
}

void main()
{
  vec3 normal = normalize(vNormalWS);
  float metaless = uMaterial.properties[0];
  float roughness = uMaterial.properties[1];

  vec3 irradiance = vec3(0.0);
  if (!set_ibl)
    irradiance = brdf(normal, roughness, metaless);
  else
    irradiance = ibl(normal, roughness, metaless);
  
  // **DO NOT** forget to do all your computation in linear space.
  vec3 albedo = sRGBToLinear(vec4(irradiance, 1.0)).rgb;
  albedo = Aces(albedo); // HDR

  // **DO NOT** forget to apply gamma correction as last step.
  outFragColor.rgba = LinearTosRGB(vec4(albedo, 1.0));
}
