#include <vector>
#include "cone.h"
#include "calc.h"
#include <string.h>
#include <iostream>
#include "objDetection.h"

#ifndef carRegulation_h
#define carRegulation_h

using namespace std;

class CarRegulation
{	
	public:
		CarRegulation();
		float distToCenter(vector<Cone> cones);
		float distToConePair(vector<Cone> conePair);
		int calcTurn(vector<Cone> conePair,string &prevConePair, bool &prevPairIsSet, int &laps);
		
		float calcAngleRegulator(vector<Cone> conePair);
		float calcAngleAlpha(vector<Cone> conePair);

	private: 
		Calc calc;
		ObjDetection obj{};
};

#endif
