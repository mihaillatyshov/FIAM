layout (location = 0) out vec4 color;

in vec2 v_UV;

uniform sampler2D u_Texture;

void main()
{
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    color = texture2D(u_Texture, v_UV);
}
