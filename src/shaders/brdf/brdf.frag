#version 450 core
// Fragment shader output
out vec4 outFragColor;
in vec3 vNormalWS;
in vec3 ViewDirectionWS;
in vec3 in_positionWS;
in vec3 fragColor;

// Lights
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightPower;
uniform float metaless;
uniform float roughness;
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

vec3 brdf(vec3 normal, float roughness, float metaless)
{
    vec3 irradiance = fragColor * lightPower / 500.0;
    vec3 lightDir = normalize(lightPos - in_positionWS);

    //! specular
//    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 h = normalize(lightDir + ViewDirectionWS);
    float D = distributionGGX(normal, h, roughness);
//    float D = distributionGGX(normal, lightDir, roughness);
    float G = geometrySchlickGGX(normal, lightDir, ViewDirectionWS, roughness);
    float ks = fresnelSchlick(rdot(h, ViewDirectionWS), 0.04 + metaless);
    // float ks = fresnelSchlick(rdot(reflectDir, ViewDirectionWS), 0.04);
    float spec = G * D * ks / (4.0 * dot(normal, ViewDirectionWS) * dot(normal, lightDir) + 1.0);

    //! diffuse
    vec3 diffuse = (1.0 - ks) * fragColor * rdot(normal, lightDir) / pi;
    diffuse *= (1.0 - metaless) * lightPower / 20.0;

    vec3 lightSample = (lightColor * lightPower / 20.0);
    irradiance += ((diffuse + spec * lightSample) * rdot(normal, lightDir));
    return irradiance;
}

void main()
{
    vec3 normal = normalize(vNormalWS);

    vec3 irradiance = brdf(normal, roughness, metaless);

    vec3 albedo = sRGBToLinear(vec4(irradiance, 1.0)).rgb;
    albedo = Aces(albedo); // HDR

    outFragColor.rgba = LinearTosRGB(vec4(albedo, 1.0));
}
