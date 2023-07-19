#version 410 core

out vec2 tc;

void main()
{
   // list for texture coordinates of the verticies of the quad, from the origin + 1 .
  vec2 patchTexCoords[] = vec2[](vec2(1.0, 1.0), vec2(2.0, 1.0), vec2(1.0, 2.0), vec2(2.0, 2.0));

   // Calculate x and y of current instance bounded by 64x64 grid.
  int x_coord = gl_InstanceID % 64;
  int y_coord = gl_InstanceID / 64;

   // Calculate ouput texture coordinate based on current instance.
  tc = vec2((x_coord + patchTexCoords[gl_VertexID].x) / 64.0, (64 - y_coord + patchTexCoords[gl_VertexID].y) / 64.0);

   // Set output position.
  gl_Position = vec4(tc.x - 0.5, 0.0, (1.0 - tc.y) - 0.5, 1.0);
}
