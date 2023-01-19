varying vec3 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = vec4(a_Position, 0.0f, 1.0f);
}