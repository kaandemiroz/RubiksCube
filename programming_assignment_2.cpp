//
//  programming_assignment_2.cpp
//  COMP410
//
//  Created by Osman Kaan Demiröz on 16/03/16.
//  Copyright © 2016 Osman Kaan Demiröz. All rights reserved.
//

#include "Angel.h"

typedef Angel::vec4	point4;
typedef Angel::vec4 color4;

const int NumPoints = 972 + 330;		// (27 cubes) * (6 faces) * (6 vertices/face) (330 extra indices to work around a color issue in lines)
const int NumLinePoints = 1296 + 200;	// (27 cubes) * (6 faces) * (8 vertices/face) (200 extra indices to work around a color issue while reshaping)
const int NumVertices = 64;				// (4 points/axis) ^ (3 axes)
const int ppc = 36;						// Points Per Cube = (6 faces) * (6 vertices/face)
const int lppc = 48;					// Line Points Per Cube = (6 faces) * (8 vertices/face)
float width, height;
float angle = 1.0;

GLfloat size = 0.38;						// Small cube piece size
GLfloat scaleFactor = 1.0;
GLfloat dt = 10.0;
GLfloat t;

vec3 lastPos = vec3(0.0, 0.0, 1.0);
vec3 theta = vec3(10.0, -10.0, 0.0);
vec3 lastTheta = theta;

mat4 ModelViews[27];
mat4 globalModelView;

