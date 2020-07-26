//  TODO: Write local testing script that clones github to a new temporary directory, compiles, and runs several test simulations

#include "System.h"
#include "physParam.h"
#include "csv.h"
#include "cmdout.h"

using namespace std;

physParam createPhysParam(){ ///A hard-coded helper function to edit PhysParams
    physParam toReturn; //make the physParam that we will return

    //step properties
    toReturn.stepSize = 0.01; // step size in seconds
    toReturn.stepMax = 100000; // how many steps the simulation should run for

    //output information
    toReturn.outputSteps = 1000; // after how many steps should you output?
    toReturn.outDirPath = "./Output/MSD_Calculations/Run5/VTP"; //path to output directory
    toReturn.outFileName = "VisualiserData"; //what file to store data into. .vtp extension should NOT be included
    toReturn.outFileType = "vtp"; //file tpye to save as (should be vtp, no dots)

    //particle properties
    toReturn.N = 300; //if randomly generating particles, this is how many to generate
    toReturn.meanR = 1; // mean radius
    toReturn.v_0 = 0.05; // active velocity parameter
    toReturn.sigmaR = 0.1; // standard deviation of the radius
    toReturn.sigmaV = 0; // standard deviation for when generating gaussian velocities
    toReturn.massRadiusRatio = 1; //ratio of particle mass to radius used for finding mass of particles

    //boundary consts
    toReturn.periodic = true; // is the boundary periodic or no?
    toReturn.L_x = 40;
    toReturn.L_y = 40;
    toReturn.overlapRatio = 0; //overlap ratio if the boundary is not periodic

    //csv information
    toReturn.loadParticles = false; // whether to load particles or not
    toReturn.pathToParticles = ".CSVs/DebugPLists/SingleAlongRandAxis.csv"; //full file path to csv containing particle information
    toReturn.pathToLoadingCSV = "./Output/MSD_Calculations/Run5/"; //file path to where debug particles are stored
    toReturn.particleDumpSteps = 0; //after how many steps to dump particle data. 0 for no dumping
    toReturn.pathToParticleData = "./Output/MSD_Calculations/Run5/ParticleData/"; //file path to the directory where particle data should be stored
    toReturn.dumpSingleParticle = false; //whether to dump a single particle or not

    //force bools - Do we want to consider the following forces in our sim, yay or nay?
    toReturn.enableHarmonicInterForce = true;
    toReturn.enableHertzianInterForce = false;
    toReturn.enableActiveForce = true;
    toReturn.enableGroundFrictionForce = true;
    toReturn.enablePersonFrictionForce = true;
    toReturn.enableRandNoisyForce = false;

    //force parameters
    toReturn.zetaActive = 1; // friction parameter for active propulsion
    toReturn.zetaGround = 1; // friction parameter against the ground
    toReturn.zetaPerson = 1; // friction parameter against other people
    toReturn.kHarmonic = 1; // harmonic spring const between particles
    toReturn.kHertzian = 1; // hertzian const between particles
    toReturn.sigmaForceX = 0.2; // standard deviation for the force resulted from noise in the x axis
    toReturn.sigmaForceY = 0.2; // standard deviation for the force resulted from noise in the y axis

    //torque bools - Do we want to conside the following torques in oru sim, yay or nay?
    toReturn.enablePolarAlignmentTorque = false;
    toReturn.enableVelocityAlignmentTorque = false;
    toReturn.enableAngularFrictionTorque = true;
    toReturn.enablePairDissipationTorque = true;
    toReturn.enableRandNoisyTorque = true;

    //torque parameters
    toReturn.xiAngular = 1; //angular friction coeff
    toReturn.xiPair = 1; //pair dissipation coeff
    toReturn.zetaPolar = 1; //polar alignment coeff
    toReturn.zetaVelocity = 0.1; //velocity alignment coeff
    toReturn.sigmaTorque = 0.2; //SD for the torque resulted from noise

    //debug parameters
    toReturn.debugType = physParam::None; //see physparam header for full list of debug states

    //return the constructed physParam
    return toReturn;
}

