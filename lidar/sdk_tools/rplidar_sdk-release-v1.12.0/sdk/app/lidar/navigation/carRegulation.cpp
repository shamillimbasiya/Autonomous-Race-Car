#include <cmath>
#include "carRegulation.h"

using namespace std;

CarRegulation::CarRegulation()
{
    calc = Calc();
    //prevConePair = prevCone;
}


/*
 * Calculate the distance to the center of the conePair h.
 */

float CarRegulation::distToConePair(vector<Cone> conePair) {
    Cone pointsLeft = conePair[0];
    Cone pointsRight = conePair[1];

    float leftConeLastDist = pointsLeft.getLastPointDistance(); 
    float rightConeFirstDist = pointsRight.getFirstPointDistance(); 
    float distDiff = leftConeLastDist - rightConeFirstDist;
    
    int leftConeLastDegree = pointsLeft.getLastPointDegree();
    int rightConeFirstDegree = pointsRight.getFirstPointDegree();
    int degDiff = rightConeFirstDegree - leftConeLastDegree;
    
    float distBetweenCones = calc.cosineTheorem(leftConeLastDist, rightConeFirstDist, degDiff); 

    float distToPort = -1; 
    float portDegree;
    float theta;

    if (distDiff > 0) //reglera vänster, på höger sida
    {
        
        theta = acos(((pow(distBetweenCones, 2) + pow(leftConeLastDist, 2) 
                            - pow(rightConeFirstDist, 2)) / (2 * leftConeLastDist * distBetweenCones)));
        portDegree = M_PI_2 - theta; 
        distToPort = leftConeLastDist * cos(portDegree);  
    }
    else if (distDiff < 0) //reglera höger, på vänster sida
    { 
        theta = acos(((pow(distBetweenCones, 2) + pow(rightConeFirstDist, 2) 
                            - pow(leftConeLastDist, 2)) / (2 * rightConeFirstDist * distBetweenCones)));
        portDegree = M_PI_2 - theta; 
        distToPort = rightConeFirstDist * cos(portDegree);  
    }
    //printf("The port degree: %f\n", portDegree);
    //printf("Distance to port is: %f\n", distToPort);
    return distToPort;
}

/*
 * Calculate e[n], if e[n] < 0 we are more to the left, if e[n] > 0 we
 * are more to the right. Otherwise if e[n] = 0 we are centered.
 */
float CarRegulation::distToCenter(vector<Cone> conePair)
{
    Cone pointsLeft = conePair[0];
    Cone pointsRight = conePair[1];
    float leftConeLastDist = pointsLeft.getLastPointDistance(); 
    float rightConeFirstDist = pointsRight.getFirstPointDistance(); 
    float distDiff = leftConeLastDist - rightConeFirstDist;
    
    
    int leftConeLastDegree = pointsLeft.getLastPointDegree();
    int rightConeFirstDegree = pointsRight.getFirstPointDegree();
    int degDiff = rightConeFirstDegree - leftConeLastDegree;
    float distBetweenCones = calc.cosineTheorem(leftConeLastDist, rightConeFirstDist, degDiff); 
    printf("Distance between cones: %f \n", distBetweenCones);

    float distToCenter = -1; //x
    float distToPort = distToConePair(conePair);
    printf("Distance to port is: %f\n", distToPort);
    
    if (distDiff > 0) //reglera vänster, på höger sida
    {
        distToCenter = sqrt(pow(leftConeLastDist, 2) - pow(distToPort, 2)) - (distBetweenCones / 2);
    }
    else if (distDiff < 0) //reglera höger, på vänster sida
    {
        distToCenter = -(sqrt(pow(rightConeFirstDist, 2) - pow(distToPort, 2)) - (distBetweenCones / 2));
    }

    return distToCenter;
}

float CarRegulation::calcAngleAlpha(vector<Cone> conePair)
{
    float leftConeLastDist = conePair[0].getLastPointDistance(); 
    float rightConeFirstDist = conePair[1].getFirstPointDistance();
    float height = distToConePair(conePair);
    
    float alpha;
    if (leftConeLastDist < rightConeFirstDist) //Left side 
    {
        alpha = calc.getToDegrees() * acos(height / rightConeFirstDist);
    }
    
    else //Right side 
    {
        alpha = calc.getToDegrees() * acos(height / leftConeLastDist);
    }
    return alpha;
}


