#ifndef SEQUENCES
#define SEQUENCES
#include <string>

struct vseq
{
    std::string name;
    std::string dims;
    std::string fps;
};

extern size_t trainSize;
extern size_t testSize;
extern struct vseq trainSeqs[];
extern struct vseq testSeqs[];

#endif