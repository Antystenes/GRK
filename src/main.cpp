#include <GL/glew.h>
#include <GL/glut.h>
#include "myShader.hpp"
#include <cmath>
#include <string.h>
#include <iostream>
#include <vector>

std::vector<float> kula;

GLuint program, vbo, vao, perspectiveMatrixUnif, modelToCameraMatrixU, lightU;
const float fFrustumScale = 1.0;
float perspectiveMatrix[16];
float light[3];
float modelToCameraMatrix[16];

float angle = 0.0f;

void renderScene()
{
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0, 0.0, 1.0,1.0);
    glBindVertexArray(vao);
    glUseProgram(program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3200);
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
    for (int i = 0; i <= 40; i++)
    {
        float lat0 = 3.14159265359 * (-0.5 + (float)(i - 1) / 40);
        float z0 = sin(lat0);
        float zr0 = cos(lat0);

        float lat1 = 3.14159265359 * (-0.5 + (float)i / 40);
        float z1 = sin(lat1);
        float zr1 = cos(lat1);

        for (int j = 0; j <= 40; j++)
        {
            float lng = 2 * 3.14159265359 * (float)(j - 1) / 40;
            float x = cos(lng);
            float y = sin(lng);

            kula.push_back(x * zr0);
            kula.push_back(y * zr0);
            kula.push_back(z0);
            kula.push_back(1.0f);

            kula.push_back(x * zr1);
            kula.push_back(y * zr1);
            kula.push_back(z1);
            kula.push_back(1.0f);
        }
    }
    light[0] = 10.0f;
    light[1] = 10.0f;
    light[2] = 10.0f;
    light[3] = 0.0f;

    glEnable(GL_DEPTH_TEST);
    //za³aduj i skompiluj shader
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    program = LoadShader("shaders/Vertex_Shader.glsl", "shaders/Fragment_Shader.glsl");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUseProgram(program);
    lightU = glGetUniformLocation(program, "light");
    glUniform4fv(lightU,4,light);

    perspectiveMatrixUnif = glGetUniformLocation(program,"perspectiveMatrix");
    float fzNear = 0.1f;
    float fzFar = 30.0f;
    memset(perspectiveMatrix, 0,sizeof(float) * 16);
    /*float[16]*/
    perspectiveMatrix[0] = fFrustumScale;
    /*const float = 1.0*/
    perspectiveMatrix[5] = fFrustumScale;
    perspectiveMatrix[10] = (fzFar + fzNear) / (fzNear-fzFar);
    perspectiveMatrix[14] = (2 * fzFar * fzNear) / (fzNear-fzFar);
    perspectiveMatrix[11] = -1.0f;
    glUniformMatrix4fv(perspectiveMatrixUnif, 1, GL_FALSE, perspectiveMatrix);

    modelToCameraMatrixU = glGetUniformLocation(program,"modelToCameraMatrix");
    memset(modelToCameraMatrix, 0, sizeof(float) * 16);
    modelToCameraMatrix[0]=modelToCameraMatrix[5]=modelToCameraMatrix[10]=0.25f;
    modelToCameraMatrix[15]=1.0f;
    modelToCameraMatrix[14]=-0.5f;
    glUniformMatrix4fv(modelToCameraMatrixU, 1, GL_FALSE, modelToCameraMatrix);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3200 * 4 * 7,
                 kula.data(),
                 GL_STREAM_DRAW);

    glGenVertexArrays(0, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

//    size_t kolory = sizeof(vertexData) / 2;
    //   glEnableVertexAttribArray(1);
    //   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) kolory);
}

void applyangle(float dif)
{
    angle+=dif;
    float temp[3] = { modelToCameraMatrix[12], modelToCameraMatrix[13], modelToCameraMatrix[14]};
    modelToCameraMatrix[0]=modelToCameraMatrix[10]=cos(angle);
    modelToCameraMatrix[2]=sin(angle);
    modelToCameraMatrix[8]=-sin(angle);
    modelToCameraMatrix[12]=cos(dif)*temp[0] - sin(dif)*temp[2];
    modelToCameraMatrix[14]=sin(dif)*temp[0] + cos(dif)*temp[2];
    glUniformMatrix4fv(modelToCameraMatrixU, 1, GL_FALSE, modelToCameraMatrix);
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 's':
        modelToCameraMatrix[14]-=0.1f;
        glUniformMatrix4fv(modelToCameraMatrixU, 1, GL_FALSE, modelToCameraMatrix);
        break;
    case 'w':
        modelToCameraMatrix[14]+=0.1f;
        glUniformMatrix4fv(modelToCameraMatrixU, 1, GL_FALSE, modelToCameraMatrix);
        break;
    case 'a':
        modelToCameraMatrix[12]+=0.1f;
        glUniformMatrix4fv(modelToCameraMatrixU, 1, GL_FALSE, modelToCameraMatrix);
        break;
    case 'd':
        modelToCameraMatrix[12]-=0.1f;
        glUniformMatrix4fv(modelToCameraMatrixU, 1, GL_FALSE, modelToCameraMatrix);
        break;
    case 'q':
        applyangle(0.1f);
        break;
    case 'e':
        applyangle(-0.1f);
        break;
    }
}

int main(	int	argc, char **argv	)
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
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    glDeleteProgram(program);
    return 0;
}
