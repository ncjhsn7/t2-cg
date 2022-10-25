//https://community.khronos.org/t/color-tables/22518/5

#ifndef ListaDeCoresRGB
#define ListaDeCoresRGB

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

enum Cores
{
    Red,
    Green,
    Blue,
    Magenta,
    Teal
};

void defineCor(int c);
int nextColor();

#endif
