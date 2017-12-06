#include <stdlib.h>
#include <GL/glut.h>
#include <vector>

#define ORTHO_SIZE 70
#define N_ANDARES 10

struct Position {
    float x, y, z, a;

    Position() : x(0), y(0), z(0), a(1) {}

    Position(float x, float y, float z) : x(x), y(y), z(z), a(1) { }
    Position(float x, float y, float z, float a) : x(x), y(y), z(z), a(a) { }
};

struct Polygon {
	Position p1, p2, p3, p4;

    Polygon() { }

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

const float w1 = 10, w2 = 18, h = 5;
const float xInit = 0 - w1 / 2, yInit = 0, zInit = 0 + w2 / 2;

const float espacoFinalFrente = 0.05, alturaEspacoFinalJanelaFrente = 0.2, alturaEspacoFinalSacadaFrente = 0.1;
const float espacoJanelasFrente = 0.1, alturaEspacoJanelaFrente = 0.4;
const float janelaFrente = 0.21, alturaJanelaFrente = 0.4;
const float sacadaFrente = 0.28, alturaSacadaFrente = 0.8;

const float espacoFinalLado = 0.16, espacoJanelasLado = 0.13, espacoJanelasBanheiroLado = 0.06;
const float alturaEspacoJanelaLado = 0.4, alturaEspacoFinalJanelaLado = 0.2, alturaEspacoJanelaBanheiroLado = 0.7, alturaEspacoFinalJanelaBanheiroLado = 0.1;
const float janelaLado = 0.14, alturaJanelaLado = 0.4;
const float janelaBanheiroLado = 0.04, alturaJanelaBanheiroLado = 0.2;

Position vidroColor = Position(0.7, 0.7, 0.7, 0.5);

GLsizei WIDTH = 1000, HEIGHT = 1000;

GLfloat xRotation = 15;
GLfloat yRotation = 15;
GLfloat zRotation = 0;
GLfloat zoom = 0;

const float terrenoSize = 100;
Polygon terreno = Polygon(Position(-terrenoSize, 0, -terrenoSize), Position(terrenoSize, 0, terrenoSize));
Polygon piso;
std::vector<Polygon> paredesFrente, paredesFundo, paredesEsquerda, paredesDireita, heliporto;
std::vector<Polygon> janelasFrente, janelasFundo, janelasEsquerda, janelasDireita;
std::vector<Polygon> paredesFrenteTerreo, paredesFundoTerreo, paredesEsquerdaTerreo, paredesDireitaTerreo;

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

void drawPolygon(Polygon p, Position color, float yFactor = 1) {
    glPushMatrix();
    glColor4d(color.x, color.y, color.z, color.a);
    glBegin(GL_POLYGON);
    glVertex3f(p.p1.x, p.p1.y + h * yFactor, p.p1.z);
    glVertex3f(p.p2.x, p.p2.y + h * yFactor, p.p2.z);
    glVertex3f(p.p3.x, p.p3.y + h * yFactor, p.p3.z);
    glVertex3f(p.p4.x, p.p4.y + h * yFactor, p.p4.z);
    glEnd();
    glPopMatrix();
}

void drawTerreno(Position color) {
    drawPolygon(terreno, color);
}

void drawManyPolygons(std::vector<Polygon> &polygons, Position &color, int yFactor) {
    for (auto p : polygons) {
        drawPolygon(p, color, yFactor);
    }
}

void drawManyPolygonsWithTransparency(std::vector<Polygon> polygons, Position &color, int &yFactor) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    drawManyPolygons(polygons, color, yFactor);
    glDisable(GL_BLEND);
}

void drawParedeFrente(Position &color, int &yFactor) {
    drawManyPolygons(paredesFrente, color, yFactor);
}

void drawParedeFundo(Position &color, int &yFactor) {
    drawManyPolygons(paredesFundo, color, yFactor);
}

void drawParedeEsquerda(Position &color, int &yFactor) {
    drawManyPolygons(paredesEsquerda, color, yFactor);
}

void drawParedeDireita(Position &color, int &yFactor) {
    drawManyPolygons(paredesDireita, color, yFactor);
}

void drawPiso(Position color, int yFactor) {
    drawPolygon(piso, color, yFactor);
}

void drawHeliporto(int yFactor) {
    for (auto p : heliporto) {
        drawPolygon(p, Position(0.8, 0.8, 0, 1), yFactor);
    }
}

void drawTransparentes() {
    for (int i = 2; i <= N_ANDARES + 1; ++i) {
        drawManyPolygonsWithTransparency(janelasFrente, vidroColor, i);
        drawManyPolygonsWithTransparency(janelasFundo, vidroColor, i);
        drawManyPolygonsWithTransparency(janelasEsquerda, vidroColor, i);
        drawManyPolygonsWithTransparency(janelasDireita, vidroColor, i);
    }
}

