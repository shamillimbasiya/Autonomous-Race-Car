#include <cmath>
#include <math.h>
#include <map>
#include <iostream>

using namespace std;

float calc_angle_regulator(float height, map<int, map<int, float> > cones) {
    float lengthLeftCone = cones[0].rbegin()->second;
    float lengthRightCone = cones[1].cbegin()->second;
    float toDegrees = 180/M_PI;

    float cone;
    int degree;

    if (lengthLeftCone < lengthRightCone) {
        cone = lengthRightCone;
        degree = cones[1].cbegin()->first;
    }
    else {
        cone = lengthLeftCone;
        degree = cones[0].rbegin()->first;
    }

    return (90 + acos(height/cone)*toDegrees - degree);
}

int main() {
    map<int, map<int, float> > cones;
    map<int, float> cone1;
    map<int, float> cone2;
    cone1[90] = 2.4;
    cone2[120] = 1.4;
    cones[0] = cone1;
    cones[1] = cone2;
    cout << calc_angle_regulator(2, cones);
}