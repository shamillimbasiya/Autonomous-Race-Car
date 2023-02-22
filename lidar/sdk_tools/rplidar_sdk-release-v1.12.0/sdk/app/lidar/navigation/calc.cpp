#include <cmath>
#include "calc.h"
#include <iostream>

using namespace std;

Calc::Calc(){}

/*
 * Calculates and returns the cathetus length in a triangel  
 */
  
float Calc::cosineTheorem(float dist1, float dist2, int degree)
{
    float rad = degree * (M_PI/180);
    float coss = pow(dist1, 2) + pow(dist2, 2) - 2*dist1*dist2*cos(rad);
    float cathLength = sqrt(coss);
    
    return cathLength;
}

/*
 * Returns the value used to convert rads to degress
 */ 
const float Calc::getToDegrees()
{
    return 180/M_PI;
}
