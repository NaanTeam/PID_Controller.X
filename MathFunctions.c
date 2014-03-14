
#include "MathFunctions.h"


//******************************************************************************
//Public Function Definitions
//******************************************************************************

float Vector_Dot_Product(float vector1[3],float vector2[3])
{
    float op=0;
    int c = 0;

    while (c < 3)
    {
        op+=vector1[c]*vector2[c];
        c++;
    }

    return op;
}


void Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3])
{
    vectorOut[0]= (v1[1]*v2[2]) - (v1[2]*v2[1]);
    vectorOut[1]= (v1[2]*v2[0]) - (v1[0]*v2[2]);
    vectorOut[2]= (v1[0]*v2[1]) - (v1[1]*v2[0]);
}


void Vector_Scale(float vectorOut[3],float vectorIn[3], float scale2)
{
    int c = 0;

    while (c < 3)
    {
        vectorOut[c]=vectorIn[c]*scale2;
        c++;
    }
}

void Vector_Add(float vectorOut[3],float vectorIn1[3], float vectorIn2[3])
{
    int c = 0;

    while (c < 3)
    {
        vectorOut[c]=vectorIn1[c]+vectorIn2[c];
        c++;
    }
}



void Matrix_Multiply(float matrixOut[3][3], float matrixIn1[3][3], float matrixIn2[3][3])
{
    float op[3];
    int x = 0; int y = 0; int w = 0;


    while (x < 3)
    {
        y = 0;
        while (y < 3)
        {
            w= 0;
            while (w < 3)
            {
                op[w]=matrixIn1[x][w]*matrixIn2[w][y];
                w++;
            }
            matrixOut[x][y]=op[0]+op[1]+op[2];

            y++;
        }
        x++;
    }
}


float constrain(float value, float minimum, float maximum)
{
    if (value > maximum)
    {
        return maximum;
    }
    else if (value < minimum)
    {
        return minimum;
    }
    else
    {
    return value;
    }
}



