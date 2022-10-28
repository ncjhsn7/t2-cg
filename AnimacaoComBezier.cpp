#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <GL/glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "Ponto.h"
#include "Poligono.h"
#include "InstanciaBZ.h"
#include <vector>

#include "Temporizador.h"
#include "ListaDeCoresRGB.h"

Temporizador T;
double AccumDeltaT = 0;
Temporizador T2;

InstanciaBZ Personagens[10];

Bezier Curvas[20];
unsigned int nCurvas;

Ponto Min, Max;

bool desenha = false;

Poligono Mapa, MeiaSeta, Mastro, PontosDeControle;
vector < vector < int >> BezierCurves;

int nInstancias = 0;

float angulo = 0.0;

double nFrames = 0;
double TempoTotal = 0;
void animate() {
    double dt;
    dt = T.getDeltaT();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;
    {
        AccumDeltaT = 0;
        angulo += 2;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0) {
        cout << "Tempo Acumulado: " << TempoTotal << " segundos. ";
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames / TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}

void reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    glOrtho(Min.x, Max.x, Min.y, Max.y, -10, +10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DesenhaEixos() {
    Ponto Meio;
    Meio.x = (Max.x + Min.x) / 2;
    Meio.y = (Max.y + Min.y) / 2;
    Meio.z = (Max.z + Min.z) / 2;
    
    glBegin(GL_LINES);
    //  eixo horizontal
    glVertex2f(Min.x, Meio.y);
    glVertex2f(Max.x, Meio.y);
    //  eixo vertical
    glVertex2f(Meio.x, Min.y);
    glVertex2f(Meio.x, Max.y);
    glEnd();
}

void DesenhaSeta() {
    glPushMatrix();
    MeiaSeta.desenhaPoligono();
    glScaled(1, -1, 1);
    MeiaSeta.desenhaPoligono();
    glPopMatrix();
}

void DesenhaApontador() {
    glPushMatrix();
    glTranslated(-4, 0, 0);
    DesenhaSeta();
    glPopMatrix();
}

void DesenhaHelice() {
    glPushMatrix();
    for (int i = 0; i < 4; i++) {
        glRotatef(90, 0, 0, 1);
        DesenhaApontador();
    }
    glPopMatrix();
}

void DesenhaHelicesGirando() {
    glPushMatrix();
    glRotatef(angulo, 0, 0, 1);
    DesenhaHelice();
    glPopMatrix();
}

void DesenhaMastro() {
    Mastro.desenhaPoligono();
}


void DesenhaCatavento() {
    glLineWidth(3);
    glPushMatrix();
    // defineCor(BrightGold);
    DesenhaMastro();
    glPushMatrix();
    glColor3f(1, 0, 0); // R, G, B  [0..1]
    glTranslated(0, 3, 0);
    glScaled(0.2, 0.2, 1);
    defineCor(Blue);
    DesenhaHelicesGirando();
    glPopMatrix();
    glPopMatrix();
}

void CriaInstancias() {
    Personagens[0].Posicao = Ponto (0, 0);
    Personagens[0].Rotacao = -90;
    Personagens[0].modelo = DesenhaMastro;
    Personagens[0].Escala = Ponto (.3,.3,.3);
    nInstancias = 1;
}

void CarregaCurvasBezier() {
    ifstream file;
    file.open("CurvasBezier.txt");
    
    if (!file.is_open()) {
        cout << "bret";
        return;
    }
    
    int numCurvas;
    int pos;
    vector <int> curve;
    file >> numCurvas;
    for (int i = 0; i < numCurvas; i++) {
        curve.clear();
        for (int i = 0; i < 3; i++) {
            file >> pos;
            curve.push_back(pos);
        }
        BezierCurves.push_back(curve);
    }
    cout << "Curvas abertas com sucesso!" << endl;
}

void CarregaModelos() {
    Mapa.LePoligono("EstadoRS.txt");
    MeiaSeta.LePoligono("MeiaSeta.txt");
    Mastro.LePoligono("Mastro.txt");
    PontosDeControle.LePoligono("PontosDeControle.txt");
    CarregaCurvasBezier();
}

void CriaCurvas() {
    nCurvas = 13;
    for (int i = 0; i < nCurvas; i++) {
        Curvas[i] = Bezier(PontosDeControle.getVertice(BezierCurves.at(i).at(0)),
                           PontosDeControle.getVertice(BezierCurves.at(i).at(1)),
                           PontosDeControle.getVertice(BezierCurves.at(i).at(2)));
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    CarregaModelos();
    CriaCurvas();
    DesenhaMastro();
    CriaInstancias();
    float d = 6;
    Min = Ponto(-d, -d);
    Max = Ponto(d, d);
}

void DesenhaPersonagens(float tempoDecorrido) {
    for (int i = 0; i < nInstancias; i++) {
        Personagens[i].AtualizaPosicao(tempoDecorrido);
        Personagens[i].desenha();
    }
}

void DesenhaCurvas() {
    for (int i = 0; i < nCurvas; i++) {
        ListaDeCoresRGB::defineCor(Curvas[i].cor);
        Curvas[i].Traca();
    }
}
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glLineWidth(1);
    glColor3f(1, 1, 1); // R, G, B  [0..1]
    //Personagens[0].Posicao.x += .01;
    DesenhaEixos();
    DesenhaPersonagens(T2.getDeltaT());
    DesenhaCurvas();
    glutSwapBuffers();
}

void ContaTempo(double tempo) {
    Temporizador T;
    
    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while (true) {
        tempo -= T.getDeltaT();
        cont++;
        if (tempo <= 0.0) {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    
    switch (key) {
        case 27:
            exit(0);
            break;
        case 't':
            ContaTempo(3);
            break;
        case ' ':
            desenha = !desenha;
            break;
        default:
            break;
    }
}

void arrow_keys(int a_keys, int x, int y) {
    switch (a_keys) {
        case GLUT_KEY_LEFT:
            Personagens[0].Posicao.x -= 0.5;
            break;
        case GLUT_KEY_RIGHT:
            Personagens[0].Rotacao++;
            break;
        case GLUT_KEY_UP:
            glutFullScreen();
            break;
        case GLUT_KEY_DOWN:
            glutPositionWindow(500, 500);
            glutReshapeWindow(700, 500);
            break;
        default:
            break;
    }
}

int main(int argc, char ** argv) {
    cout << "Programa OpenGL" << endl;
    glutInit( & argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(600, 500);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Animacao com Bezier");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrow_keys);
    glutMainLoop();
    
    return 0;
}