point4 points[NumPoints];
color4 colors[NumPoints];
point4 linepoints[NumLinePoints];
point4 vertices[NumVertices] = {
	point4( -1.5 * size,  1.5 * size,  1.5 * size, 1.0 ),
	point4( -0.5 * size,  1.5 * size,  1.5 * size, 1.0 ),
	point4(  0.5 * size,  1.5 * size,  1.5 * size, 1.0 ),
	point4(  1.5 * size,  1.5 * size,  1.5 * size, 1.0 ),
	point4( -1.5 * size,  0.5 * size,  1.5 * size, 1.0 ),
	point4( -0.5 * size,  0.5 * size,  1.5 * size, 1.0 ),
	point4(  0.5 * size,  0.5 * size,  1.5 * size, 1.0 ),
	point4(  1.5 * size,  0.5 * size,  1.5 * size, 1.0 ),
	point4( -1.5 * size, -0.5 * size,  1.5 * size, 1.0 ),
	point4( -0.5 * size, -0.5 * size,  1.5 * size, 1.0 ),
	point4(  0.5 * size, -0.5 * size,  1.5 * size, 1.0 ),
	point4(  1.5 * size, -0.5 * size,  1.5 * size, 1.0 ),
	point4( -1.5 * size, -1.5 * size,  1.5 * size, 1.0 ),
	point4( -0.5 * size, -1.5 * size,  1.5 * size, 1.0 ),
	point4(  0.5 * size, -1.5 * size,  1.5 * size, 1.0 ),
	point4(  1.5 * size, -1.5 * size,  1.5 * size, 1.0 ),
	point4( -1.5 * size,  1.5 * size,  0.5 * size, 1.0 ),
	point4( -0.5 * size,  1.5 * size,  0.5 * size, 1.0 ),
	point4(  0.5 * size,  1.5 * size,  0.5 * size, 1.0 ),
	point4(  1.5 * size,  1.5 * size,  0.5 * size, 1.0 ),
	point4( -1.5 * size,  0.5 * size,  0.5 * size, 1.0 ),
	point4( -0.5 * size,  0.5 * size,  0.5 * size, 1.0 ),
	point4(  0.5 * size,  0.5 * size,  0.5 * size, 1.0 ),
	point4(  1.5 * size,  0.5 * size,  0.5 * size, 1.0 ),
	point4( -1.5 * size, -0.5 * size,  0.5 * size, 1.0 ),
	point4( -0.5 * size, -0.5 * size,  0.5 * size, 1.0 ),
	point4(  0.5 * size, -0.5 * size,  0.5 * size, 1.0 ),
	point4(  1.5 * size, -0.5 * size,  0.5 * size, 1.0 ),
	point4( -1.5 * size, -1.5 * size,  0.5 * size, 1.0 ),
	point4( -0.5 * size, -1.5 * size,  0.5 * size, 1.0 ),
	point4(  0.5 * size, -1.5 * size,  0.5 * size, 1.0 ),
	point4(  1.5 * size, -1.5 * size,  0.5 * size, 1.0 ),
	point4( -1.5 * size,  1.5 * size, -0.5 * size, 1.0 ),
	point4( -0.5 * size,  1.5 * size, -0.5 * size, 1.0 ),
	point4(  0.5 * size,  1.5 * size, -0.5 * size, 1.0 ),
	point4(  1.5 * size,  1.5 * size, -0.5 * size, 1.0 ),
	point4( -1.5 * size,  0.5 * size, -0.5 * size, 1.0 ),
	point4( -0.5 * size,  0.5 * size, -0.5 * size, 1.0 ),
	point4(  0.5 * size,  0.5 * size, -0.5 * size, 1.0 ),
	point4(  1.5 * size,  0.5 * size, -0.5 * size, 1.0 ),
	point4( -1.5 * size, -0.5 * size, -0.5 * size, 1.0 ),
	point4( -0.5 * size, -0.5 * size, -0.5 * size, 1.0 ),
	point4(  0.5 * size, -0.5 * size, -0.5 * size, 1.0 ),
	point4(  1.5 * size, -0.5 * size, -0.5 * size, 1.0 ),
	point4( -1.5 * size, -1.5 * size, -0.5 * size, 1.0 ),
	point4( -0.5 * size, -1.5 * size, -0.5 * size, 1.0 ),
	point4(  0.5 * size, -1.5 * size, -0.5 * size, 1.0 ),
	point4(  1.5 * size, -1.5 * size, -0.5 * size, 1.0 ),
	point4( -1.5 * size,  1.5 * size, -1.5 * size, 1.0 ),
	point4( -0.5 * size,  1.5 * size, -1.5 * size, 1.0 ),
	point4(  0.5 * size,  1.5 * size, -1.5 * size, 1.0 ),
	point4(  1.5 * size,  1.5 * size, -1.5 * size, 1.0 ),
	point4( -1.5 * size,  0.5 * size, -1.5 * size, 1.0 ),
	point4( -0.5 * size,  0.5 * size, -1.5 * size, 1.0 ),
	point4(  0.5 * size,  0.5 * size, -1.5 * size, 1.0 ),
	point4(  1.5 * size,  0.5 * size, -1.5 * size, 1.0 ),
	point4( -1.5 * size, -0.5 * size, -1.5 * size, 1.0 ),
	point4( -0.5 * size, -0.5 * size, -1.5 * size, 1.0 ),
	point4(  0.5 * size, -0.5 * size, -1.5 * size, 1.0 ),
	point4(  1.5 * size, -0.5 * size, -1.5 * size, 1.0 ),
	point4( -1.5 * size, -1.5 * size, -1.5 * size, 1.0 ),
	point4( -0.5 * size, -1.5 * size, -1.5 * size, 1.0 ),
	point4(  0.5 * size, -1.5 * size, -1.5 * size, 1.0 ),
	point4(  1.5 * size, -1.5 * size, -1.5 * size, 1.0 ),
};

enum {xoff = 1, yoff = 4, zoff = 16};

// Model-view and projection matrices uniform location
GLuint GlobalModelView, ModelView, Projection;
GLuint selectedFace = 0;
mat3 faces[6] = {
	mat3(0,1,2,3,4,5,6,7,8),		// Front
	mat3(18,19,20,9,10,11,0,1,2),		// Top
	mat3(2,11,20,5,14,23,8,17,26),		// Left
	mat3(18,9,0,21,12,3,24,15,6),		// Right
	mat3(6,7,8,15,16,17,24,25,26),		// Bottom
	mat3(20,19,18,23,22,21,26,25,24)	// Behind
};

bool selected = false;
bool trackingMouse = false;

void updatePoints();

