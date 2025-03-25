#include "Cube.h"
#include <stringHelp.h>


using namespace std;
using namespace glm;


// Example data for cube model

// Packed vertex buffer for cube
static float positionArray[] = {

	//top
	-1.0f, 1.0f, 1.0f, 1.0f,    //top front left,0
	-1.0f, 1.0f, -1.0f, 1.0f,   //top back left,1
	1.0f, 1.0f, -1.0f, 1.0f,    //top back right,2
	1.0f, 1.0f, 1.0f, 1.0f,     //top front right,3

	//bottom
	-1.0f, -1.0f, 1.0f, 1.0f,    //bottom front left,4
	-1.0f, -1.0f, -1.0f, 1.0f,   //bottom back left,5
	1.0f, -1.0f, -1.0f, 1.0f,    //bottom back right,6
	1.0f, -1.0f, 1.0f, 1.0f,     //bottom front right,7

	//right
	1.0f, 1.0f, -1.0f, 1.0f,    //top front right,8
	1.0f, 1.0f, 1.0f, 1.0f,     //top back right,9
	1.0f, -1.0f, -1.0f, 1.0f,   //bottom front right,10
	1.0f, -1.0f, 1.0f, 1.0f,    //bottom back right,11

	//left
	-1.0f, 1.0f, 1.0f, 1.0f,    //top front left,12
	-1.0f, 1.0f, -1.0f, 1.0f,   //top back left,13
	-1.0f, -1.0f, 1.0f, 1.0f,   //bottom front left,14
	-1.0f, -1.0f, -1.0f, 1.0f,  //bottom back left,15

	//front
	-1.0f, 1.0f, 1.0f, 1.0f,    //top front left,16
	1.0f, 1.0f, 1.0f, 1.0f,		//top front right,17
	-1.0f, -1.0f, 1.0f, 1.0f,   //bottom front left,18
	1.0f, -1.0f, 1.0f, 1.0f,    //bottom front right,19

	//back
	-1.0f, 1.0f, -1.0f, 1.0f,    //top back left,20
	1.0f, 1.0f, -1.0f, 1.0f,     //top back right,21
	-1.0f, -1.0f, -1.0f, 1.0f,   //bottom back left,22
	1.0f, -1.0f, -1.0f, 1.0f,    //bottom back right,23


};

// Packed colour buffer for principle axes model
static float colourArray[] = {
	//top
	0.3f, 0.3f, 0.3f, 1.0f,//top front left,0
	0.3f, 0.3f, 0.3f, 1.0f,//top back left,1
	0.3f, 0.3f, 0.3f, 1.0f,//top back right,2
	0.3f, 0.3f, 0.3f, 1.0f,//top front right,3
	
	//bottom
	0.0f, 0.0f, 1.0f, 1.0f,//bottom front left,4
	0.0f, 0.0f, 1.0f, 1.0f,//bottom back left,5
	0.0f, 0.0f, 1.0f, 1.0f,//bottom back right,6
	0.0f, 0.0f, 1.0f, 1.0f, //bottom front right,7
	
	//right
	1.0f, 1.0f, 0.0f, 1.0f,//top back right,8
	1.0f, 1.0f, 0.0f, 1.0f,//top front right,9
	1.0f, 1.0f, 0.0f, 1.0f,//bottom back right,10
	1.0f, 1.0f, 0.0f, 1.0f, //bottom front right,11
	
	//left
	0.0f, 1.0f, 1.0f, 1.0f,//top front left,12
	0.0f, 1.0f, 1.0f, 1.0f,//top back left,13
	0.0f, 1.0f, 1.0f, 1.0f,//bottom front left,14
	0.0f, 1.0f, 1.0f, 1.0f,//bottom back left,15
	
	//front
	0.0f, 0.5f, 0.0f, 1.0f,//top front left,16
	0.0f, 0.5f, 0.0f, 1.0f,//top front right,17
	0.0f, 0.5f, 0.0f, 1.0f,//bottom front left,18
	0.0f, 0.5f, 0.0f, 1.0f, //bottom front right,19
	
	//back
	1.0f, 0.0f, 0.0f, 1.0f,//top back left,20
	1.0f, 0.0f, 0.0f, 1.0f,//top back right,21
	1.0f, 0.0f, 0.0f, 1.0f,//bottom back left,22
	1.0f, 0.0f, 0.0f, 1.0f//bottom back right,23
};


// Line list topology to render principle axes
static unsigned int indexArray[] = {

	// Top face
	2, 1, 0,  
	3, 2, 0,

	// Bottom face
	5, 6, 4, 
	6, 7, 4,

	// Right face
	8, 9, 11, 
	11, 10, 8,

	// Left face
	13, 14, 12,
	14, 13, 15,

	// Front face
	16,19,17,
	19,16,18,
	
	// Back face
	20, 21, 22,
	21, 23, 22
};



Cube::Cube() {

	m_type = "CUBE";

	m_numFaces = 6 * 2;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// setup vbo for position attribute
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionArray), positionArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	// setup vbo for colour attribute
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourArray), colourArray, GL_STATIC_DRAW);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(4);

	// setup vbo for cube) index buffer
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


Cube::~Cube() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_colourBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}


void Cube::Render() {
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_numFaces * 3 , GL_UNSIGNED_INT, (const GLvoid*)0);
}

void Cube::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
}

