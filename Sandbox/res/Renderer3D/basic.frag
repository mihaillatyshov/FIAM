
layout (location = 0) out vec4 color;

in vec4 v_Position;
in vec2 v_UV;
in vec3 v_Normal;
flat in int v_MID;

struct Material
{
	int DiffuseTID;
	vec4 DiffuseColor;
};

#define MAX_MATERIALS 256
#define ambi 0.9f

uniform Material material[MAX_MATERIALS];

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
	vec3 FragPos = v_Position.xyz;
	vec3 ViewDir = normalize(u_ViewPos - FragPos);
	color = vec4(CalcDirLight(v_MID, u_DirLight, v_Normal, ViewDir), 1.0f);
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//color.a = GetTextureColor(material[v_MID].DiffuseTID).a * material[v_MID].DiffuseColor.a;
}


vec3 CalcDirLight(int mid, DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(lightDir, normal), 0.0);

    float spec = CalcSpec(mid, normal, lightDir, viewDir);

    vec3 ambient  = light.ambient  * ambi * material[mid].DiffuseColor.xyz;
    vec3 diffuse  = light.diffuse  * diff * material[mid].DiffuseColor.xyz;
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
