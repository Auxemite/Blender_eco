#version 450 core

struct PointLight {
    vec3 position;
    float radius;
    vec3 color;
    float padding_1;
};

in vec3 fragColor;
in vec2 uv;
in vec4 fragPos;

out vec4 FragColor;

layout(binding = 1) buffer PointLights {
    PointLight point_lights[];
};

layout(binding = 0) uniform sampler2D tex;

uniform vec3 cameraPos;
uniform vec3 unicolor;
uniform vec2 pbrFactor;
uniform vec3 materialAlbedo;

uniform vec3 modifierLightPos;
uniform vec3 modifierLightColor;
uniform float modifierLightRadius;

uniform uint lightNumber;

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

vec3 brdf(vec3 normal, float roughness, float metaless, vec3 ViewDirectionWS,
            vec3 lightPos, float lightPower, vec3 lightColor)
{
    vec3 irradiance = materialAlbedo * lightPower / 500.0;
    vec3 lightDir = normalize(lightPos - fragPos.xyz);

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
    vec3 diffuse = (1.0 - ks) * materialAlbedo * rdot(normal, lightDir) / pi;
    diffuse *= (1.0 - metaless) * lightPower / 20.0;

    vec3 lightSample = (lightColor * lightPower / 20.0);
    irradiance += ((diffuse + spec * lightSample) * rdot(normal, lightDir));
    return irradiance;
}

void main() {
    vec4 dpdx = dFdx(fragPos);
    vec4 dpdy = dFdy(fragPos);
    vec3 normal = normalize(cross(dpdx.xyz, dpdy.xyz));

    vec3 ViewDirectionWS = normalize(cameraPos - fragPos.xyz);
    vec3 irradiance = brdf(normal, pbrFactor.x, pbrFactor.y, ViewDirectionWS,
            modifierLightPos, modifierLightRadius, modifierLightColor);
    for(uint i = 0; i != lightNumber; ++i) {
        PointLight pl = point_lights[i];
        irradiance += brdf(normal, pbrFactor.x, pbrFactor.y, ViewDirectionWS, pl.position, pl.radius, pl.color);
    }

    vec3 albedo = sRGBToLinear(vec4(irradiance, 1.0)).rgb;
    albedo = Aces(albedo); // HDR

    vec4 texColor = texture(tex, uv);
    FragColor = vec4(albedo * texColor.rgb, 1.0f);
//    FragColor = vec4(irradiance, 1.0f);
}
