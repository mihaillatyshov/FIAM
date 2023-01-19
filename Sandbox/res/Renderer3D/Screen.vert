
out vec2 		v_UV;

void main()
{
    v_UV = a_Position;
    gl_Position = vec4(a_Position, 0.0f, 1.0f);
}