// Generate a quad for the four points specified using the given color
// Additionally, generate black boundary lines
int Index = 0, LineIndex = 0;
void quad(int a, int b, int c, int d, color4 color){
	colors[Index] = color; points[Index] = vertices[a]; Index++;
	colors[Index] = color; points[Index] = vertices[b]; Index++;
	colors[Index] = color; points[Index] = vertices[c]; Index++;
	colors[Index] = color; points[Index] = vertices[b]; Index++;
	colors[Index] = color; points[Index] = vertices[c]; Index++;
	colors[Index] = color; points[Index] = vertices[d]; Index++;
	linepoints[LineIndex] = vertices[a]; LineIndex++;
	linepoints[LineIndex] = vertices[b]; LineIndex++;
	linepoints[LineIndex] = vertices[a]; LineIndex++;
	linepoints[LineIndex] = vertices[c]; LineIndex++;
	linepoints[LineIndex] = vertices[b]; LineIndex++;
	linepoints[LineIndex] = vertices[d]; LineIndex++;
	linepoints[LineIndex] = vertices[c]; LineIndex++;
	linepoints[LineIndex] = vertices[d]; LineIndex++;
}

// Construct a cube piece by combining 6 faces of different colors on different sides
void cubepiece(int x, int y, int z){
	int index = x * xoff + y * yoff + z * zoff;
	// Front (Red)
	quad(index, index + xoff, index + yoff, index + xoff + yoff, color4(0.8, 0.0, 0.0, 1.0));
	// Top (White)
	quad(index, index + xoff, index + zoff, index + xoff + zoff, color4(0.8, 0.8, 0.8, 1.0));
	// Right (Blue)
	quad(index + xoff, index + xoff + zoff, index + xoff + yoff, index + xoff + yoff + zoff, color4(0.0, 0.0, 0.8, 1.0));
	// Left (Green)
	quad(index, index + zoff, index + yoff, index + zoff + yoff, color4(0.0, 0.8, 0.0, 1.0));
	// Bottom (Yellow)
	quad(index + yoff, index + xoff + yoff, index + yoff + zoff, index + xoff + yoff + zoff, color4(0.8, 0.8, 0.0, 1.0));
	// Behind (Magenta)
	quad(index + zoff, index + xoff + zoff, index + yoff + zoff, index + xoff + yoff + zoff, color4(0.8, 0.0, 0.8, 1.0));
}

// Construct a Rubik's Cube using 27 smaller cube pieces
void rubikscube(){
	glLineWidth(10);
	for(int z=0; z<3; z++){
		for(int y=0; y<3; y++){
			for(int x=0; x<3; x++){
				cubepiece(x, y, z);
			}
		}
	}
}

// Rotate random faces 100 times to randomize the cube's configuration.
void randomizeCube(){
	int oldSelectedFace = selectedFace;
	int oldT = t;
	srand((int)time(NULL));
	
	for(int i=0; i<100; i++){
		selectedFace = rand() % 6 + 1;
		float r = rand();
		if(r < 0.5) t = -90;
		else t = 90;
		updatePoints();
	}
	selectedFace = oldSelectedFace;
	t = oldT;
}

// Returns true if a face contains a specific cube piece
bool contains(mat3 face, int cubeID){
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			if(face[i][j] == cubeID) return true;
		}
	}
	return false;
}

// OpenGL initialization
void init(){
	
	// Load the Rubik's Cube
	rubikscube();
	// Perform random rotations for initialization
	randomizeCube();
	
	// Create a vertex array object
	GLuint vao;
	glGenVertexArraysAPPLE( 1, &vao );
	glBindVertexArrayAPPLE( vao );
	
	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(	 1,				 &buffer );
	glBindBuffer(	 GL_ARRAY_BUFFER, buffer );
	glBufferData(	 GL_ARRAY_BUFFER,	 sizeof(points) + sizeof(colors) + sizeof(linepoints), NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points),  points );
	glBufferSubData( GL_ARRAY_BUFFER,	 sizeof(points),  sizeof(colors),  colors );
	glBufferSubData( GL_ARRAY_BUFFER,	 sizeof(points) + sizeof(colors),  sizeof(linepoints), linepoints );
	
	// Load shaders and use the resulting shader program
	GLuint program = InitShader( "vshader_pa2.glsl", "fshader_pa2.glsl" );
	
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	
	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
	
	// Retrieve transformation uniform variable locations
	GlobalModelView = glGetUniformLocation(program, "GlobalModelView");
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	
	glUseProgram( program );
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClearStencil(0);
	glEnable( GL_DEPTH_TEST );
}

