#include <GL/glew.h>
#include <GL/glut.h>
#include "Shader_Loader.h"
#include <string.h>
#include <iostream>

const float vertexData[] = {
    0.25f, 0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,

    0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,

    0.25f, 0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,

    -0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,

    -0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,

    0.25f, 0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,

    0.25f, 0.25f, -1.25f, 1.0f,
    0.25f, 0.25f, -2.75f, 1.0f,
    0.25f, -0.25f, -2.75f, 1.0f,

    0.25f, 0.25f, -2.75f, 1.0f,
    0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,

    0.25f, 0.25f, -2.75f, 1.0f,
    -0.25f, 0.25f, -1.25f, 1.0f,
    -0.25f, 0.25f, -2.75f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,
    0.25f, -0.25f, -1.25f, 1.0f,

    0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -2.75f, 1.0f,
    -0.25f, -0.25f, -1.25f, 1.0f,




    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,

};
using namespace Core;
GLuint program, vbo, vao, czasU, perspectiveMatrixUnif;
const float fFrustumScale = 1.0;
float perspectiveMatrix[16];

float modelToCameraMatrix[16];

void renderScene()
{
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 0.0, 1.0,1.0);
    glBindVertexArray(vao);
    glUseProgram(program);
    glUniform1f(czasU, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    perspectiveMatrix[0] = fFrustumScale * (h / (float)w);
    perspectiveMatrix[5] = fFrustumScale;
    glUseProgram(program);
    glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, perspectiveMatrix);
    glUseProgram(0);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void Init()
{
    glEnable(GL_DEPTH_TEST);
    //za³aduj i skompiluj shader
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    Core::Shader_Loader shaderLoader;
    program = shaderLoader.CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    czasU = glGetUniformLocation(program,"czas");
    GLuint czasokresU = glGetUniformLocation(program, "czasokres");
    GLuint fragCzasokresU = glGetUniformLocation(program, "fragCzasokres");
    glUseProgram(program);
    glUniform1f(czasokresU, 5.0f);
    glUniform1f(fragCzasokresU, 10.0f);

    perspectiveMatrixUnif = glGetUniformLocation(program,"perspectiveMatrix");
    float fzNear = 0.5f;
    float fzFar = 3.0f;
    memset(perspectiveMatrix, 0,sizeof(float) * 16);
    /*float[16]*/
    perspectiveMatrix[0] = fFrustumScale;
    /*const float = 1.0*/
    perspectiveMatrix[5] = fFrustumScale;
    perspectiveMatrix[10] = (fzFar + fzNear) / (fzNear-fzFar);
    perspectiveMatrix[14] = (2 * fzFar * fzNear) / (fzNear-fzFar);
    perspectiveMatrix[11] = -1.0f;
    glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, perspectiveMatrix);



    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertexData),
                 vertexData,
                 GL_STREAM_DRAW);

    glGenVertexArrays(0, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    size_t kolory = sizeof(vertexData) / 2;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) kolory);
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(500, 500);
    glutInitWindowSize(800, 600);
    if (glewIsSupported("GL_VERSION_4_3"))
        glutCreateWindow("OpenGL4.3 Pierwsze Okno");
    else
        glutCreateWindow("OpenGLPierwsze Okno");
    glewInit();

    Init();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutMainLoop();
    glDeleteProgram(program);
    return 0;
}
