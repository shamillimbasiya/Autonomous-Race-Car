#include <cmath>
#ifndef calc_h
#define calc_h

using namespace std;

class Calc
{	
	public:
		Calc();
		float cosineTheorem(float dist1,float dist2, int degree);
		const float getToDegrees();
};

#endif