// OpenGL display function
void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 animation[27];
	
	// Generate the global model-view matrix
	// If a face was selected, rotate the face according to the mouse movement
	if(selected){
		t = theta.x - theta.y + theta.z - lastTheta.x + lastTheta.y - lastTheta.z;
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				switch (selectedFace-1) {
					case 0:
						animation[(int)faces[0][i][j]] = RotateZ(t);
						break;
						
					case 1:
						animation[(int)faces[1][i][j]] = RotateY(t);
						break;
						
					case 2:
						animation[(int)faces[2][i][j]] = RotateX(t);
						break;
						
					case 3:
						animation[(int)faces[3][i][j]] = RotateX(-t);
						break;
						
					case 4:
						animation[(int)faces[4][i][j]] = RotateY(-t);
						break;
						
					case 5:
						animation[(int)faces[5][i][j]] = RotateZ(-t);
						break;
						
					default:
						break;
				}
			}
		}
		// Do NOT rotate whole cube if the user has selected a face
		globalModelView = (Scale(scaleFactor, scaleFactor, scaleFactor) *
						  RotateX( lastTheta.x ) *
						  RotateY( lastTheta.y ));
	}else{
		// Rotate whole cube if the user has NOT selected a face
		globalModelView = (Scale(scaleFactor, scaleFactor, scaleFactor) *
						  RotateX( theta.x ) *
						  RotateY( theta.y ));
	}
	glUniformMatrix4fv( GlobalModelView, 1, GL_TRUE, globalModelView );
	
	// Enable Stencil Test for picking
	glEnable( GL_STENCIL_TEST );
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	color4 temp[ppc];
	// Iterate over each face, and assign a stencil index for each face.
	// For each face, render the cubes appropriately.
	// If the user mouseovers a face, highlight that face.
	for(int f=0; f<6; f++){
		glStencilFunc(GL_ALWAYS, f + 1, -1);
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				int cube = faces[f][i][j];
				// Multiply the continuous mouse rotation with the cubes' individual rotated matrices
				glUniformMatrix4fv( ModelView, 1, GL_TRUE, animation[cube] * ModelViews[cube] );
				
				// Highlight if the mouse is over a face
				if(selectedFace > 0 && selectedFace < 7 && contains(faces[selectedFace-1],cube)){
					for(int k=0; k<ppc; k++){
						temp[k] = colors[cube*ppc+k];
						if(selected) colors[cube*ppc+k] = (colors[cube*ppc+k] + color4(0.4, 0.4, 0.4, 0.0));
						else colors[cube*ppc+k] = (colors[cube*ppc+k] + color4(0.2, 0.2, 0.2, 0.0));
					}
					// Render the cubes
					glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
					glDrawArrays(GL_TRIANGLES, cube*ppc, ppc);
					glDrawArrays(GL_LINES, 2 * NumPoints + cube*lppc, lppc);
					
					// Replace the highlighted colors with the original ones after drawing
					for(int k=0; k<ppc; k++){
						colors[cube*ppc+k] = temp[k];
					}
					glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
				// If the mouse isn't above a face, just render cubes normally.
				}else{
					glDrawArrays(GL_TRIANGLES, cube*ppc, ppc);
					glDrawArrays(GL_LINES, 2 * NumPoints + cube*lppc, lppc);
				}
			}
		}
	}
	
	glDisable( GL_STENCIL_TEST );
	glutSwapBuffers();
}

//  IMPORTANT: Most of the following two methods (trackball_ptov and rotate)
//	have been taken from the example codes of the textbook in Chapter 3, "track.cpp".
//	However, they have been modified significantly to fit the assignment.

vec3 trackball_ptov(int x, int y, int width, int height){
	float d, a;
	vec3 v;
	
	// project x,y onto a hemi-sphere centered within width, height
	v.x = (2.0 * x - width) / width;
	v.y = (height - 2.0 * y) / height;
	
	// ensure that we are inside the circle on the z = 0 plane
	d = sqrt(v.x * v.x + v.y * v.y);
	if(d < 1.0) v.z = cos((M_PI/2.0) * d);
	else v.z = 0.0;
	
	a = 1.0 / sqrt(dot(v,v));
	v *= a;
	
	return v;
}

