#extension GL_ARB_bindless_texture : enable

layout (location = 0) out vec4 color;


in vec4 v_Position;
in vec2 v_UV;
in vec3 v_Normal;
flat in int v_MID;
flat in int v_TransformID;
flat in int InstanceID;

layout(std430, binding = 1) buffer Materials { uvec2 Material[]; };

//#texture

struct DirLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_ViewPos;
uniform DirLight u_DirLight;

float CalcSpec(int mid, vec3 normal, vec3 lightDir, vec3 viewDir);

vec3  CalcDirLight(int mid, DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    const float ColorCoef = 16.0f;
	vec3 FragPos = v_Position.xyz;
	vec3 ViewDir = normalize(u_ViewPos - FragPos);
    //color = texture(u_TestTexture, v_UV);
	//color = Material[v_MID % 4];
    if (Material[v_MID % 4] == 0)
        color = vec4(1.0f);
    else
        color = texture2D(sampler2D(Material[v_MID % 4]), v_UV);

    //color = vec4(CalcDirLight(v_MID, u_DirLight, v_Normal, ViewDir), 1.0f);
    //color = vec4(0.0f, 0.0f, v_TransformID / ColorCoef, 1.0f);
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//color.a = GetTextureColor(material[v_MID].DiffuseTID).a * material[v_MID].DiffuseColor.a;
}


vec3 CalcDirLight(int mid, DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(lightDir, normal), 0.0);

    float spec = CalcSpec(mid, normal, lightDir, viewDir);

    vec3 ambient;//  = light.ambient  * ambi * material[mid].DiffuseColor.xyz;
    vec3 diffuse;//  = light.diffuse  * diff * material[mid].DiffuseColor.xyz;
    vec3 specular = light.specular * spec * vec3(1.0f);

    //ambient *= GetTextureColor(material[mid].DiffuseTID).xyz;
    //diffuse *= GetTextureColor(material[mid].DiffuseTID).xyz;
    //specular *= getColor(materials[mid].specular_tid);

	//return (GetTextureColor(material[mid].DiffuseTID) * material[mid].DiffuseColor).xyz;
    return (ambient + diffuse + specular);
}


float CalcSpec(int mid, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    if (dot(normal, lightDir) < 0.0f)
        return 0.0f;
    vec3 halfwayDir = normalize(lightDir + viewDir);
	float shininess = 100.0f;
    return pow(max(dot(normal, halfwayDir), 0.0), shininess);
}