/*
 * Calc angle theta 
 */
float CarRegulation::calcAngleRegulator(vector<Cone> conePair) 
{
    float theta;
    float alpha = calcAngleAlpha(conePair);
    
    float leftConeLastDist = conePair[0].getLastPointDistance();
    float rightConeFirstDist = conePair[1].getFirstPointDistance();
    
    float angleToCone;
    float angleDiff;
    
    if (leftConeLastDist < rightConeFirstDist) // Left side
    {
        angleToCone = conePair[1].getFirstPointDegree();
        angleDiff = angleToCone - 90;
        theta = alpha - angleDiff;
    }
    
    else // Right side
    {
        angleToCone = 180 - conePair[0].getLastPointDegree();
        angleDiff = angleToCone - 90;
        theta = -(alpha - angleDiff);
    }
    return theta;
}

/*
 * Calculates the turn segment based on the regulator value it gets from e[n] and the angle or
 * based on the previous cone pair and the angle. 
 */ 
int CarRegulation::calcTurn(vector<Cone> conePair, string &prevConePair, bool &prevPairIsSet, int &laps) {
    float error = distToCenter(conePair);
    float angle = calcAngleRegulator(conePair);
    float alfa = 2;
    float beta = 28;
    int turn; // ska vara mellan 0-30

    int regulatorValue; 
    float distToPort = distToConePair(conePair);
    
    if (distToPort > 1050)
    {
        prevPairIsSet = false;
    }
    
    else if (distToPort <= 1000) 
    {
        
        if (!prevPairIsSet)
        {
            prevPairIsSet = true;
            prevConePair = obj.getConeAction(conePair);
            
            // Count laps
            if (prevConePair == "START")
            {
                laps += 1;
            }
        }
    
       /* if (prevConePair == "STRAIGHT" && (obj.getConeAction(conePair) == "LEFT" || obj.getConeAction(conePair) == "RIGHT") && distToPort >= 500)
        {
            prevConePair = obj.getConeAction(conePair);
        }*/
    }
        
    
    if (prevConePair == "RIGHT" && abs(angle) > 40)
    {
        regulatorValue = -1500;
    }
    else if (prevConePair == "LEFT" && abs(angle) > 40)
    {
        regulatorValue = 1500;
    }
    else 
    {
        regulatorValue = round(alfa * error + beta * angle);
    }
    
    if (regulatorValue > 1500) 
    {
        regulatorValue = 1500;
    }
    
    else if (regulatorValue < -1500) 
    {
        regulatorValue = -1500;
    }
    
    turn = 15 + floor(regulatorValue / 100); 
    
    printf("distToCenter: %f\n", error);
    //printf("angleTheta: %f\n", angle);
    //printf("regulatorValue (U[n]): %d\n", regulatorValue);
    printf("turn: %d\n", turn);
    
    printf("LEFT: ");
    conePair[0].print();
    printf("RIGHT: ");
    conePair[1].print();
    
    int leftConeFirstDeg = conePair[0].getFirstPointDegree();
	int leftConeLastDeg = conePair[0].getLastPointDegree();
	int leftDegDiff = leftConeLastDeg - leftConeFirstDeg;
	float leftConeWidth = calc.cosineTheorem(conePair[0].getFirstPointDistance(), conePair[0].getLastPointDistance(), leftDegDiff);
    
    int rightConeFirstDeg = conePair[1].getFirstPointDegree();
	int rightConeLastDeg = conePair[1].getLastPointDegree();
	int rightDegDiff = rightConeLastDeg - rightConeFirstDeg;
	float rightConeWidth = calc.cosineTheorem(conePair[1].getFirstPointDistance(), conePair[1].getLastPointDistance(), rightDegDiff);
    
    printf("Left ConeWidth: %f, Right ConeWidth: %f", leftConeWidth, rightConeWidth);
    return turn;
}


