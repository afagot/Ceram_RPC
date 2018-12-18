//******************************************************
//*      GIF Cosmic Muons Monte Carlo Simulation       *
//*      RPC School Mexico 2018                        *
//*                                                    *
//*      Alexis FAGOT                                  *
//*      alexis.fagot@ugent.be                         *
//*                                                    *
//*      February 15, 2018                             *
//*                                                    *
//*      MAIN FILE                                     *
//******************************************************


#include "../include/fonctions.h"

#include <fstream>
#include <random>

#include "TFile.h"
#include "TH2F.h"
#include "TTree.h"

using namespace std;

int main(){
    /** PROGRAM CODE **/
    // Start with random seed
    random_device rd;
    Generator generator(rd());

    // Get the thing started
    generator.discard(100);

    //Definitions of the coordinates and of the cosmic direction
    Point muonPos;
    Direction dir_gen;

    //Set the number of muons to detect in the trigger during the simulation
    int Nmuons = 1e4;

    int Niterations = 100;
    double acceptance[100] = {0.};
    double average = 0.;
    double error = 0.;

    for(int i=0; i<Niterations; i++){
        //Variables to calculate geometrical acceptance
        int NTop = 0;
        int NTriggers = 0;
        int NDetected = 0;

        //Set number to follow the status of the simulation through
        //a progression bar
        float Stat = (float)Nmuons;
        float progress = 0.;

        while(NTop<Nmuons){
            muonPos = getRandomMuonPosition(generator);
            dir_gen = getRandomDirection(generator);

            if(isInTopScint(muonPos,dir_gen)){
                NTop++;
                if(isInBotScint(muonPos,dir_gen)){
                    NTriggers++;
                    if(isInRPC(muonPos,dir_gen)){
                        NDetected++;
                    }
                }
            }
            //Print progress bar
            progress = float(NTop/Stat);

            int barWidth = 70;

            cout << "Progress [";
            int pos = barWidth * progress;
            for (int i = 0; i < barWidth; ++i) {
                if (i < pos) cout << "=";
                else if (i == pos) cout << ">";
                else cout << " ";
            }
            cout << "] " << int(progress * 100.0) << " %\r";
            cout.flush();
        }
        cout << endl;

        float rate = (double)NTriggers/NTop*mFlux*SCINT_AREA;
        float rate_per_min = rate*60.;
        cout << "Expected trigger rate = " << rate << "Hz (" << rate_per_min << "muons/min)" << endl;

        acceptance[i] = (double)NDetected/NTriggers;
        average += acceptance[i];
    }

    average /= 100.;

    double variance = 0.;

    for(int i=0; i< Niterations; i++){
        variance += (acceptance[i]-average)*(acceptance[i]-average);
    }

    error = 2*sqrt(variance)/Niterations;

    cout << "Geometrical acceptance of the setup = " << average << " +- " << error << endl;

    return 0;
}
