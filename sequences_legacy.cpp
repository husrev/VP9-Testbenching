#include "sequences.h"

size_t trainSize = 3;
struct vseq trainSeqs[] = {
    // {"Beauty_3840x2160.yuv","--width=3840 --height=2160","120"}, // 4K Videos
    // {"Bosphorus_3840x2160.yuv","--width=3840 --height=2160","120"},  
    // {"HoneyBee_3840x2160.yuv","--width=3840 --height=2160","120"},
    // {"Jockey_3840x2160.yuv","--width=3840 --height=2160","120"},
    // {"ReadySteadyGo_3840x2160.yuv","--width=3840 --height=2160","120"},
    // {"ShakeNDry_3840x2160.yuv","--width=3840 --height=2160","120"},
    // {"YachtRide_3840x2160.yuv","--width=3840 --height=2160","120"}
    //   {"Traffic_2560x1600_30_crop.yuv","--width=2560 --height=1600","30"}, // Class A 2560x1600 :
    //   {"PeopleOnStreet_2560x1600_30_crop.yuv","--width=2560 --height=1600","30"}
    //    {"Kimono1_1920x1080_24.yuv","--width=1920 --height=1080","24"}, // Class B  1920x1080 : 
    //    {"ParkScene_1920x1080_24.yuv","--width=1920 --height=1080","24"},
    //    {"Cactus_1920x1080_50.yuv","--width=1920 --height=1080","50"},
    //    {"BQTerrace_1920x1080_60.yuv","--width=1920 --height=1080","60"},
    //   {"BasketballDrive_1920x1080_50.yuv","--width=1920 --height=1080","50"}
    //    {"RaceHorses_832x480_30.yuv","--width=832 --height=480","30"}, // Class C 832x480 :  
    //  {"BQMall_832x480_60.yuv","--width=832 --height=480","60"},
    //  {"PartyScene_832x480_50.yuv","--width=832 --height=480","50"},
    //  {"BasketballDrillText_832x480_50.yuv","--width=832 --height=480","50"},
    //      {"RaceHorses_416x240_30.yuv","--width=416 --height=240","30"}, // Class D 416x240 :
    //      {"BQSquare_416x240_60.yuv","--width=416 --height=240","60"},
    //    {"BlowingBubbles_416x240_50.yuv","--width=416 --height=240","50"},
    //    {"BasketballPass_416x240_50.yuv","--width=416 --height=240","50"}
       {"FourPeople_1280x720_60.yuv","--width=1280 --height=720","60"}, // Class E 1280x720 :
     {"Johnny_1280x720_60.yuv","--width=1280 --height=720","60"},
     {"KristenAndSara_1280x720_60.yuv","--width=1280 --height=720","60"}//
    };

size_t testSize = 22;
struct vseq testSeqs[] = {
    {"Beauty_3840x2160.yuv","--width=3840 --height=2160","120"}, // 4K Videos
    {"Bosphorus_3840x2160.yuv","--width=3840 --height=2160","120"},  
    {"HoneyBee_3840x2160.yuv","--width=3840 --height=2160","120"},
    {"Jockey_3840x2160.yuv","--width=3840 --height=2160","120"},
    {"ReadySteadyGo_3840x2160.yuv","--width=3840 --height=2160","120"},
    {"ShakeNDry_3840x2160.yuv","--width=3840 --height=2160","120"},
    {"YachtRide_3840x2160.yuv","--width=3840 --height=2160","120"},
    {"Traffic_2560x1600_30_crop.yuv","--width=2560 --height=1600","30"}, // Class A 2560x1600 :
    {"PeopleOnStreet_2560x1600_30_crop.yuv","--width=2560 --height=1600","30"},
    {"Kimono1_1920x1080_24.yuv","--width=1920 --height=1080","24"}, // Class B  1920x1080 : 
    {"ParkScene_1920x1080_24.yuv","--width=1920 --height=1080","24"},
    {"Cactus_1920x1080_50.yuv","--width=1920 --height=1080","50"},
    {"BQTerrace_1920x1080_60.yuv","--width=1920 --height=1080","60"},
    {"BasketballDrive_1920x1080_50.yuv","--width=1920 --height=1080","50"},
    {"RaceHorses_832x480_30.yuv","--width=832 --height=480","30"}, // Class C 832x480 :  
    {"BQMall_832x480_60.yuv","--width=832 --height=480","60"},
    {"PartyScene_832x480_50.yuv","--width=832 --height=480","50"},
    {"BasketballDrillText_832x480_50.yuv","--width=832 --height=480","50"},
    {"RaceHorses_416x240_30.yuv","--width=416 --height=240","30"}, // Class D 416x240 :
    {"BQSquare_416x240_60.yuv","--width=416 --height=240","60"},
    {"BlowingBubbles_416x240_50.yuv","--width=416 --height=240","50"},
    {"BasketballPass_416x240_50.yuv","--width=416 --height=240","50"}
    // {"FourPeople_1280x720_60.yuv","--width=1280 --height=720","60"}, // Class E 1280x720 :
    // {"Johnny_1280x720_60.yuv","--width=1280 --height=720","60"},
    // {"KristenAndSara_1280x720_60.yuv","--width=1280 --height=720","60"}
};