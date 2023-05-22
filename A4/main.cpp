#include <iostream>
#include <cmath>

#define RESERVED 'R'
#define CORRUPTED 'D'
#define FREE '1'
#define OCCUPIED '0'


struct BsCluster{
    BsCluster* preElement;
    unsigned int positionInStatusArray; //hat er nicht explicit gesagt, aber denke das wird nötig sein
    BsCluster* nextElement;
};

struct BsFile{
    char* name;
    unsigned char* attributes;
    unsigned int size;
    BsCluster* cluster;
};

struct BsFat{
    unsigned char* status;
    BsFile** files;
    unsigned int blockSize;
    unsigned int fatSize;
};

bool checkConvention(const char* name, const unsigned int length){
    int alphaCounter = 0;
    int dot = 0;
    for (int i = 0; i < length; i++) {
        if(i == 0 && !std::isalnum(name[i])){
            return false;
        }
        if(dot != 0 && !std::isalpha(name[i])){
            if(name[i] != '.'){
                return false;
            }
        }
        if(name[i] == '.'){
            dot++;
        }
    }
    return true;
}

void createFile(BsFat* bsFat, int sizeFile, char* filename, unsigned char attribute, int position, int length){
    bsFat->files[position] = new BsFile;
    bsFat->files[position]->name = new char[length];
    bsFat->files[position]->attributes = new unsigned char [1];

    if(checkConvention(filename, length)){
        bsFat->files[position]->name = filename;
        bsFat->files[position]->attributes[0] = attribute;
        bsFat->files[position]->size = sizeFile;
    }
     int numberOfBlocks = ceil((double)sizeFile / (double) bsFat->blockSize + 1);

    BsCluster** bsClusters = new BsCluster*[numberOfBlocks];

    for (int i = 0; i < numberOfBlocks; i++) {
        bsClusters[i] = new BsCluster;
    }
    bsClusters[0]->preElement = nullptr;
    bsClusters[numberOfBlocks - 1]->nextElement = nullptr;

    if(numberOfBlocks > 2){
        for (int i = 1; i < numberOfBlocks - 1; i++) {
            bsClusters[i]->preElement = bsClusters[i-1];
            bsClusters[i]->nextElement = bsClusters[i+1];
        }

        bsFat->files[position]->cluster = bsClusters[0];
        bsClusters[numberOfBlocks - 1]->preElement = bsClusters[numberOfBlocks - 2];

    }else if(numberOfBlocks == 1){

        bsFat->files[position]->cluster = bsClusters[0];

    }else{
        bsFat->files[position]->cluster = bsClusters[0];
        bsClusters[0]->nextElement = bsClusters[1];
        bsClusters[1]->preElement = bsClusters[0];
    }

    unsigned int pos = 0;

    for (int i = 0; i < numberOfBlocks; i++) {
        pos = (rand() % (bsFat->fatSize / bsFat->blockSize));
        while(bsFat->status[pos] != FREE){
            pos = (rand() % (bsFat->fatSize / bsFat->blockSize));
        }
        bsClusters[i]->positionInStatusArray = pos;
        bsFat->status[i] = OCCUPIED;
    }
}

void createFiles(BsFat* bsFat){
    int length = 11;
    char** fileNames = new char*[10] {"programm1.c", "2programm.c", "prog3.c.cpp", "p4rogramm1.c", "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};

    for (int i = 0; i < 10; i++) {
        createFile(bsFat, (unsigned int) (rand() % 1700 + 1), fileNames[i], (unsigned char) (rand() % 253 + 1), i, length);
    }
}

BsFat* createBsFat(unsigned int _blockSize, unsigned int _fatSize){
    BsFat* bsFat = new BsFat;
    bsFat->files = new BsFile*[10];
    bsFat->blockSize = _blockSize;
    bsFat->fatSize = _fatSize;
    bsFat->status = new unsigned char[_fatSize / _blockSize];

    for (int i = 0; i < _fatSize / _blockSize; ++i) {
        bsFat->status[i] = FREE;
    }

    bsFat->status[3] = CORRUPTED;
    bsFat->status[8] = RESERVED;
    bsFat->status[11] = CORRUPTED;
    bsFat->status[10] = CORRUPTED;
    bsFat->status[21] = CORRUPTED;

    for (int i = 0; i < _fatSize / _blockSize; ++i) {
        std::cout << bsFat->status[i] << ", ";
    }

    std::cout << std::endl;

    createFiles(bsFat);

    for (int i = 0; i < _fatSize / _blockSize; ++i) {
        std::cout << bsFat->status[i] << ", ";
    }

    return bsFat;
}

int main() {
    createBsFat(512, 16384);
}