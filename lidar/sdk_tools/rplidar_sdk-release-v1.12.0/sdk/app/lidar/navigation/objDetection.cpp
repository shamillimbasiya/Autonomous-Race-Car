#include <map>
#include <string.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "cone.h"
#include "calc.h"
#include "objDetection.h"

using namespace std;

/*
 * Construtor for the object dection object
 */ 
ObjDetection::ObjDetection()
{
	calc = Calc();
}

/*
 * Finds all the cones in the lidars first and last 90 degrees
 */ 
vector<Cone> ObjDetection::findCones(float distance[360])
{
	vector<Cone> cones;
    Cone cone = Cone();
    bool pointIsWall = false;
    
    int degree = 270;
    int diffDegree = 0;
    for (int i = 0; i < 180; i++)
    {
        // If the dataPoint is in the allowed distance interval
        if (distance[degree] <= 2800 && distance[degree] >= 100)
        {
            // If we have not added any dataPoints to the cone
            if(cone.isEmpty())
            { 
                cone.addPoint(i, distance[degree]);
                diffDegree = 0;
            } 
            
            else 
            {
                float diffDist = cone.getDistance(i-1) - distance[degree];
                
                // If the difference in distance between two dataPoints is greater than 20cm,
                // it must be a wall
                if (diffDist > 200)
                {
                    pointIsWall = true;
                }
                
                // else if, check coneWidth and then add the dataPoint to the cone
                else if (abs(diffDist) < 200)
                {
                    diffDegree += 1;
                    float firstDist = cone.getFirstPointDistance();
                    float lastDist = distance[degree];
                    float cathLength = calc.cosineTheorem(firstDist, lastDist, diffDegree);
                    
                    // If the cone width is greater than 33cm, the found cone must be a wall
                    if (cathLength > 330)
                    {
                        pointIsWall = true;
                    }
                    cone.addPoint(i, distance[degree]);
                }
                
                // if diffDist is < -200 we have reached the last dataPoint of a cone
                else 
                {
                    if (!pointIsWall && cone.getNumPoints() > 1) 
                    {
                        cones.push_back(cone);
                    }
                    
                    cone.clear();
                    cone.addPoint(i, distance[degree]);
                    diffDegree = 0;
                    pointIsWall = false; 
                    
                }
            }
        }
        
        else if(!cone.isEmpty()) 
        {
            if (!pointIsWall && cone.getNumPoints() > 1) 
            {
				cones.push_back(cone);
            }
            cone.clear();
            diffDegree = 0;
            pointIsWall = false;
        }
        
        degree += 1;
        
        if (degree == 360)
        {
            degree = 0;
        }
    }
    return cones;
}

/*
 * Finds the two nearest cones from all the cones it found 
 */ 
vector<Cone> ObjDetection::nearestConePair(vector<Cone> cones)
{
	int numCones = cones.size();
    
    int coneNum1 = -1;
    int coneNum2 = -1;
    
    for (int i = 0; i < numCones; i++) 
    {
        Cone cone1 = cones[i];
        
        for (int j = i+1; j < numCones; j++) 
        {
            Cone cone2 = cones[j];
            
            float cone1LastDist = cone1.getLastPointDistance();
            float cone2FirstDist = cone2.getFirstPointDistance();
            int cone1LastDeg = cone1.getLastPointDegree();
            int cone2FirstDeg = cone2.getFirstPointDegree();
            
            int degDiff = cone2FirstDeg - cone1LastDeg;
            float distBetweenCones = calc.cosineTheorem(cone1LastDist, cone2FirstDist, degDiff);
            
            if (distBetweenCones >= 650 && distBetweenCones <= 850) 
            {
                if (coneNum1 == -1 && coneNum2 == -1) 
                {
                    coneNum1 = i;
                    coneNum2 = j;
                }
                
                else 
                {
                    float nearestCone1Dist = cones[coneNum1].getLastPointDistance();
                    float nearestCone2Dist = cones[coneNum2].getFirstPointDistance();
                    float currMeanDistToPort = (nearestCone1Dist + nearestCone2Dist) / 2;
                    
                    float compMeanDistToPort = (cone1LastDist + cone2FirstDist) / 2;
                    
                    if (compMeanDistToPort < currMeanDistToPort) 
                    {
                        coneNum1 = i;
                        coneNum2 = j;
                    }
                } 
            }
        }
    }
       
    //printf("conePair: [coneNum1: %d, coneNum2: %d]\n", coneNum1, coneNum2);
    
   if (coneNum1 != -1 && coneNum2 != -1) 
    {
		Cone cone1 = cones[coneNum1];
		Cone cone2 = cones[coneNum2];
		cones.clear();
		cones.push_back(cone1);
		cones.push_back(cone2);
        return cones;
    }
    cones.clear();
    return cones;
}

/*
 * Returns the action type based on the size of the cones
 */ 
string ObjDetection::getConeAction(vector<Cone> conePair)
{
    //printf("Left ");
	string leftConeType = conePair[0].getType();
    
    //printf("Right ");
	string rightConeType = conePair[1].getType();
	string actionType = "";
    
    if (leftConeType == SMALL && rightConeType == SMALL) 
    {
        actionType = STRAIGHT;
    }
    
    else if (leftConeType == SMALL && rightConeType == BIG) 
    {
        actionType = LEFT;
    }
    
    else if (leftConeType == BIG && rightConeType == SMALL) 
    {
        actionType = RIGHT;
    }
    
    else if (leftConeType == BIG && rightConeType == BIG) 
    {
        actionType = START;
    }
    return actionType;
}
