/*  Data-driven fine tuning on the previous experimental
Part of the MSc thesis work of Husrev Cilasun, supervised by Fatih Kamisli

Make this file with -std=c++11 or c++14
To configure and make the VP9 for intra-only mode, use the following lines in the main libvpx project folder
./sudo make clean
./configure --disable-sse --disable-sse2 --disable-sse3 --disable-ssse3 --disable-mmx --disable-vp8-encoder --disable-vp8-decoder
./sudo make

An example call for testing the vp9 binary is as follows:
./vpxenc Traffic_2560x1600_30_crop.yuv -o vp9_enc_bitstream.webm --width=2560 --height=1600 --codec=vp9 --fps=30/1 --end-usage=3 --limit=1 --passes=2 --kf-max-dist=1 --psnr --cq-level=28

When calling this function, DO NOT FORGET TO GAIN ADMINISTRATOR PRIVILEGES (sudo) 
Also don't forget to assign your pattern of test sequences to the compare_lossy.pl*/


#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <vector>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include "experiment.h"

int main()
{   
    std::string sequencesDirectory = "../testsequences/";
    double stepSize = 0.0001; // Step size in radians
    bool resumeExperiment = false;
    experiment finetuning(resumeExperiment);
    std::ios_base::openmode arg = resumeExperiment ? std::ofstream::app : arg = std::ofstream::out;

    // Open the log file
    std::ofstream logfile("log.txt", arg);
    logfile << "FINE TUNING EXPERIMENT LOG" << std::endl << std::endl;
    logfile << "Optimization started. StepSize=" << stepSize << " [rad] ";

    // Process the original VP9 binary and save results for future reference. 
    std::string init_vp9 = finetuning.process("orig_unopt","init_vp9", sequencesDirectory);

    // Start optimizing each angle.
    for (int angle = finetuning.getStartAngle(); angle < 16; angle++)
    {
        std::string bestCandidate; 
        int iteration = 0;

        // Loop until convergence for a specific angle.
        while(true)
        {
            // Save current coefficients and experiment results.
            finetuning.writeAngles();
            std::string baseline = finetuning.process("modified","baseline", sequencesDirectory);

            // Print the initial BDBR with respect to the original VP9 binary.
            if(!iteration && !angle)
            {
                std::cout << "Initial BDBR is " << finetuning.getBDBR(init_vp9, baseline);
                std::cout << std::endl;
                logfile << "and the initial BDBR is " << finetuning.getBDBR(init_vp9, baseline) ;
                logfile << "%" << std::endl << std::endl;
            }

            std::cout << "Started iteration " << iteration << " for angle " << angle << ": ";
            logfile << "    Started iteration " << iteration << " for angle#" << angle << ": ";

 
            // Obtain candidate-1 results for coefficient-1 for current angle
            finetuning.setAngle(angle, finetuning.getAngle(angle)-stepSize);
            finetuning.writeAngles();
            std::string experiment1 = finetuning.process("modified","candidate1", sequencesDirectory);
            long double cand1BDBR = finetuning.getBDBR(baseline, experiment1);

            // Obtain candidate-2 results for coefficient+1 for current angle
            finetuning.setAngle(angle, finetuning.getAngle(angle)+2*stepSize);
            finetuning.writeAngles();
            std::string experiment2 = finetuning.process("modified","candidate2", sequencesDirectory);
            long double cand2BDBR = finetuning.getBDBR(baseline, experiment2);

            iteration++;

            if(cand1BDBR < cand2BDBR && cand1BDBR < 0)
            {
                logfile << "Decreased by " << stepSize << " since it gives BDBR " << cand1BDBR << std::endl;

                // If candidate-1 outperforms the current coefficients and candidate-2
                finetuning.setAngle(angle, finetuning.getAngle(angle)-2*stepSize);
                bestCandidate = experiment1;
            }
            else if(cand2BDBR < cand1BDBR && cand2BDBR < 0)
            {
                logfile << "Increased by " << stepSize << " since it gives BDBR " << cand2BDBR << std::endl;

                // If candidate-2 outperforms the current coefficients and candidate-1
                bestCandidate = experiment2;
            }
            else
            { 
                logfile << "No change in angles since BDBR_cand1=" << cand1BDBR;
                logfile << " and BDBR_cand2=" << cand2BDBR << std::endl;

                // No candidate outperforms the rest. Therefore assert convergence and break.
                bestCandidate = baseline;
                finetuning.setAngle(angle, finetuning.getAngle(angle)-stepSize);
                finetuning.writeAngles();
                break;
            }
        }
        std::cout << "Optimized angle " << angle << ", current BDBR wrt VP9_ORIG is ";
        std::cout << finetuning.getBDBR(init_vp9, bestCandidate) << std::endl << std::endl;
        logfile << "Optimized angle " << angle << ", current BDBR wrt original VP9 is ";
        logfile << finetuning.getBDBR(init_vp9, bestCandidate) << std::endl << std::endl;
    }
    std::cout << "Fine-tuning completed." << std::endl;
    logfile << "Fine-tuning completed." << std::endl;
    logfile.close();

    return 0;
}