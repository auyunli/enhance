#ifndef GLHELPER_H
#define GLHELPER_H

#include <stdio.h>
#include <stdlib.h>

#include "GLIncludes.hpp"

/* #include <GL/glew.h> */

/* #ifdef MAC_OS */
/* #include <GLUT/glut.h> */
/* #else */
/* #include <GL/glut.h> */
/* #endif */
	   
#include <iostream>
using namespace std;

namespace GLSLShader {
    enum GLSLShaderType {
        VERTEX, FRAGMENT, GEOMETRY,TESS_CONTROL, TESS_EVALUATION
    };
};

bool GLCompileShaderFromString( GLuint & shader, char const * Source, GLSLShader::GLSLShaderType type );
bool GLCompileShaderFromFile( GLuint & shader, char const * fileName, GLSLShader::GLSLShaderType type );
bool GLCreateProgram( GLuint & program );
bool GLLinkProgram( GLuint & program );
bool GLLinkUseProgram( GLuint & program );
void GLUseProgram( GLuint & program );
void GLPrintInfo();

void GLBindAttribLocation( GLuint Program, GLuint Loc, char const * Name );
void GLBindFragDataLocation( GLuint Program, GLuint Loc, char const * Name );
bool GLSetUniformVec3( GLuint Program, char const * Name, float * v );
bool GLSetUniformVec4( GLuint Program, char const * Name, float * v );
bool GLSetUniformMat3( GLuint Program, char const * Name, float * m );
bool GLSetUniformMat4( GLuint Program, char const * Name, float * m );
bool GLSetUniform( GLuint Program, char const * Name, float val );
bool GLSetUniform( GLuint Program, char const * Name, int val );
bool GLSetUniform( GLuint Program, char const * Name, bool val );
void GLPrintActiveUniforms( GLuint Program );
void GLPrintActiveAttribs( GLuint Program );

bool GLGenTextures( int Size, GLuint * TexNames );
bool GLSetTexture( int GLTextureUnitOffset, GLuint TexName, unsigned char const * TexData, int Width, int Height );
void GLBindVertexArray( GLuint vbo );
void GLUnBindVertexArray();

void GLUniformSubroutinesuiv( GLenum ShaderType, GLsizei Count, GLuint const * Indices ); //loads the active subroutine uniforms
GLint GLGetSubroutineUniformLocation( GLuint Program, GLenum Shadertype, GLchar const * Name ); //get subroutine uniform location in a program's shader stage
GLuint GLGetSubroutineIndex(GLuint program, GLenum shadertype, const GLchar * name ); //get index of a subroutine uniform in a program's shader stage

#endif
