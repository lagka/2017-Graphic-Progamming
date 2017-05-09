attribute vec4 av4position;
attribute vec3 av3normal;


varying vec4 vv4color;

uniform mat4 mvp;
uniform mat4 MM;
struct LightSourceParameters {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec4 halfVector;
	vec4 spotDirection;
	float spotExponent;
	float spotCutoff; // (range: [0.0,90.0], 180.0)
	float spotCosCutoff; // (range: [1.0,0.0],-1.0)
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

struct MaterialParameters {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform MaterialParameters Material;
uniform LightSourceParameters LightSource[4];
uniform mat4 Normal;
uniform vec3 EyePoint;
uniform int flag;
uniform int mod[3];
uniform int dl;
uniform int poi;
uniform int spoteffect;
uniform int shad;
void main() {
	
	vec4 vv4ambient_D = Material.ambient * LightSource[1].ambient;
	
	vec4 vertex_position=Normal*av4position;
	vec4 L=normalize(LightSource[1].position-vertex_position);
	vec3 N=normalize((transpose(inverse(MM))*vec4(av3normal,0))).xyz;
	vec4 vv4diffuse_D=Material.diffuse*LightSource[1].diffuse*max(dot(N,normalize(L.xyz)),0);
	vec4 V=normalize(vec4(EyePoint,1)-vertex_position);
	vec4 H=normalize(L+V);
	vec3 R=normalize(reflect(-L,N));
	vec4 vv4specular_D;
	if(shad)vv4specular_D=Material.specular*LightSource[1].specular*pow(max(dot(R,V.xyz),0),Material.shininess);
	else if(shad==0)vv4specular_D=Material.specular*LightSource[1].specular*pow(max(dot(N,H.xyz),0),Material.shininess);
	
	vec4 L2=normalize(LightSource[2].position-vertex_position);
	vec3 N2=normalize((transpose(inverse(MM))*vec4(av3normal,0))).xyz;
	vec4 vv4diffuse_P=Material.diffuse*LightSource[2].diffuse*max(dot(N2,normalize(L2.xyz)),0);
	vec4 V2=normalize(vec4(EyePoint,1)-vertex_position);
	vec4 H2=normalize(L2+V2);
	vec3 R2=normalize(reflect(-L2,N2));
	vec4 vv4specular_P;
	if(shad)vv4specular_P=Material.specular*LightSource[2].specular*pow(max(dot(R2,V2.xyz),0),Material.shininess);
	else if(shad==0)vv4specular_P=Material.specular*LightSource[2].specular*pow(max(dot(N2,H2.xyz),0),Material.shininess);
	float fatt=min(1/(LightSource[2].constantAttenuation+LightSource[2].linearAttenuation*length(L2)+LightSource[2].quadraticAttenuation*pow(length(L2),2)),1);
	
	if(dl==3){	
		vv4diffuse_D=vec4(0,0,0,0);
		vv4specular_D=vec4(0,0,0,0);
	}
	if(poi==1){
		vv4specular_P=vec4(0,0,0,0);
		vv4diffuse_P=vec4(0,0,0,0);
	}
	float se,sse;
	
	vec4 L3=normalize(LightSource[3].position-vertex_position);
	vec3 N3=normalize((transpose(inverse(MM))*vec4(av3normal,0))).xyz;
	vec4 vv4diffuse_S=Material.diffuse*LightSource[3].diffuse*max(dot(N3,normalize(L3.xyz)),0);
	vec4 V3=normalize(vec4(EyePoint,1)-vertex_position);
	vec4 H3=normalize(L3+V3);
	
	if(spoteffect==1){se=1;sse=0;}
	if(dot(L3,normalize(-1*LightSource[3].spotDirection))>=LightSource[3].spotCosCutoff&&spoteffect==0){
		sse=pow(max(dot(L3,normalize(-1*LightSource[3].spotDirection)),0),LightSource[3].spotExponent);
	}
	else if(dot(L3,normalize(-1*LightSource[3].spotDirection))<LightSource[3].spotCosCutoff&&spoteffect==0){
		sse=0;	
	}
	vec3 R3=normalize(reflect(-L3,N3));
	vec4 vv4specular_S;
	if(shad) vv4specular_S=Material.specular*LightSource[3].specular*pow(max(dot(R3,V3.xyz),0),Material.shininess);
	else if(shad==0)vv4specular_S=Material.specular*LightSource[3].specular*pow(max(dot(N3,H3.xyz),0),Material.shininess);
	float fatt2=min(1/(LightSource[2].constantAttenuation+LightSource[2].linearAttenuation*length(L3)+LightSource[2].quadraticAttenuation*pow(length(L3),2)),1);
	if(mod[0]==1)vv4ambient_D=vec4(0,0,0,0);
	if(mod[1]==1)vv4diffuse_D=vec4(0,0,0,0);
	if(mod[2]==1)vv4specular_D=vec4(0,0,0,0);
	if(mod[1]==1)vv4diffuse_P=vec4(0,0,0,0);
	if(mod[2]==1)vv4specular_P=vec4(0,0,0,0);
	if(mod[1]==1)vv4diffuse_S=vec4(0,0,0,0);
	if(mod[2]==1)vv4specular_S=vec4(0,0,0,0);
	vv4color = se*vv4ambient_D+vv4diffuse_D+vv4specular_D+fatt*(vv4specular_P+vv4diffuse_P)+sse*fatt2*(vv4specular_S+vv4diffuse_S);
	gl_Position = mvp * av4position;
}