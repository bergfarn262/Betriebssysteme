#include <iostream>
#include <cmath>
#include <cstring>

#define RESERVED 'R'
#define CORRUPTED 'D'
#define FREE '0'
#define OCCUPIED '1'

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
    unsigned int numberOfFiles;
    unsigned int blockSize;
    unsigned int fatSize;
};

int tstBit(const char* array, int bitToTest){
    return (array[bitToTest/8] & (int) pow(2, (bitToTest % 8) - 1)) ? 1 : 0;
}

void getAttributes(const char* array) {
    tstBit(array, 1) ? std::cout << "This file is executable." << std::endl : std::cout << "This file cannot be executed." << std::endl;
    tstBit(array, 2) ? std::cout << "This file is read only." << std::endl : std::cout << "This file can be edited." << std::endl;
    tstBit(array, 3) ? std::cout << "This file is visible." << std::endl : std::cout << "This file is hidden."<< std::endl;
}

bool checkConvention(const char* name, const unsigned int length){
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

void showFat (BsFat* pFat) {
    std::cout << "|";
    for (int i = 0; i < (pFat->fatSize / pFat->blockSize); i++) {
        std::cout << pFat->status[i]  << "|";
    }
    std::cout << std::endl;
}

int getFreeDiskSpace(BsFat* bsFat){
    int freeCounter = 0;
    for (int i = 0; i < (bsFat->fatSize / bsFat->blockSize); i++) {
        if(bsFat->status[i] == FREE){
            freeCounter++;
        }
    }
    return freeCounter;
}

void printSizeAndIndexesOfFile(BsFat* bsFat){
    for (int i = 0; i < bsFat->numberOfFiles; i++) {
        if(bsFat->files[i] != nullptr) {
            BsCluster *bsCluster = bsFat->files[i]->cluster;
            std::cout << "File name: " << bsFat->files[i]->name << "; Groesse File: " << bsFat->files[i]->size << "Mb;  "; // nur zur Ausgabe
            while (bsCluster != nullptr) {
                std::cout << bsCluster->positionInStatusArray << ", "; // nur zur Ausgabe
                bsCluster = bsCluster->nextElement;
            }
            std::cout << std::endl; // nur zur Ausgabe
        } else {
            continue;
        }
    }
}

void createFile(BsFat* bsFat, int sizeFile, char* filename, unsigned char attribute, int position, int length){
    int numberOfBlocks = ceil((double)sizeFile / (double) bsFat->blockSize);
    bsFat->files[position] = nullptr;

    if(checkConvention(filename, length) && numberOfBlocks <= getFreeDiskSpace(bsFat)){
        bsFat->files[position] = new BsFile;
        bsFat->files[position]->name = new char[length];
        bsFat->files[position]->attributes = new unsigned char [1];

        bsFat->files[position]->name = filename;
        bsFat->files[position]->attributes[0] = attribute;
        bsFat->files[position]->size = sizeFile;

        BsCluster** bsClusters = new BsCluster*[numberOfBlocks];

        for (int i = 0; i < numberOfBlocks; i++) {
            bsClusters[i] = new BsCluster;
        }

        bsClusters[0]->preElement = nullptr;
        bsClusters[numberOfBlocks - 1]->nextElement = nullptr;
        bsFat->files[position]->cluster = bsClusters[0];

        if(numberOfBlocks > 2){
            for (int i = 1; i < numberOfBlocks - 1; i++) {
                bsClusters[i-1]->nextElement = bsClusters[i];
                bsClusters[i]->preElement = bsClusters[i-1];
                bsClusters[i]->nextElement = bsClusters[i+1];
                bsClusters[i+1]->preElement = bsClusters[i];
            }

            bsClusters[numberOfBlocks - 1]->preElement = bsClusters[numberOfBlocks - 2];

        }else if (numberOfBlocks == 2){
            bsClusters[1]->preElement = bsClusters[0];
            bsClusters[0]->nextElement = bsClusters[1];
        }

        unsigned int pos;

        for (int i = 0; i < numberOfBlocks; i++) {
            pos = (rand() % (bsFat->fatSize / bsFat->blockSize));
            while(bsFat->status[pos] != FREE){
                pos = (rand() % (bsFat->fatSize / bsFat->blockSize));
            }
            bsClusters[i]->positionInStatusArray = pos;
            bsFat->status[pos] = OCCUPIED;
        }
    } else{
        std::cerr << "Die File konnte nicht erstellt werden, weil der Name nicht der Konvention entspricht (" << filename
        << ") oder weil die File zu gross ist: (numberOfBlocksNeeded: "
        << numberOfBlocks
        << ", freeBlocks: "
        << getFreeDiskSpace(bsFat) << ")!"
        << std::endl;
    }

}

void createFiles(BsFat* bsFat){
    int length = 11;
    char** fileNames = new char*[bsFat->numberOfFiles] {"programm1.c", "2programm.c", "prog3.c.cpp", "p4rogramm1.c", "program5m.c", "pr6ogramm.c", "progra7mm.c", "programm8.c", "progr9amm.c", "program.cpp"};

    for (int i = 0; i < bsFat->numberOfFiles; i++) {
        createFile(bsFat, (unsigned int) (rand() % 2100 + 1), fileNames[i], (unsigned char) (rand() % 253 + 1), i, length);
    }
}

BsFat* createBsFat(unsigned int _blockSize, unsigned int _fatSize){
    BsFat* bsFat = new BsFat;
    bsFat->numberOfFiles = 10;
    bsFat->files = new BsFile*[bsFat->numberOfFiles];
    bsFat->blockSize = _blockSize;
    bsFat->fatSize = _fatSize;
    bsFat->status = new unsigned char[_fatSize / _blockSize];

    for (int i = 0; i < _fatSize / _blockSize; i++) {
        bsFat->status[i] = FREE;
    }

    bsFat->status[3] = CORRUPTED;
    bsFat->status[8] = RESERVED;
    bsFat->status[11] = CORRUPTED;
    bsFat->status[10] = CORRUPTED;
    bsFat->status[21] = CORRUPTED;

    showFat(bsFat);

    createFiles(bsFat);

    showFat(bsFat);

    return bsFat;
}

void deleteFile (BsFat* pFat, char* fileName) {
    for (int i = 0; i < pFat->numberOfFiles; i++) {
        if(pFat->files[i] != nullptr) {
            if (strcmp(pFat->files[i]->name, fileName) == 0) {
                BsFile *fileToDelete = pFat->files[i];
                pFat->files[i] = nullptr;

                BsCluster *currentCluster = fileToDelete->cluster;

                while (currentCluster != nullptr) {
                    pFat->status[currentCluster->positionInStatusArray] = FREE;
                    BsCluster *nextCluster = currentCluster->nextElement;
                    delete currentCluster;
                    currentCluster = nextCluster;
                }
                delete fileToDelete;
                std::cout << "File deleted: " << fileName << std::endl;
            }
        }
    }
}

float getFragmentation(BsFat* pFat) {
    float sumOfFrag = 0.0;
    for (int i = 0; i < pFat->numberOfFiles; i++) {
        if (pFat->files[i] == nullptr) {
            continue;
        }

        BsCluster* currentCluster = pFat->files[i]->cluster;

        int minIndex = currentCluster->positionInStatusArray;
        int maxIndex = currentCluster->positionInStatusArray;
        int numberOfBlocks = 0;

        while(currentCluster != nullptr) {
            if (currentCluster->positionInStatusArray > maxIndex) {
                maxIndex = currentCluster->positionInStatusArray;
            } else if (currentCluster->positionInStatusArray < minIndex) {
                minIndex = currentCluster->positionInStatusArray;
            }
            currentCluster = currentCluster->nextElement;
            numberOfBlocks++;
        }
        int length = maxIndex - minIndex + 1;
        sumOfFrag += (float)(length - numberOfBlocks) / length;
    }
    return  sumOfFrag / pFat->numberOfFiles;
}

void defragDisk(struct BsFat* pFat){
    unsigned int counter = 0;

    for (int i = 0; i < pFat->numberOfFiles; i++) {
        if(pFat->files[i] != nullptr){
            std::cout << "Beginne Degragmentierung von Datei: " << pFat->files[i]->name << std::endl;
            BsCluster* current = pFat->files[i]->cluster;

            while (current != nullptr) {
                if (pFat->status[counter] == FREE) {
                    pFat->status[counter] = RESERVED;
                    pFat->status[current->positionInStatusArray] = FREE;
                    current->positionInStatusArray = counter;
                    pFat->status[counter] = OCCUPIED;
                } else {

                    while (pFat->status[counter] == RESERVED || pFat->status[counter] == CORRUPTED) {
                        counter++;
                    }

                    if(pFat->status[counter] == FREE){
                        pFat->status[counter] = RESERVED;
                        pFat->status[current->positionInStatusArray] = FREE;
                        current->positionInStatusArray = counter;
                        pFat->status[counter] = OCCUPIED;
                    }

                    for (int j = 0; j < pFat->numberOfFiles; j++) {
                        if (pFat->files[j] != nullptr) {
                            BsCluster *secondCurrent = pFat->files[j]->cluster;

                            while (secondCurrent != nullptr) {
                                if (secondCurrent->positionInStatusArray == counter) {
                                    unsigned int saver = current->positionInStatusArray;
                                    current->positionInStatusArray = secondCurrent->positionInStatusArray;
                                    secondCurrent->positionInStatusArray = saver;
                                    break;
                                }
                                secondCurrent = secondCurrent->nextElement;
                            }
                        }
                    }
                }
                counter++;
                current = current->nextElement;
            }
            std::cout << "Beende Defragmentierung von Datei: " << pFat->files[i]->name << std::endl;
            std::cout << "Grad der Fragmentierung betraegt nun: " << getFragmentation(pFat) * 100 << "%\n"  << std::endl;
        }
    }

}


/**
 *  PROTOTYP
 **/
int getFreeConnectedBlocks(BsFat* pFat, int startIndex) {
    int counter = 0;
    int index = startIndex;
    unsigned char* statusArray = pFat->status;
    while (statusArray[index] == FREE) {
        counter++;
    }
    return counter;
}

int getIndexByFileName(BsFat* pFat, char* filename) {
    for (int i = 0; i < pFat->numberOfFiles; i++) {
        if (pFat->files[i]->name == filename) {
            return i;
        }
    }
    return -1;
}

void insertFile(BsFat* pFat, int index, char* filename) {
    BsFile* file = pFat->files[getIndexByFileName(pFat, filename)];
    BsCluster* currentCluster = file->cluster;
    unsigned char* status = pFat->status;
    while (currentCluster != nullptr) {
        if (status[index] == FREE) {
            status[index] = OCCUPIED;
            file->cluster->positionInStatusArray = index;
            index++;
            currentCluster = currentCluster->nextElement;
        } else {
            index++;
        }
    }
}

int* createHelpArray (BsFat* pFat) {
    int* helpArray = new int[pFat->numberOfFiles];
    for (int i = 0; i < pFat->numberOfFiles; i++) {
        if (pFat->files[i] != nullptr) {
            helpArray[i] = 1;
        } else {
            helpArray[i] = 0;
        }
    }
    return helpArray;
}

void defragDiskPrototype(struct BsFat* pFat){
    unsigned char* status = pFat->status;
    for (int i = 0; i < (pFat->fatSize / pFat->blockSize); i++) {
        if (status[i] == OCCUPIED) {
            status[i] == FREE;
        }
    }
    int pointer = 0;
    int gap = -1;
    do {
        gap = getFreeConnectedBlocks(pFat, pointer);
    } while (gap == 0);
    int* helpArray = createHelpArray(pFat);
    while (pointer < (pFat->fatSize / pFat->blockSize)){
        bool hasFit = false;
        for (int j = 0; j < pFat->numberOfFiles; j++) {
            if (helpArray[j] == 1) {
                int blocksInFile = ceil((double)pFat->files[j]->size / (double) pFat->blockSize);
                if (blocksInFile == gap) {
                    insertFile(pFat, pointer, pFat->files[j]->name);
                    helpArray[j] = 0;
                    hasFit = true;
                    pointer += blocksInFile;
                    gap = getFreeConnectedBlocks(pFat, pointer);
                }
            }
        }
        if (!hasFit) {
            int indexOfNextSmallerFile = -1;
            int blocksOfNextSmallerFile = -1;
            for (int j = 0; j < pFat->numberOfFiles; j++) {
                if (helpArray[j] == 1) {
                    int blocksInFile = ceil((double)pFat->files[j]->size / (double) pFat->blockSize);
                    if (blocksInFile < gap && blocksInFile > blocksOfNextSmallerFile) {
                        indexOfNextSmallerFile = j;
                        blocksOfNextSmallerFile = blocksInFile;
                    }
                }
            }
            if (blocksOfNextSmallerFile > 0) {
                insertFile(pFat, pointer, pFat->files[indexOfNextSmallerFile]->name);
                helpArray[indexOfNextSmallerFile] = 0;
                hasFit = true;
                pointer += blocksOfNextSmallerFile;
                gap = getFreeConnectedBlocks(pFat, pointer);
            } else {
                gap += getFreeConnectedBlocks(pFat, (pointer + gap + 1));
            }
        }
    }
}
/** **/


int main() {
    BsFat* bsFat = createBsFat(512, 16384);
    char fileToSearch[] = "programm1.c";

    printSizeAndIndexesOfFile(bsFat);

    deleteFile(bsFat, fileToSearch);

    showFat(bsFat);

    float frag = getFragmentation(bsFat) * 100;

    std::cout << "Die Fragmentierung der Platte betraegt: " << frag << "%" <<  std::endl;

    defragDisk(bsFat);

    showFat(bsFat);

    frag = getFragmentation(bsFat) * 100;

    std::cout << "Die Fragmentierung der Platte betraegt: " << frag << "%" <<  std::endl;

    printSizeAndIndexesOfFile(bsFat);
}
