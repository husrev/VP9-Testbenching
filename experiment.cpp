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
#include <math.h>
#include "experiment.h"
#include "sequences.h"

typedef std::chrono::high_resolution_clock Clock;
const int max_buffer = 4096;

experiment::experiment(bool resume)
{
    // To resume a previous optimization, read coefficients from file.
    if(resume)
    {
        std::ifstream coeffsFileIn(anglesFile);
        for (int i=0; i<16; i++)
        {
            coeffsFileIn >> btf_angles[i];
        }
        coeffsFileIn.close();
        std::ifstream logFileIn("log.txt");
        std::string a;
        bool flag = false;
        int candidate = 0;
        while(logFileIn >> a)
        {            
            if(flag == true)
            {
                candidate = std::stol(a);
                flag = false;
            }
            flag = (a == "angle") ? true : false;
        }
        startAngle = candidate;
        logFileIn.close();
    }
}

bool experiment::writeAngles()
{
    // Write given coefficients to file.
    std::ofstream fileout(anglesFile);
    for(int i=0; i<16; i++)
    {
        fileout << (int) round(pow(2,14) * sin(btf_angles[i])) << std::endl;
    }
    for(int i=0; i<16; i++)
    {
        fileout << (int) round(pow(2,14) * cos(btf_angles[i])) << std::endl;
    }
    fileout.close();
}

long double experiment::getBDBR(std::string baseline, std::string exp1)
{
    // Prepare variables and the command to push into the BDBR calculation script.
    std::string cmd = ("sudo perl compare_lossy.pl " + baseline + ".txt "+exp1+".txt 0").c_str();
    FILE * stream;
    std::string data;
    char buffer[max_buffer];
    std::vector<std::string> results;

    // Try to access and push the command.
    try
    {
        stream = popen(cmd.c_str(), "r");
    }
    catch(const std::exception& e)
    {            
        std::cout << "ERROR: THE COMPARISON OUTPUT COULDN'T BE ACCESSED" << std::endl;
    }

    // Buffer the output.
    if (stream)
    {
        while (!feof(stream))
        {
            if (fgets(buffer, max_buffer, stream) != NULL)
            {
                data.append(buffer);
            }
        }
        pclose(stream);
    }

    // Clean whitespace and return BDBR.
    boost::split(results, data,[](char c){return isspace(c);});
    return ((long double)stod(results[0]));
}