//rewrite the input physParam using the standard "experiment" format
physParam rewriteParamForExperiment(physParam input, std::string expDirectory){
    input.outDirPath = expDirectory+"/VTP"; //path to output directory
    input.pathToParticles = expDirectory+"/initPartData.csv"; //full file path to csv containing particle information for loading
    input.pathToLoadingCSV = expDirectory+"/"; //file path to where debug particle data (ICs) are stored
    input.pathToParticleData = expDirectory+"/ParticleData/"; //file path to the directory where particle data should be stored
    return input;
}

/* Perform actions depending on the arguments given to main. Argument list includes:
    -loadPhysParam: load phys param csv file from specified operand
    -expDir: force experiment directory to specified operand
    -loadParticles: load particle data from specified operand
    -v OR -verbose: set cmd output to verbose (more outputs!)
*/
void performArgumentActions(std::string argument, std::string operand, physParam &param){
    if(argument=="-loadPhysParam"){ 
        cmdout::cmdWrite(false, "Importing parameters from " + operand);
        param = csv::importPhysParam(operand);
    } 
    else if(argument=="-expDir"){ 
        cmdout::cmdWrite(false, "Forcing experiment format. Experiment directory set to " + operand);
        param = rewriteParamForExperiment(param, operand);
   } 
   else if(argument=="-loadParticles"){ 
        cmdout::cmdWrite(false, "Forcing loading particles from " + operand);
        param.loadParticles = true;
        param.pathToParticles = operand;
    } 
    else if(argument=="-v"||argument=="-verbose"){
        cmdout::setVerbose();
    }
    else {
        //make an error warning the user that the parameter wasn't understood
        cmdout::cmdWrite(true, "Did not understand program argument " + argument + "="+operand);
    }
}

int main(int argc, char* argv[])
{
    cmdout::cmdWrite(true, "================================");
    cmdout::cmdWrite(true, "|  Custom Crowds in C++ (CCC)  |");
    cmdout::cmdWrite(true, "================================");
    cmdout::cmdWrite(true, " "); //newline for spacing
 
    physParam sysParam = createPhysParam(); // set up phys param object in case we need it
    //TODO: Edit the above once I make the phys param handler class (to set up for pybind)

    //extract data from each argument. expect arguments in the form -OPERATOR=OPERAND
    for(int i = 0; i<argc; i++){ //loop through the arguments
        std::string arg = argv[i]; //write curr argument as a string for simplicity
        
        //first split into operator and operand, code copied and edited from csv.cpp
        std::stringstream s(arg); //making the stream which does the splitting based on the line
        std::vector<std::string> argComponents;
        std::string currWord; //the current word

        while(std::getline(s, currWord, '=')){ //loop through every instance of "="
            argComponents.push_back(currWord); //add each "word" to the toReturn vector
        }

        performArgumentActions(argComponents[0], argComponents[1], sysParam); //pass operator then operand to the handling function
    }

    ///Uncomment below to force an experiment through hardcoding main
    //sysParam  = rewriteParamForExperiment(sysParam, "/home/tk16693/Documents/Summer19_Project/ProgramData/Output/temp");
    ///Uncomment below to force loading of system parameter data
    //sysParam = csv::importPhysParam("/home/tk16693/Documents/Summer19_Project/ProgramData/Output/Flocking/350parts/Run1/PhysParamData.csv");

    //set flag to dump particle data of a single particle if necessary
    if(sysParam.dumpSingleParticle){
        csv::setupSingleFile(sysParam.pathToParticleData);
    }

    ///Uncomment below to export the sysParam
    csv::exportPhysParam(sysParam, sysParam.pathToLoadingCSV); //export the sysParam we have at this point for later usage by default

    ///Uncomment below to run a simulation based on sysParam
    System simulation(sysParam); //init the system
    simulation.runSimulation(); //run the system

    return 0;
}
