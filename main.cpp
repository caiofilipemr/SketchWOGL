#include <stdlib.h>
#include <GL/glut.h>
#include <vector>

#define ORTHO_SIZE 30

struct Position {
    float x, y, z;

    Position() : x(0), y(0), z(0) {}

    Position(float x, float y, float z) : x(x), y(y), z(z) { }
};

struct Polygon {
	Position p1, p2, p3, p4;
	
	Polygon(Position p1, Position p2) {
		if (p1.x == p2.x) {
			startPlanX(p1, p2);
		} else {
			startPlanZ(p1, p2);
		}
	}
	
	void startPlanX(Position p1, Position p2) {
		this->p1 = p1;
		this->p2 = Position(p1.x, p1.y, p2.z);
		this->p3 = p2;
		this->p4 = Position(p2.x, p2.y, p1.z);
	}
	
	void startPlanZ(Position p1, Position p2) {
		this->p1 = p1;
		this->p2 = Position(p2.x, p1.y, p1.z);
		this->p3 = p2;
		this->p4 = Position(p1.x, p2.y, p2.z);
	}
};

const float w1 = 10, w2 = 12, h = 5;
const float xInit = 0 - w1 / 2, yInit = 0, zInit = 0 + w2 / 2;

const float espacoFinalFrente = 0.05, alturaEspacoFinalJanelaFrente = 0.2, alturaEspacoFinalSacadaFrente = 0.1;
const float espacoJanelasFrente = 0.1, alturaEspacoJanelaFrente = 0.4;
const float janelaFrente = 0.21, alturaJanelaFrente = 0.4;
const float sacadaFrente = 0.28, alturaSacadaFrente = 0.8;

GLsizei WIDTH = 1000, HEIGHT = 1000;
GLfloat lightPosition[] = {0.f, 0.f, -50.f, 1.f};

GLfloat xRotation = 0;
GLfloat yRotation = 0;
GLfloat zRotation = 0;
GLfloat zoom = 0;

Polygon terreno = Polygon(Position(-15, 0, -15), Position(15, 0, 15));
std::vector<Polygon> paredesFrente, paredesFundo, paredesEsquerda, paredesDireita;

void clearBuffers() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void setViewAngle() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, zoom);
    glRotatef(xRotation, 1, 0, 0);
    glRotatef(yRotation, 0, 1, 0);
    glRotatef(zRotation, 0, 0, 1);
}

void drawPolygon(Polygon p, Position color) {
    glPushMatrix();
    glColor3d(color.x, color.y, color.z);
    glBegin(GL_POLYGON);
    glVertex3f(p.p1.x, p.p1.y, p.p1.z);
    glVertex3f(p.p2.x, p.p2.y, p.p2.z);
    glVertex3f(p.p3.x, p.p3.y, p.p3.z);
    glVertex3f(p.p4.x, p.p4.y, p.p4.z);
    glEnd();
    glPopMatrix();
}

void drawTerreno(Position color) {
    drawPolygon(terreno, color);
}

void draw() {
    clearBuffers();

    setViewAngle();

    Position color = Position(0, 1, 0);
    drawTerreno(color);

    color = Position(1, 0, 0);
    for (auto p : paredesFrente) {
        drawPolygon(p, color);
    }

    color = Position(0, 0, 1);
    for (auto p : paredesFundo) {
        drawPolygon(p, color);
    }

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
    switch (key) {
        case ',':
            zRotation += 1;
            break;
        case '.':
            zRotation -= 1;
            break;
        case '+':
            zoom += 1;
            break;
        case '-':
            zoom -= 1;
            break;
    }
    glutPostRedisplay();
}

void keyboardSpecialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_LEFT:
            yRotation -= 1;
            break;
        case GLUT_KEY_RIGHT:
            yRotation += 1;
            break;
        case GLUT_KEY_UP:
            xRotation -= 1;
            break;
        case GLUT_KEY_DOWN:
            xRotation += 1;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void defineWindowConfiguration() {
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Sketch WOGL");
    glClearColor(1, 1, 1, 0);
}

void definePerspective() {
    glMatrixMode(GL_PROJECTION);
    glOrtho(-ORTHO_SIZE, ORTHO_SIZE, -ORTHO_SIZE, ORTHO_SIZE, -ORTHO_SIZE, ORTHO_SIZE);
    glMatrixMode(GL_MODELVIEW);
}

void defineCallbacks() {
    glutDisplayFunc(draw);
    glutMouseFunc(mouseClick);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboardKeys);
    glutSpecialFunc(keyboardSpecialKeys);
}

void defineLightConfiguration() {
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//    glEnable(GL_COLOR_MATERIAL);
}

void createParedeFrenteFundoPolygons(float z, std::vector<Polygon> &parede) {
    float x[8];
    x[0] = xInit; x[1] = x[0] + w1 * espacoFinalFrente; x[2] = x[1] + w1 * janelaFrente; x[3] = x[2] + w1 * espacoJanelasFrente;
    x[4] = x[3] + w1 * sacadaFrente; x[5] = x[4] + w1 * espacoJanelasFrente; x[6] = x[5] + w1 * janelaFrente; x[7] = x[6] + w1 * espacoFinalFrente;

    float yJan[4];
    yJan[0] = yInit; yJan[1] = yJan[0] + h * alturaEspacoJanelaFrente; yJan[2] = yJan[1] + h * alturaJanelaFrente; yJan[3] = yJan[2] + h * alturaEspacoFinalJanelaFrente;

    float ySac[4];
    ySac[0] = yInit; ySac[1] = ySac[0] + h * alturaEspacoFinalSacadaFrente; ySac[2] = ySac[1] + h * alturaSacadaFrente; ySac[3] = ySac[2] + h * alturaEspacoFinalSacadaFrente;

    parede.push_back(Polygon(Position(x[0], yInit, z), Position(x[1], yInit + h, z)));
    parede.push_back(Polygon(Position(x[1], yJan[0], z), Position(x[2], yJan[1], z)));
    parede.push_back(Polygon(Position(x[1], yJan[2], z), Position(x[2], yJan[3], z)));
    parede.push_back(Polygon(Position(x[2], yInit, z), Position(x[3], yInit + h, z)));
    parede.push_back(Polygon(Position(x[3], ySac[0], z), Position(x[4], ySac[1], z)));
    parede.push_back(Polygon(Position(x[3], ySac[2], z), Position(x[4], ySac[3], z)));
    parede.push_back(Polygon(Position(x[4], yInit, z), Position(x[5], yInit + h, z)));
    parede.push_back(Polygon(Position(x[5], yJan[0], z), Position(x[6], yJan[1], z)));
    parede.push_back(Polygon(Position(x[5], yJan[2], z), Position(x[6], yJan[3], z)));
    parede.push_back(Polygon(Position(x[6], yInit, z), Position(x[7], yInit + h, z)));
}

void createAndarPolygons() {
    createParedeFrenteFundoPolygons(zInit, paredesFrente);
    createParedeFrenteFundoPolygons(zInit - w2, paredesFundo);
}

void createPolygons() {
    createAndarPolygons();

}

void init() {
    defineWindowConfiguration();
    definePerspective();
    defineCallbacks();
    defineLightConfiguration();
    createPolygons();
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    init();
    return 0;
}
