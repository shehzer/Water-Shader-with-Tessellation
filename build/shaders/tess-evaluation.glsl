#version 410

layout (quads, fractional_even_spacing, ccw) in;


struct Wave {
    float w;
    vec2 D;
    float A;
    float phi;
    float Q;
};

in vec2 tc_out[];
out vec2 texCoords;
out vec3 fragPos;
// Define Wave structure for Gerstner Waves
const int NUM_WAVES = 4;

uniform mat4 mvp;
uniform mat4 model;
uniform sampler2D heightMap1;
uniform float depth;
uniform float wavesOffset;
uniform Wave waves[NUM_WAVES];
uniform float time;

float calcHeight(vec2 tc);

// Return GerstnerWave
vec3 GerstnerWave(vec3 pos, Wave wave) {
    float wxD = wave.w * dot(wave.D, pos.xz);
    float Ai = wave.A * cos(wxD + wave.phi * time);
    float dAi = -wave.A * wave.w * sin(wxD + wave.phi * time);
    return vec3(wave.Q * Ai * wave.D.x + dAi * pos.x, Ai, wave.Q * Ai * wave.D.y + dAi * pos.z);
}

void main()
{
  // Calculate texture co-ordinate and texture height offset for the tesselated points based on displacement map.
  vec2 tc = vec2(tc_out[0].x + (gl_TessCoord.x) / 64.0, tc_out[0].y + (1.0 - gl_TessCoord.y) / 64.0);
  tc = vec2(tc.x*0.75, tc.y*0.75);
  // Get tessellated point for each 64x64 grid.
  vec4 finalTessellatedPoint = vec4(gl_in[0].gl_Position.x + gl_TessCoord.x / 64.0, calcHeight(tc), gl_in[0].gl_Position.z + gl_TessCoord.y / 64.0, 1.0);
  // Get final position of each vertex
  gl_Position = mvp * finalTessellatedPoint;
  // Pass the fragment position and texture cordinates to next stage
  fragPos = vec3(model * finalTessellatedPoint);
  texCoords = tc;

}

// Return height of vertex based on height map and Gerstner waves.
float calcHeight(vec2 tc) {
    float height1 = texture(heightMap1, vec2(tc.x, tc.y)).r;
    float wavesHeight = texture(heightMap1, vec2(tc.x, tc.y + wavesOffset)).r * 4;
    height1 = mix(height1, wavesHeight, 0.175);
    height1 *= depth;

    vec3 pos = vec3(tc.x, height1, tc.y);
    for (int i = 0; i < NUM_WAVES; ++i) {
        pos += GerstnerWave(pos, waves[i]);
    }

    return pos.y;
}

