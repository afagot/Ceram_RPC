//******************************************************
//*      GIF Cosmic Muons Monte Carlo Simulation       *
//*                                                    *
//*      Alexis FAGOT                                  *
//*      alexis.fagot@ugent.be                         *
//*                                                    *
//*      October 18, 2016                              *
//*                                                    *
//*      HEADER FILE                                   *
//******************************************************


#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

//pi Definition
const double PI = 4*atan(1);
const double mFlux = 1e-4;

//********************************************************************************************

//Function to cast an integer into a string
string intTostring(int value){
    string word;
    stringstream ss;
    ss << setw(2) << setfill('0') << value;
    ss >> word;

    return word;
}

//Function to cast a float into a string
string floatTostring(float value){
    string word;
    stringstream ss;
    ss.setf(ios::fixed, ios::floatfield);
    ss.precision(3);
    ss << value;
    ss >> word;

    return word;
}

//Function to convert deg angles to rad
float DegToRad(float deg){
    return deg*PI/180.;
}

//Function to convert rad angles to deg
float RadToDeg(float rad){
    float deg = rad*180./PI;
    if(deg < 0.) deg += 360.;
    return deg;
}

//********************************************************************************************

//RPC constants
const float GAS = 2.;
const float LENGTH = 35.;

//Dimensions of the SCINTILLATORS
const float SCINT_LENGTH = 45.;
const float SCINT_TOP = 63.;
const float SCINT_BOT = -53.;
const float SCINT_THICKNESS = 15.;
const float SCINT_AREA = SCINT_LENGTH*SCINT_LENGTH;

//Reference point of the RPC
const float A_X0 = 0.;
const float A_Y0 = 0.;
const float A_Z0 = 0.;

//const float max_theta = atan(sqrt(2)*LENGTH/(3*BOX_THICKNESS+BAR_LENGTH));
const float max_theta = PI/2.;

//********************************************************************************************

struct Point{
    float x;
    float y;
    float z;
};

typedef std::pair<double,double> Direction;

//********************************************************************************************

typedef std::mt19937 Generator;

double getRandom(Generator& generator){
    return generate_canonical<double, 32>(generator);
}

double getRandomInRange(Generator& generator, double x0, double length){
    return length*getRandom(generator) + x0;
}

Direction getRandomDirection(Generator& generator){
    double phi = 2*PI*getRandom(generator);

    // Create cos²(theta) distribution
    double theta = PI*getRandom(generator)/2.;
    double test = getRandom(generator);

    while (test > cos(theta)*cos(theta)){
        theta = PI*getRandom(generator)/2.;
        test = getRandom(generator);
    }

    return make_pair(theta, phi);
}

Point getRandomMuonPosition(Generator& generator){
    Point muon;

    muon.x = getRandomInRange(generator, -SCINT_LENGTH/2., SCINT_LENGTH/2.);
    muon.y = getRandomInRange(generator, -SCINT_LENGTH/2., SCINT_LENGTH/2.);
    muon.z = (SCINT_BOT+SCINT_TOP)/2.;

    return muon;
}

float getDistance(Point A, Point B){
    float dx = B.x - A.x;
    float dy = B.y - A.y;
    float distance = sqrt(dx*dx+dy*dy);
    return distance;
}

bool isInTopScint(const Point& P, const Direction& D){
    //Top Scint
    Point T;

    T.z = SCINT_TOP;
    T.x = P.x - (P.z-T.z)*sin(D.first)/cos(D.first)*cos(D.second);
    T.y = P.y + (P.z-T.z)*sin(D.first)/cos(D.first)*sin(D.second);

    bool isInTop = (abs(T.x) <= SCINT_LENGTH/2.) && (abs(T.y) <= SCINT_LENGTH/2.);

    return isInTop;
}

bool isInBotScint(const Point& P, const Direction& D){
    //Bottom Scint
    Point B;

    B.z = SCINT_BOT;
    B.x = P.x - (P.z-B.z)*sin(D.first)/cos(D.first)*cos(D.second);
    B.y = P.y + (P.z-B.z)*sin(D.first)/cos(D.first)*sin(D.second);

    bool isInBottom = (abs(B.x) <= SCINT_LENGTH/2.) && (abs(B.y) <= SCINT_LENGTH/2.);

    return isInBottom;
}

bool isInRPC(const Point& P, const Direction& D){
    Point I;

    I.z = 0.;
    I.x = P.x - P.z*sin(D.first)/cos(D.first)*cos(D.second);
    I.y = P.y + P.z*sin(D.first)/cos(D.first)*sin(D.second);

    return (abs(I.x) <= LENGTH/2.) && (abs(I.y) <= LENGTH/2.);
}

#endif // FONCTIONS_H
