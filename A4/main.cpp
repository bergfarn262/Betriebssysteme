#include <iostream>

struct BsCluster{
    BsCluster* preElement;
    BsCluster* nextElement;
};

struct BsFile{
    unsigned char* name;
    unsigned char* attributes;
    unsigned int size;
    BsCluster cluster;
};

struct BsFat{
    unsigned char* status;
    BsFile** files;
    unsigned int blockSize;
    unsigned int numberOfBlocks;
};

BsFat* createBsFat(unsigned int _blockSize, unsigned int _numberOfBlocks){
    BsFat* bsFat = new BsFat;
    bsFat->blockSize = _blockSize;
    bsFat->numberOfBlocks = _numberOfBlocks;
    bsFat->bsClusters = new BsCluster* [_numberOfBlocks];

    return bsFat;
}

int main() {

}