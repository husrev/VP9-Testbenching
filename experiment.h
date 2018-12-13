#ifndef VP9_FINETUNING
#define VP9_FINETUNING

#include <string>
#include <chrono>

#define my_sizeof(type) ((char *)(&type+1)-(char*)(&type))

class experiment{

    private:
    
        std::string const anglesFile = "coefficients.txt";
        int startAngle = 0;
        double btf_angles[16] =
        {
            1.383628275538983, 1.205684375253760, 1.036618661005613, 0.871580223347819,
            0.818272929312937, 0.796932577905346, 1.003242837154322, 0.838776894108840,
            0.294777910775701, 0.943742188732190, 0.790680487520078, 0.989849276150831,
            0.840309847166370, 0.873478990393634, 0.614812128569093, 0.615277084212252
        };
        std::string qps[4] = {"45","46","47","48"};

    public:

        experiment(bool resume);
        std::string process(std::string binary, std::string exp1, std::string dir);
        long double getBDBR(std::string baseline, std::string exp1);
        bool writeAngles();
        double getAngle(int angle){return btf_angles[angle];}
        int getStartAngle(){return startAngle;}
        void setAngle(int angle, double value) {btf_angles[angle] = value;}
};

#endif