// Rotate the Rubik's Cube as a whole
void rotate(int x, int y){
	vec3 curPos = trackball_ptov(x, y, width, height);
	if(trackingMouse || selected){
		float a = dot(curPos,curPos);
		
		//check if mouse moved
		if (a){
			// slow down rotation if needed by changing speed
			float speed = 1.1;
			angle = speed * (M_PI/2.0) * sqrt(a);
			
			// compute and normalize rotation direction vector
			theta = sin(angle/2.0) * (lastTheta / sin(angle/2.0) + 100 * cross(lastPos, curPos));
		}
	}
	glutPostRedisplay();
}

// According to the face and the direction of the rotation,
// update the indices of the faces array to match the current configuration
void updatePoints(){
	int f = selectedFace - 1;
	if(t > 45){
		t = 90;
		faces[f] = mat3(faces[f][0][2], faces[f][1][2], faces[f][2][2],
						faces[f][0][1], faces[f][1][1], faces[f][2][1],
						faces[f][0][0], faces[f][1][0], faces[f][2][0]);
	}else if(t < -45){
		t = -90;
		faces[f] = mat3(faces[f][2][0], faces[f][1][0], faces[f][0][0],
						faces[f][2][1], faces[f][1][1], faces[f][0][1],
						faces[f][2][2], faces[f][1][2], faces[f][0][2]);
	}else t = 0;
	
	switch (f) {
		case 0:
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					ModelViews[(int)faces[0][i][j]] = RotateZ(t) * ModelViews[(int)faces[0][i][j]];
				}
			}
			
			for(int i=0; i<3; i++) faces[1][2][i] = faces[f][0][i];
			for(int i=0; i<3; i++) faces[2][i][0] = faces[f][i][2];
			for(int i=0; i<3; i++) faces[3][i][2] = faces[f][i][0];
			for(int i=0; i<3; i++) faces[4][0][i] = faces[f][2][i];
			break;
			
		case 1:
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					ModelViews[(int)faces[1][i][j]] = RotateY(t) * ModelViews[(int)faces[1][i][j]];
				}
			}
			
			for(int i=0; i<3; i++) faces[5][0][i] = faces[f][0][2-i];
			for(int i=0; i<3; i++) faces[2][0][i] = faces[f][2-i][2];
			for(int i=0; i<3; i++) faces[3][0][i] = faces[f][i][0];
			for(int i=0; i<3; i++) faces[0][0][i] = faces[f][2][i];
			break;
			
		case 2:
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					ModelViews[(int)faces[2][i][j]] = RotateX(t) * ModelViews[(int)faces[2][i][j]];
				}
			}
			
			for(int i=0; i<3; i++) faces[1][i][2] = faces[f][0][2-i];
			for(int i=0; i<3; i++) faces[5][i][0] = faces[f][i][2];
			for(int i=0; i<3; i++) faces[0][i][2] = faces[f][i][0];
			for(int i=0; i<3; i++) faces[4][i][2] = faces[f][2][i];
			break;
			
		case 3:
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					ModelViews[(int)faces[3][i][j]] = RotateX(-t) * ModelViews[(int)faces[3][i][j]];
				}
			}
			
			for(int i=0; i<3; i++) faces[1][i][0] = faces[f][0][i];
			for(int i=0; i<3; i++) faces[0][i][0] = faces[f][i][2];
			for(int i=0; i<3; i++) faces[5][i][2] = faces[f][i][0];
			for(int i=0; i<3; i++) faces[4][i][0] = faces[f][2][2-i];
			break;
			
		case 4:
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					ModelViews[(int)faces[4][i][j]] = RotateY(-t) * ModelViews[(int)faces[4][i][j]];
				}
			}
			
			for(int i=0; i<3; i++) faces[0][2][i] = faces[f][0][i];
			for(int i=0; i<3; i++) faces[2][2][i] = faces[f][i][2];
			for(int i=0; i<3; i++) faces[3][2][i] = faces[f][2-i][0];
			for(int i=0; i<3; i++) faces[5][2][i] = faces[f][2][2-i];
			break;
			
		case 5:
			for(int i=0; i<3; i++){
				for(int j=0; j<3; j++){
					ModelViews[(int)faces[5][i][j]] = RotateZ(-t) * ModelViews[(int)faces[5][i][j]];
				}
			}
			
			for(int i=0; i<3; i++) faces[1][0][i] = faces[f][0][2-i];
			for(int i=0; i<3; i++) faces[3][i][0] = faces[f][i][2];
			for(int i=0; i<3; i++) faces[2][i][2] = faces[f][i][0];
			for(int i=0; i<3; i++) faces[4][2][i] = faces[f][2][2-i];
			break;
			
		default:
			break;
	}
	glutPostRedisplay();
}

