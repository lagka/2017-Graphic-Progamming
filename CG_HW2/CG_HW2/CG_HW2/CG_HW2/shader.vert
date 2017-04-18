attribute vec4 av4position;
attribute vec3 av3color;

varying vec3 vv3color;

uniform mat4 mvp;

void main() {
	// NOTE!! column major

	vv3color = av3color;
	gl_Position = mvp * av4position;
}
