precision mediump float;

in vec2 vs_texpos;
out vec4 color;
uniform sampler2D Tex1;

void main()
{
  vec4 tmp = texture(Tex1, vs_texpos);
  if (tmp.a < 1.0)
    discard;
  color = tmp;
}