void drawTerreo() {
    Position color = Position(0.9, 0.8, 0.8, 1);

    for (auto p : paredesFrenteTerreo) {
        drawPolygon(p, color);
    }
    for (auto p : paredesFundoTerreo) {
        drawPolygon(p, color);
    }
    for (auto p : paredesEsquerdaTerreo) {
        drawPolygon(p, color);
    }
    for (auto p : paredesDireitaTerreo) {
        drawPolygon(p, color);
    }

    bool b = true;
    Position c1 = Position(0.2, 0.2, 0.2), c2 = Position(0.8, 0.8, 0.8);
    for (int i = 0; i < w1; i = i + 2) {
        for (int j = 0; j < w2; j = j + 2) {
            drawPolygon(Polygon(Position(xInit + i, yInit + 0.001f, zInit - j),
                                Position(xInit + i + 2, yInit + 0.001f, zInit - (j + 2))),
                        b ? c1 : c2, 1);
            b = !b;
        }
    }
}

void drawAndares() {
    Position color = Position(0.9, 0.8, 0.8, 1);

    for (int i = 2; i <= N_ANDARES + 1; ++i) {
        drawParedeFrente(color, i);
        drawParedeFundo(color, i);
        drawParedeEsquerda(color, i);
        drawParedeDireita(color, i);
        drawPiso(Position(0.7, 0.5, 0.4, 1), i);
    }

    drawPiso(Position(0.25, 0.275, 0.3, 1), N_ANDARES + 2);
    drawHeliporto(N_ANDARES + 2);

    drawTransparentes();
}

void draw() {
    clearBuffers();
    setViewAngle();
    drawTerreno(Position(0.2, 0.5, 0.3));
    drawTerreo();
    drawAndares();
    glutSwapBuffers();
}

void mouseClick(int btn, int state, int, int)
{
    if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {

    }
    if(btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
        exit(1);
    }
}

void mouseMove(int, int) {

}

static void keyboardKeys(unsigned char key, int, int) {
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
        default:
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
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("Sketch WOGL");
    glClearColor(0.2, 0.2, 0.5, 0);
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
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
    GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};// "cor"
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
    GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;
    // Define a refletância do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    // Define os parâmetros da luz de número 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

    //Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    //Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    //Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);
}

void createParedeFrenteTerreoPolygons(std::vector<Polygon> &parede) {
    float x[4];
    x[0] = xInit;
    x[1] = x[0] + w1 * 0.3f;
    x[2] = x[1] + w1 * 0.4f;
    x[3] = x[2] + w1 * 0.3f;

    parede.push_back(Polygon(Position(x[0], yInit, zInit), Position(x[1], yInit + h, zInit)));
    parede.push_back(Polygon(Position(x[1], yInit + h * 0.7f, zInit), Position(x[2], yInit + h, zInit)));
    parede.push_back(Polygon(Position(x[2], yInit, zInit), Position(x[3], yInit + h, zInit)));
}

void createTerreo() {
    createParedeFrenteTerreoPolygons(paredesFrenteTerreo);
    paredesFundoTerreo.push_back(Polygon(Position(xInit, yInit, zInit - w2), Position(xInit + w1, yInit + h, zInit - w2)));
    paredesEsquerdaTerreo.push_back(Polygon(Position(xInit, yInit, zInit), Position(xInit, yInit + h, zInit - w2)));
    paredesDireitaTerreo.push_back(Polygon(Position(xInit + w1, yInit, zInit), Position(xInit + w1, yInit + h, zInit - w2)));
}

void createParedeFrenteFundoPolygons(float z, std::vector<Polygon> &parede, std::vector<Polygon> &vidros) {
    float x[8];
    x[0] = xInit; x[1] = x[0] + w1 * espacoFinalFrente; x[2] = x[1] + w1 * janelaFrente;
    x[3] = x[2] + w1 * espacoJanelasFrente; x[4] = x[3] + w1 * sacadaFrente; x[5] = x[4] + w1 * espacoJanelasFrente;
    x[6] = x[5] + w1 * janelaFrente; x[7] = x[6] + w1 * espacoFinalFrente;

    float yJan[4];
    yJan[0] = yInit; yJan[1] = yJan[0] + h * alturaEspacoJanelaFrente;
    yJan[2] = yJan[1] + h * alturaJanelaFrente; yJan[3] = yJan[2] + h * alturaEspacoFinalJanelaFrente;

    float ySac[4];
    ySac[0] = yInit; ySac[1] = ySac[0] + h * alturaEspacoFinalSacadaFrente;
    ySac[2] = ySac[1] + h * alturaSacadaFrente; ySac[3] = ySac[2] + h * alturaEspacoFinalSacadaFrente;

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

    vidros.push_back(Polygon(Position(x[1], yJan[1], z), Position(x[2], yJan[2], z)));
    vidros.push_back(Polygon(Position(x[5], yJan[1], z), Position(x[6], yJan[2], z)));
    vidros.push_back(Polygon(Position(x[3], ySac[1], z), Position(x[4], ySac[1] + h * 0.4f * alturaSacadaFrente, z)));
}

