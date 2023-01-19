
out vec4 		v_Position;
out vec2 		v_UV;
out vec3 		v_Normal;
flat out int	v_MID;
flat out int    v_TransformID;
flat out int	InstanceID;

uniform mat4 u_ProjectionMatrix	= mat4(1.0);
uniform mat4 u_ViewMatrix		= mat4(1.0);
//uniform samplerBuffer u_TransformTexture;
//
//mat4 GetMatrix(int id)
//{
//    return mat4(texelFetch(u_TransformTexture, id * 4 + 0),
//                texelFetch(u_TransformTexture, id * 4 + 1),
//                texelFetch(u_TransformTexture, id * 4 + 2),
//                texelFetch(u_TransformTexture, id * 4 + 3));
//}

void main()
{
    v_UV = a_TexCoord;
    v_Normal = a_Normal;
    v_MID = a_MaterialId;
    v_TransformID = a_TransformId;
    InstanceID = gl_InstanceID;
	//InstanceID = gl_BaseInstance;
    
    //mat4 ModelTransform = GetMatrix(a_TransformId);
    mat4 ModelTransform = mat4(1.0);
    v_Position = ModelTransform * vec4(a_Position, 1.0f);
    vec4 Position = u_ProjectionMatrix * u_ViewMatrix * ModelTransform * (vec4(a_Position, 1.0f) + vec4(0.0f, gl_InstanceID / 2.0f, 0.0f, 0.0f));
    gl_Position = Position;
}
