#version 300 es
precision mediump float;

layout(location = 0) in vec3 vpos;
layout(location = 1) in vec2 tpos;

out vec2 vs_texpos;

uniform ivec2 Viewport;

void main(){
  gl_Position = vec4(vec2(vpos.x, vpos.y) / vec2(Viewport.xy), -1.0, 1.0);
  vs_texpos = tpos;
}
