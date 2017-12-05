#include <stdlib.h>
#include <GL/glut.h>

GLsizei WIDTH = 600, HEIGHT = 600;
GLfloat lightPosition[] = {-25.f, 0.f, 50.f, 1.f};

void clearBuffers() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void draw() {
    clearBuffers();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3d(0, 1, 0);

    glPushMatrix();
        glBegin(GL_QUADS);
            glVertex3f(0.0,0.0,0.0);
            glVertex3f(5.0,0.0,0.0);
            glVertex3f(5.0,5.0,0.0);
            glVertex3f(0.0,5.0,0.0);
        glEnd();
    glPopMatrix();


    glutSwapBuffers();
}

void mouseClick(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {

    }
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
        exit(1);
    }
}

void mouseMove(int x, int y) {

}

static void keyboardKeys(unsigned char key, int x, int y) {

}

void defineWindowConfiguration() {
    glutInitWindowSize(600, 600);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Sketch WOGL");
    glClearColor(1, 1, 1, 0);
}

void definePerspective() {
    glMatrixMode(GL_PROJECTION);
    glOrtho(-15, 15, -15, 15, -15, 15);
    glMatrixMode(GL_MODELVIEW);
}

void defineCallbacks() {
    glutDisplayFunc(draw);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboardKeys);
    //glutSpecialFunc(keyboardSpecialKeys);
}

void defineLightConfiguration() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_COLOR_MATERIAL);
}

void init() {
    defineWindowConfiguration();
    definePerspective();
    defineCallbacks();
    defineLightConfiguration();
    glutMainLoop();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    init();
    return 0;
}