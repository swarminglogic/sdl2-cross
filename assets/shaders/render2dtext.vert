precision mediump float;

layout (location = 0) in vec2 quadVertices;

uniform vec2 charBoxDimensions;
uniform ivec2 Viewport;

// xy: quad offset
// zw: texture offset
uniform vec4[64] offset;

out vec2 vs_texpos;

void main(){
  gl_Position = vec4((quadVertices + offset[gl_InstanceID].xy) /
                     vec2(Viewport.xy),
                     -1.0, 1);

  vec2[4] foo = vec2[4](vec2(0, 1), vec2(1, 1), vec2(0, 0), vec2(1, 0));
  vs_texpos = offset[gl_InstanceID].zw + foo[gl_VertexID] * charBoxDimensions;
}