std::string experiment::process(std::string binary, std::string exp1, std::string dir)
{
    // Prepare command arguments.
    std::cout << std::endl <<">> Processing binary " << binary << " for experiment " << exp1 << std::endl;
    std::string filename = exp1 + ".txt";
    std::string exebase = "./vpxenc_" + binary + " -o vp9_enc_bitstream_qp_";
    std::string ext = ".webm --codec=vp9 --fps=";
    std::string args1 = "/1 --end-usage=3 --limit=";
    std::string args2 = " --passes=2 --kf-max-dist=1 --psnr --cq-level=";
    const std::string frames = "1"; // Number of frames to be coded

    // Determine the number of video sequences.
    const int number_of_ss = my_sizeof(files)/my_sizeof(files[0]);

    // Prepare the file where the results are going to be written.
    std::ofstream fileout(filename);
    fileout << std::endl << std::endl << "@@@@@@@@@@@@@@@@@@ Executable used : " << exebase;
    fileout << "  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

    // Loop for every video sequence.
    for(int s=0; s<number_of_ss; s++)
    {
        std::cout << files[s].name << std::endl;
        fileout << std::endl << files[s].name <<" \t FramesToBeEncoded= " << frames << " " << std::endl ;
        
        // Loop for every quality parameter.
        for(int q=0; q < 4; q++)
        {
            // Prepare the command to be pushed into VP9 encoder binary and other variables. 
            std::string cmd = exebase + qps[q] + ext + files[s].fps + args1 + frames + 
                        args2 + qps[q] + " "+ dir + files[s].name + " " + files[s].dims;
            std::string data;
            FILE * stream;
            FILE * stream_dec;
            char buffer[max_buffer];

            // Record encoder start time.
            auto t1 = Clock::now();

            // Try to access the encoder binary and push the command.
            try
            {
                cmd.append(" 2>&1");
                stream = popen(cmd.c_str(), "r");
            }
            catch(const std::exception& e)
            {            
                std::cout << "ERROR: VP9 BINARY " << binary << " COULDN'T BE ACCESSED" << std::endl;
            }

            // Buffer the output.
            if (stream) 
            {
                while (!feof(stream))
                {
                    if (fgets(buffer, max_buffer, stream) != NULL)
                    {
                        data.append(buffer);
                    }
                }
                pclose(stream);
            }

            // Record encoder stop time.
            auto t2 = Clock::now();
        
            // Clean whitespace and prepare variables.
            std::vector<std::string> results;
            boost::split(results, data,[](char c){return isspace(c);});
            int base = 0;
            std::string bitrate, bitrate_kbps, timeDiffStr, timeUnit;
            long double br, time_highprecision;

            // Parse the encoder output and extract necessary variables.
            for(int i=0; i<results.size(); i++)
            {
                if(results[i].back()=='s')
                {
                    bitrate = results[i];
                    bitrate.pop_back();
                    if(bitrate.back()=='/')
                    {
                        bitrate_kbps = bitrate;                   
                        bitrate_kbps.pop_back();
                        bitrate_kbps.pop_back();                       
                    }
                    else if(bitrate.back()=='m' || bitrate.back()=='u')
                    {
                        timeDiffStr = results[i-1];
                        timeUnit = results[i];
                    }
                }
                else if(results[i]=="Stream")
                {
                    base = i;
                    break;
                }
            }
            
            // Try to extract the encoding time information.
            try
            {
                br = ((long double)stol(bitrate_kbps))/1000.0;
                if(timeUnit == "us")
                {
                    time_highprecision = ((long double)stol(timeDiffStr))/1000000.0;
                }
                else
                {
                    time_highprecision = ((long double)stol(timeDiffStr))/1000.0;
                }
            }
            catch(const std::exception& e)
            {            
                std::cout << "ERROR: VP9 OUTPUT OF BINARY " << binary << " COULDN'T BE PARSED" << std::endl;
            }

            // Calculate encoding time
            long timeDiff_i = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            double timeDiff = ((double)timeDiff_i)/1000000000.0;   

            // Prepare variables for the decoder. 
            std::string cmd_dec = "./vpxdec_" + binary + " --progress --codec=vp9 -o dec.y4m ";
            cmd_dec = cmd_dec + "vp9_enc_bitstream_qp_" + qps[q] + ".webm";
                        
            // Record decoder start time.
            auto t3 = Clock::now();

            // Try to access the decoder binary and push the command.
            stream_dec = popen(cmd_dec.c_str(), "r");

            // Buffer the output.
            if (stream_dec) 
            {
                while (!feof(stream_dec))
                {
                    if (fgets(buffer, max_buffer, stream_dec) != NULL)
                    {
                        data.append(buffer);
                    }
                }
                pclose(stream_dec);
            }

            // Record decoder stop time.
            auto t4 = Clock::now();

            // Calculate decoding time
            long dec_timeDiff_i = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();
            double dec_timeDiff = ((double)dec_timeDiff_i)/1000000000.0;
            
            // Print and log the output.
            std::cout << "QP=" << qps[q] << std::fixed << std::setprecision(3) <<" BR=" << br << " PSNR=" << results[base+4];
            std::cout << " Y=" << results[base+6] << " U=" << results[base+7] << " V=" << results[base+8] << " Enc_t_hp=";
            std::cout << time_highprecision << " Enc_t_lp=" << timeDiff << " dec_t=" << dec_timeDiff << std::endl;
            fileout << "QP= " << qps[q] << "  " << results[base+4] << "  " << std::fixed << std::setprecision(3) << br << "   \t ";
            fileout << results[base+6] << " " << results[base+7] << " " << results[base+8] << "   ( Enctime: " << time_highprecision;
            fileout << " s Dectime: " << dec_timeDiff << " s  DecodingOK: YES ) " << std::endl;          
        }
    }

    fileout.close();
    std::cout << std::endl;
    return exp1;
}