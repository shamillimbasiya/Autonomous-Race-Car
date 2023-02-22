#include <map>
#include <string.h>
#include <iostream>

#include "cone.h"
#include "calc.h"


using namespace std;

/*
 * Cone constructor
 */ 
Cone::Cone()
{
	calc = Calc();
}

/*
 * Clears the contet of the cone
 */ 
void Cone::clear()
{
	return cone.clear();
}

/*
 * Adds a point that hit the cone
 */ 
void Cone::addPoint(int degree, float distance) 
{
	cone[degree] = distance;
}

/*
 * Checks if the cone is empty
 */ 
bool Cone::isEmpty() 
{
	return cone.empty();
}

/*
 * Get the number of points
 */ 
int Cone::getNumPoints()
{
	return cone.size();
}

/*
 * Get distance for a specfic point
 */ 
float Cone::getDistance(int degree) 
{
	return cone[degree];
}

/*
 * Get the distance of the first point that hit the cone
 */ 
float Cone::getFirstPointDistance()
{
	return cone.cbegin()->second;
}

/*
 * Get the distance of the last point that hit the cone
 */ 
float Cone::getLastPointDistance()
{
	return cone.rbegin()->second;
}

/*
 * Get the degree of the first point that hit the cone
 */ 
int Cone::getFirstPointDegree()
{
	return cone.cbegin()->first;
}

/*
 * Get the degree of the last point that hit the cone
 */ 
int Cone::getLastPointDegree()
{
	return cone.rbegin()->first;
}

/*
 * Prints all the values of the points that hit the cone
 */ 
void Cone::print()
{
	// [Cone: {deg: x, dist: y}, {deg: x, dist: y}...]
	printf("[Cone: ");
	for (auto it = cone.cbegin(); it != cone.cend(); ++it)
	{
		printf("{deg: %d, dist: %f}, ", it->first, it->second);
	}
	printf("]\n");
}

/*
 * Returns the type of cone based on the size 
 */ 
string Cone::getType()
{    
    string coneType = "";
    int coneFirstDeg = getFirstPointDegree();
	int coneLastDeg = getLastPointDegree();
	int degDiff = coneLastDeg - coneFirstDeg;
	
	float coneWidth = calc.cosineTheorem(getFirstPointDistance(), getLastPointDistance(), degDiff);
	
	if (coneWidth <= 190)
	{
		return SMALL;
	}
	return BIG;
}

/*
 * Converts all the data to a printable string
 */ 
string Cone::stringToPrint()
{
	//printf("{deg: %d, dist: %f, type: %s}, ", getFirstPointDegree(), getFirstPointDistance(), getType());
	// stringToPrint: "deg, dist, type"
	string stringToPrint = to_string(getFirstPointDegree()) 
							+ ", " + to_string(getFirstPointDistance()) 
							+ ", " + getType();
	return stringToPrint;
}



