varying vec3 vv3color;
uniform int colorflag;
void main() {
	if(colorflag==1){
		gl_FragColor=vec4(vv3color.r,0,0,1.0);
	}
	else if(colorflag==2){
		gl_FragColor=vec4(0,vv3color.g,0,1.0);
	}
	else if(colorflag==3){
		gl_FragColor=vec4(0,0,vv3color.b,1.0);
	}
	else {
		gl_FragColor = vec4(vv3color, 1.0);
	}
}
