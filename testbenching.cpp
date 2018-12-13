// Make this file with -std=c++11

// To configure VP9, use the following
// ./sudo make clean
// ./configure --disable-sse --disable-sse2 --disable-sse3 --disable-ssse3 
// --disable-mmx --disable-vp8-encoder --disable-vp8-decoder
// ./sudo make

// An example call 
// ./vpxenc Traffic_2560x1600_30_crop.yuv -o vp9_enc_bitstream.webm --width=2560 --height=1600 --codec=vp9 --fps=30/1 --end-usage=3 --limit=1 --passes=2 --kf-max-dist=1 --psnr --cq-level=28

// When calling this function, DO NOT FORGET TO GAIN ADMINISTRATOR PRIVILEGES (sudo)

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include <chrono>
using namespace std;
#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))
typedef chrono::high_resolution_clock Clock;
string process(string binary, string cmp);
struct vseq
{
    string name;
    string dims;
    string fps;
};

int main()
{
    string compfolder = "Results_Comparisons_TDIP_noscaling/";
    string cmpdir = "../../../HEVC-testseqs/Results_otomatik_script/" + compfolder;
    string experiment = "trained_final";    
    string cmp = cmpdir + experiment;
    string cmpfdir = compfolder + experiment;

 //   popen(("mkdir -p " + cmp).c_str(), "r");
    string baseline = process("orig_unopt",cmp);
    string exp1 = "trained_final"; //process("trained_final",cmp);
//  string exp2 = process("mm2opt10btf",cmp);
//  string exp3 = process("mm2opt16btf",cmp);
//  string exp4 = process("mm2edst",cmp);
//  string exp5 = process("mm2odst",cmp);

    popen(("cd "+cmpdir+"; sudo perl compare_lossy.pl "+cmpfdir+" "+baseline+".txt " +cmpfdir+" "+exp1+".txt 0").c_str(), "r");
    //popen(("cd "+cmpdir+"; sudo perl compare_lossy.pl "+cmpfdir+" "+baseline+".txt " +cmpfdir+" "+exp2+".txt 0").c_str(), "r");
    //popen(("cd "+cmpdir+"; sudo perl compare_lossy.pl "+cmpfdir+" "+baseline+".txt " +cmpfdir+" "+exp3+".txt 0").c_str(), "r");
    //popen(("cd "+cmpdir+"; sudo perl compare_lossy.pl "+cmpfdir+" "+baseline+".txt " +cmpfdir+" "+exp4+".txt 0").c_str(), "r");
    //popen(("cd "+cmpdir+"; sudo perl compare_lossy.pl "+cmpfdir+" "+baseline+".txt " +cmpfdir+" "+exp5+".txt 0").c_str(), "r");

//    popen(("rsync -a "+cmp+" "+"../../../../Dropbox").c_str(), "r");

    return 0;
}

