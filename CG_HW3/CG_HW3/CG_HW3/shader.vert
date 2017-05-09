attribute vec4 av4position;
attribute vec3 av3color;
attribute vec3 av3normal;

varying vec3 vv3color;
varying vec4 vertex_position;
uniform mat4 mvp;
uniform mat4 MM;
uniform mat4 Normal;
varying vec3 n;
void main() {
	
	vv3color = av3color;
	vertex_position=Normal*av4position;
	n=normalize(transpose(inverse(MM))*vec4(av3normal,0)).xyz;
	gl_Position = mvp * av4position;
}
