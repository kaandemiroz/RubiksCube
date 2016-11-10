attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 GlobalModelView;
uniform mat4 ModelView;
uniform mat4 Projection;

void main(){
	
	color = vColor;
	gl_Position = Projection * GlobalModelView * ModelView * vPosition;
	
}