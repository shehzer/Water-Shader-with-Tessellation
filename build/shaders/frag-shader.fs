#version 410 core
out vec4 fragColor;

in vec2 texCoords;
in vec3 fragPos;


struct Light
{
  vec3 direction;
  vec3 diffuse;
  vec3 ambient;
  vec3 specular;
};

const vec3 COLOR_OF_WATER = vec3(0.678, 0.847, 0.902);
const int SHINYFACTOR = 256;
const float NORM_HEIGT = 2.0;

uniform sampler2D heightMap1;
uniform sampler2D water;
uniform Light light;
uniform float depth;
uniform float wavesOffset;
uniform vec3 viewPos;

vec3 getNormal();
vec3 getHeight();
vec3 getLight(Light light, vec3 material, vec3 viewDir, vec3 normal);


void main()
{
  vec3 viewingDirection = normalize(viewPos - fragPos);

  vec3 normal = getNormal();
  vec3 height = getHeight();
  height.r = max(14 * height.r, 1.0);
  // Sample two water textures with different co-ordinates and blend them.
  vec3 waterFar = texture(water, vec2(texCoords.x, texCoords.y + wavesOffset/5)).rgb;
  vec3 waterClose = texture(water, vec2(texCoords.x/5, -texCoords.y/5 - wavesOffset/10)).rgb;

  // Blend them 50/50
  vec3 water = mix(waterFar, waterClose, 0.5);
  vec3 material = mix(water*COLOR_OF_WATER, water, 0.35);

  // Blend material with white - emphasizing water surface
  material = material * height.rrr;
  material = mix(vec3(1.0, 1.0, 1.0), material, pow(normal.y, 2));

  // Compute final fragment colour based on Phong methodology.
  fragColor = vec4(getLight(light, material, viewingDirection, normal), 1.0);
}

// Return height given displacement map
vec3 getHeight()
{
    float height = texture(heightMap1, vec2(texCoords.x, texCoords.y)).r;
    float waveHeight = texture(heightMap1, vec2(texCoords.x, texCoords.y + wavesOffset)).r * 4;
    height = mix(height, waveHeight, 0.2);
    height *= depth;
    return vec3(height, height, height);
}

// Get ambient, diffuse and specular
vec3 getLight(Light light, vec3 material, vec3 viewDir, vec3 normal)
{
      vec3 lightDir = normalize(-light.direction);
      float difference = max(dot(lightDir, normal), 0.0);
      vec3 middle = normalize(lightDir + viewDir);
      float angle = max(dot(middle, normal), 0.0);
      float spec = pow(angle, SHINYFACTOR);

      vec3 ambient = light.ambient * material;
      vec3 diffuse = light.diffuse * material * difference;
      vec3 specular = light.specular * spec;
      return ambient + diffuse + specular;
}
// Get the normal, given displacement map.
vec3 getNormal()
{
  vec3 heightColor = texture(heightMap1, vec2(texCoords.x, texCoords.y)).rgb;
  vec3 wavesColor = texture(water, vec2(texCoords.x, texCoords.y + wavesOffset)).rgb * 4;
  vec3 normalColor = mix(heightColor, wavesColor, 0.175).rgb;
  vec3 normal = vec3(normalColor.r * 4.0 - 2.0, normalColor.b * NORM_HEIGT, normalColor.g * 4.0 - 2.0);
  return normalize(normal);
}