string process(string binary, string cmp)
{
    cout << ">> Processing binary " << binary << endl;
    string filename = binary + ".txt";
    string exebase = "../../vpxenc_" + binary + " -o vp9_enc_bitstream_qp_";
    //string ext = ".webm --codec=vp9 --threads=1 --fps=";
    string ext = ".webm --codec=vp9 --fps=";
    string args1 = "/1 --end-usage=3 --limit=";
    string args2 = " --passes=2 --kf-max-dist=1 --psnr --cq-level=";
    string qps[] = {"28","24","20","16"};
    string dir = "../../origCfP/";
    const string frames = "1"; // Number of frames to be coded
    
    struct vseq files[] = {
 //       {"Beauty_3840x2160.yuv","--width=3840 --height=2160","120"}, // 4K Videos
 //       {"Bosphorus_3840x2160.yuv","--width=3840 --height=2160","120"},  
 //       {"HoneyBee_3840x2160.yuv","--width=3840 --height=2160","120"},
 //       {"Jockey_3840x2160.yuv","--width=3840 --height=2160","120"},
        {"ReadySteadyGo_3840x2160.yuv","--width=3840 --height=2160","120"},
        {"ShakeNDry_3840x2160.yuv","--width=3840 --height=2160","120"},
        {"YachtRide_3840x2160.yuv","--width=3840 --height=2160","120"},
 //       {"Traffic_2560x1600_30_crop.yuv","--width=2560 --height=1600","30"}, // Class A 2560x1600 :
        {"PeopleOnStreet_2560x1600_30_crop.yuv","--width=2560 --height=1600","30"},
////        {"NebutaFestival_2560x1600_60_10bit_crop.yuv","--width=2560 --height=1600","60"},
////        {"SteamLocomotiveTrain_2560x1600_60_10bit_crop.yuv","--width=2560 --height=1600","60"},
//        {"Kimono1_1920x1080_24.yuv","--width=1920 --height=1080","24"}, // Class B  1920x1080 : 
//        {"ParkScene_1920x1080_24.yuv","--width=1920 --height=1080","24"},
//        {"Cactus_1920x1080_50.yuv","--width=1920 --height=1080","50"},
        {"BQTerrace_1920x1080_60.yuv","--width=1920 --height=1080","60"},
        {"BasketballDrive_1920x1080_50.yuv","--width=1920 --height=1080","50"},
//        {"RaceHorses_832x480_30.yuv","--width=832 --height=480","30"}, // Class C 832x480 :  
//       {"BQMall_832x480_60.yuv","--width=832 --height=480","60"},
        {"PartyScene_832x480_50.yuv","--width=832 --height=480","50"},
        {"BasketballDrillText_832x480_50.yuv","--width=832 --height=480","50"},
//        {"RaceHorses_416x240_30.yuv","--width=416 --height=240","30"}, // Class D 416x240 :
//        {"BQSquare_416x240_60.yuv","--width=416 --height=240","60"},
        {"BlowingBubbles_416x240_50.yuv","--width=416 --height=240","50"},
        {"BasketballPass_416x240_50.yuv","--width=416 --height=240","50"},
//        {"FourPeople_1280x720_60.yuv","--width=1280 --height=720","60"}, // Class E 1280x720 :
        {"Johnny_1280x720_60.yuv","--width=1280 --height=720","60"},
        {"KristenAndSara_1280x720_60.yuv","--width=1280 --height=720","60"}//,
////        {"BasketballDrillText_832x480_50.yuv","--width=832 --height=480","50"}, // Class F mixed:
////        {"ChinaSpeed_1024x768_30.yuv","--width=1024 --height=768","30"},
////        {"SlideEditing_1280x720_30.yuv","--width=1280 --height=720","30"},
////        {"SlideShow_1280x720_20.yuv","--width=1280 --height=720","20"}
    };
    
    /*
    struct vseq files[] = {
        {"Bosphorus_3840x2160.yuv","--width=3840 --height=2160","120"}  
    };
    */

    const int number_of_ss = my_sizeof(files)/my_sizeof(files[0]);

    ofstream fileout(filename);

    fileout << endl << endl << "@@@@@@@@@@@@@@@@@@ Executable used : " << exebase;
    fileout << "  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;

    for(int s=0; s<number_of_ss; s++)
    {
        cout << files[s].name << endl;
        fileout << endl << files[s].name <<" \t FramesToBeEncoded= " << frames << " " << endl ;
        for(int q=0; q < 4; q++)
        {
            string cmd = exebase + qps[q] + ext + files[s].fps + args1 + frames + 
                        args2 + qps[q] + " "+ dir + files[s].name + " " + files[s].dims;
            string data;
            FILE * stream;
            FILE * stream_dec;
            const int max_buffer = 4096;
            char buffer[max_buffer];
            auto t1 = Clock::now();
            //cout << cmd << endl; //For debug purposes
            try{
                cmd.append(" 2>&1");
                stream = popen(cmd.c_str(), "r");
            }
            catch(const std::exception& e){            
                cout << "ERROR: VP9 BINARY " << binary << " COULDN'T BE ACCESSED" << endl;
            }

            if (stream) 
            {
                while (!feof(stream))
                    if (fgets(buffer, max_buffer, stream) != NULL) 
                        data.append(buffer);
                pclose(stream);
            }
            auto t2 = Clock::now();
            
            vector<string> results;
            boost::split(results, data,[](char c){return isspace(c);});
            int base = 0;
            string bitrate, bitrate_kbps, timeDiffStr, timeUnit;
            long double br, time_highprecision;
            for(int i=0; i<results.size(); i++)
            {
                //cout << i << "=" << results[i] << endl;
                if(results[i].back()=='s')
                {
                    bitrate = results[i];
                    bitrate.pop_back();
                    if(bitrate.back()=='/')
                    {
                        bitrate_kbps = bitrate;                   
                        bitrate_kbps.pop_back();
                        bitrate_kbps.pop_back();
                        /*
                        cout << "TIMER VALUE:" << results[i+1] << " ";
                        cout << results[i+2] << " " << results[i+3] << " ";
                        cout << results[i+4] << " " << results[i+5] << endl;
                        */
                        
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
            
            try{
                br = ((long double)stol(bitrate_kbps))/1000.0;
                //cout << "FINALTIME=" << timeDiffStr << endl;
                if(timeUnit == "us")
                    time_highprecision = ((long double)stol(timeDiffStr))/1000000.0;
                else
                    time_highprecision = ((long double)stol(timeDiffStr))/1000.0;
            }
            catch(const std::exception& e){            
                cout << "ERROR: VP9 OUTPUT OF BINARY" << binary << " COULDN'T BE PARSED" << endl;
            }


            long timeDiff_i = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
            double timeDiff = ((double)timeDiff_i)/1000000000.0;
            
            string cmd_dec = "../../vpxdec_" + binary + " --progress --codec=vp9 -o dec.y4m ";
            cmd_dec = cmd_dec + "vp9_enc_bitstream_qp_" + qps[q] + ".webm";
            auto t3 = Clock::now();
            stream_dec = popen(cmd_dec.c_str(), "r");
            if (stream_dec) 
            {
                while (!feof(stream_dec))
                    if (fgets(buffer, max_buffer, stream_dec) != NULL) 
                        data.append(buffer);
                pclose(stream_dec);
            }
            auto t4 = Clock::now();

            long dec_timeDiff_i = std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count();
            double dec_timeDiff = ((double)dec_timeDiff_i)/1000000000.0;

            cout << "QP=" << qps[q] << fixed << setprecision(3) <<" BR=" << br << " PSNR=" << results[base+4];
            cout << " Y=" << results[base+6] << " U=" << results[base+7] << " V=" << results[base+8] << " Enc_t_hp=";
            cout << time_highprecision << " Enc_t_lp=" << timeDiff << " dec_t=" << dec_timeDiff << endl;
            fileout << "QP= " << qps[q] << "  " << results[base+4] << "  " << fixed << setprecision(3) << br << "   \t ";
            fileout << results[base+6] << " " << results[base+7] << " " << results[base+8] << "   ( Enctime: " << time_highprecision;
            fileout << " s Dectime: " << dec_timeDiff << " s  DecodingOK: YES ) " << endl;          
        }
    }
    popen(("cp "+binary+ ".txt "+cmp).c_str(), "r");
    cout << endl;
    return binary;
}