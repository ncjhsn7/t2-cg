#include "ListaDeCoresRGB.h"

//https://community.khronos.org/t/color-tables/22518/5

int colorCount = 0;

float Cores[][3] = {
    1,0,0,
    0,1,0,
    0,0,1,
    1,0,1,
    1,1,0,
};

void defineCor(int c)
{
   // glColor3fv(Cores[c]);
    glColor3f(Cores[c][0], Cores[c][1], Cores[c][2]);
}

int nextColor()
{
    if (colorCount > 4)
    {
        colorCount = 0;
    }
    int temp = colorCount;
    colorCount++;
    return temp;
}