// Mouse click functions
void mouse(int btn, int state, int x, int y){
	// Modify cube rotation while holding right mouse button
	if(btn == GLUT_RIGHT_BUTTON) switch(state){
		case GLUT_DOWN:
			selected = false;
			trackingMouse = true;
			lastPos = trackball_ptov(x, y, width, height);
			break;
		case GLUT_UP:
			trackingMouse = false;
			lastTheta = theta;
			break;
	// Rotate a face while holding left mouse button
	}else if(btn == GLUT_LEFT_BUTTON) switch(state){
		case GLUT_DOWN:
			lastPos = trackball_ptov(x, y, width, height);
			if(selectedFace > 0 && selectedFace < 7) selected = true;
			break;
		case GLUT_UP:
			selected = false;
			updatePoints();
			theta = lastTheta;
			break;
	}
	glutPostRedisplay();
}

// Passively checks whether the user hovers over a new face and
// posts a redisplay if that is the case, in order to highlight the face
void pick(int x, int y){
	GLuint lastFace = selectedFace;
	glReadPixels(x, height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &selectedFace);
	if(lastFace != selectedFace){
//		printf("Face: %d\n",selectedFace);
		glutPostRedisplay();
	}
}

// Adjust rotation parameters on directional button presses
void special(int key, int x, int y){
	switch (key) {
		case GLUT_KEY_UP:
			theta.x -= dt;
			if (theta.x < 0.0){
				theta.x += 360.0;
			}
			break;
			
		case GLUT_KEY_DOWN:
			theta.x += dt;
			if (theta.x > 360.0){
				theta.x -= 360.0;
			}
			break;
			
		case GLUT_KEY_LEFT:
			theta.y -= dt;
			if (theta.y < 0.0){
				theta.y += 360.0;
			}
			break;
			
		case GLUT_KEY_RIGHT:
			theta.y += dt;
			if (theta.y > 360.0){
				theta.y -= 360.0;
			}
			break;
			
		default:
			break;
	}
	lastTheta = theta;
	glutPostRedisplay();
}

// Keyboard keypress functions
void keyboard(unsigned char key, int x, int y){
	switch (key) {
			// Reset the rotation
		case 'i':
			theta = lastTheta = vec3(10.0, -10.0, 0.0);
			break;
			
			// "Zoom-in" to the object
		case 'z':
			scaleFactor *= 1.1;
			break;
			
			// "Zoom-out" from the object
		case 'Z':
			scaleFactor *= 0.9;
			break;
			
			// Randomize the faces of the cube
		case 'r':
			randomizeCube();
			break;
			
			// Print help instructions
		case 'h':
			printf("HELP: \n Right Click -- hold and drag to rotate the whole cube \n Left Click -- select a face and drag to rotate the particular face of the cube \n Up and Down arrows -- rotate the cube incrementally around the X-axis \n Left and Right arrows -- rotate the cube incrementally around the Y-axis \n i -- reset the cube rotation \n r -- randomize the faces of the cube \n z -- zoom-in, Z -- zoom-out \n q -- quit (exit) the program\n");
			break;
			
			// Exit the application
		case 'q':
			exit(EXIT_SUCCESS);
			break;
	}
	glutPostRedisplay();
}

// Reshape the application window, preserving the object aspect ratio
void reshape(int w, int h){
	glViewport( 0, 0, w, h );
	width = w;
	height = h;
	GLfloat aspect = GLfloat(w) / h;
	
	mat4  projection;
	if (w <= h)
		projection = Ortho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect, -1.0, 1.0);
	else
		projection = Ortho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);
	
	glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
}

int main(int argc, char **argv){
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(320, 180);
	glutCreateWindow("Programing Assignment 2");
	
	init();
	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutMotionFunc(rotate);
	glutPassiveMotionFunc(pick);
	
	glutMainLoop();
	return 0;
	
}
