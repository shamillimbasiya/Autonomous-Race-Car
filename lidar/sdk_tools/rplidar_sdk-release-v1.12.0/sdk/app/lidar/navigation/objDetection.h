#include <map>
#include <string.h>
#include <iostream>
#include <vector>
#include "calc.h"
#include "cone.h"

#ifndef objDetection_h
#define objDetection_h

using namespace std;

class ObjDetection
{	
	public:
		ObjDetection();
		float calcCath(float dist1,float dist2, int degree);
		vector<Cone> findCones(float distance[360]);
		vector<Cone> nearestConePair(vector<Cone> cones);
		string getConeAction(vector<Cone> conePair);
				
	private:
		Calc calc;
		
		const string BIG = "BIG";
		const string SMALL = "SMALL";
		const string STRAIGHT = "STRAIGHT";
		const string LEFT = "LEFT";
		const string RIGHT = "RIGHT";
		const string START = "START";
};

#endif
