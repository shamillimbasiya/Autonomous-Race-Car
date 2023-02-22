#include <map>
#include <string.h>
#include <iostream>
#include "calc.h"

#ifndef cone_h
#define cone_h

using namespace std;

class Cone
{
	public:
		Cone();
		void clear();
		
		string getType();
		void addPoint(int degree, float distance);
		bool isEmpty();
		
		int getNumPoints();
		float getDistance(int degree);
		float getFirstPointDistance();
		float getLastPointDistance();
		int getFirstPointDegree();
		int getLastPointDegree();
		void print();
		string stringToPrint();
	
	private:
		Calc calc;
		map<int, float> cone;
		
		const string BIG = "BIG";
		const string SMALL = "SMALL";
}; 

#endif
