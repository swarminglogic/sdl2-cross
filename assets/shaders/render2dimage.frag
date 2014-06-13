#version 300 es
precision mediump float;

in vec2 vs_texpos;
out vec4 color;
uniform sampler2D Tex1;

void main()
{
  color = texture(Tex1, vs_texpos);
}