void createParedeLadoPolygons(float x, std::vector<Polygon> &parede, std::vector<Polygon> &vidros) {
    float z[10];
    z[0] = zInit; z[1] = z[0] - w2 * espacoFinalLado; z[2] = z[1] - w2 * janelaLado; z[3] = z[2] - w2 * espacoJanelasLado;
    z[4] = z[3] - w2 * janelaBanheiroLado; z[5] = z[4] - w2 * espacoJanelasBanheiroLado; z[6] = z[5] - w2 * janelaBanheiroLado;
    z[7] = z[6] - w2 * espacoJanelasLado; z[8] = z[7] - w2 * janelaLado; z[9] = z[8] - w2 * espacoFinalLado;

    float yJan[4];
    yJan[0] = yInit; yJan[1] = yJan[0] + h * alturaEspacoJanelaLado;
    yJan[2] = yJan[1] + h * alturaJanelaLado; yJan[3] = yJan[2] + h * alturaEspacoFinalJanelaLado;

    float yBan[4];
    yBan[0] = yInit; yBan[1] = yBan[0] + h * alturaEspacoJanelaBanheiroLado;
    yBan[2] = yBan[1] + h * alturaJanelaBanheiroLado; yBan[3] = yBan[2] + h * alturaEspacoFinalJanelaBanheiroLado;

    parede.push_back(Polygon(Position(x, yInit, z[0]), Position(x, yInit + h, z[1])));
    parede.push_back(Polygon(Position(x, yJan[0], z[1]), Position(x, yJan[1], z[2])));
    parede.push_back(Polygon(Position(x, yJan[2], z[1]), Position(x, yJan[3], z[2])));
    parede.push_back(Polygon(Position(x, yInit, z[2]), Position(x, yInit + h, z[3])));
    parede.push_back(Polygon(Position(x, yBan[0], z[3]), Position(x, yBan[1], z[4])));
    parede.push_back(Polygon(Position(x, yBan[2], z[3]), Position(x, yBan[3], z[4])));
    parede.push_back(Polygon(Position(x, yInit, z[4]), Position(x, yInit + h, z[5])));
    parede.push_back(Polygon(Position(x, yBan[0], z[5]), Position(x, yBan[1], z[6])));
    parede.push_back(Polygon(Position(x, yBan[2], z[5]), Position(x, yBan[3], z[6])));
    parede.push_back(Polygon(Position(x, yInit, z[6]), Position(x, yInit + h, z[7])));
    parede.push_back(Polygon(Position(x, yJan[0], z[7]), Position(x, yJan[1], z[8])));
    parede.push_back(Polygon(Position(x, yJan[2], z[7]), Position(x, yJan[3], z[8])));
    parede.push_back(Polygon(Position(x, yInit, z[8]), Position(x, yInit + h, z[9])));

    vidros.push_back(Polygon(Position(x, yJan[1], z[1]), Position(x, yJan[2], z[2])));
    vidros.push_back(Polygon(Position(x, yJan[1], z[7]), Position(x, yJan[2], z[8])));
    vidros.push_back(Polygon(Position(x, yBan[1], z[3]), Position(x, yBan[2], z[4])));
    vidros.push_back(Polygon(Position(x, yBan[1], z[5]), Position(x, yBan[2], z[6])));
}

void createAndarPolygons() {
    createParedeFrenteFundoPolygons(zInit, paredesFrente, janelasFrente);
    createParedeFrenteFundoPolygons(zInit - w2, paredesFundo, janelasFundo);
    createParedeLadoPolygons(xInit, paredesEsquerda, janelasEsquerda);
    createParedeLadoPolygons(xInit + w1, paredesDireita, janelasDireita);
    piso = Polygon(Position(xInit, yInit + 0.001f, zInit), Position(xInit + w1, yInit + 0.001f, zInit - w2));
}

void createHeliporto() {
    heliporto.push_back(Polygon(Position(xInit + w1 * 0.2f, yInit + 0.01f, zInit - w2 * 0.3f),
                                Position(xInit + w1 * 0.3f, yInit + 0.01f, zInit - w2 * 0.7f)));
    heliporto.push_back(Polygon(Position(xInit + w1 * 0.7f, yInit + 0.01f, zInit - w2 * 0.3f),
                                Position(xInit + w1 * 0.8f, yInit + 0.01f, zInit - w2 * 0.7f)));
    heliporto.push_back(Polygon(Position(xInit + w1 * 0.3f, yInit + 0.01f, zInit - w2 * 0.47f),
                                Position(xInit + w1 * 0.7f, yInit + 0.01f, zInit - w2 * 0.53f)));
}

void createPolygons() {
    createTerreo();
    createAndarPolygons();
    createHeliporto();
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
