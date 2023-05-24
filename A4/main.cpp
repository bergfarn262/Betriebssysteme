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
                if(getFreeDiskSpace(bsFat) == 0){
                    std::cout << "Fehler! Kein Speicher frei!" << std::endl;
                    break;
                }
                pos = (rand() % (bsFat->fatSize / bsFat->blockSize));
            }
            bsClusters[i]->positionInStatusArray = pos;
            bsFat->status[pos] = OCCUPIED;
            std::cout << pos << ";"; // nur zur Ausgabe
        }
        std::cout << std::endl; // nur zur Ausgabe
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
        std::cout << (float)(length - numberOfBlocks) / length << std::endl; // nur zur Ausgabe
        sumOfFrag += (float)(length - numberOfBlocks) / length;
    }
    return  sumOfFrag / pFat->numberOfFiles;
}

/**
 * Algorithmus:
 * berechne größe Lücke
 * berechne größe File (Blöcke)
 * 1. Fall: File passt perfekt -> File reinschreiben
 * 2. Fall: keine File passt (alle zu klein) -> nächst größere File reinschreiben
 * 3. Fall: keine File Passt (alle zu groß) -> check nächste Lücke und schreib passende rein (mit der Lücke) oder nächst größere wenn wieder nicht passt
 *
 */

/*void defragDisk(struct BsFat* pFat){
    for (int i = 0; i < pFat->fatSize / pFat->blockSize; i++) {
        if(pFat->status[i] == OCCUPIED){
            pFat->status[i] = FREE;
        }
    }

    unsigned int counter = 0;

    for (int i = 0; i < pFat->numberOfFiles; i++) {
        if(pFat->files[i] == nullptr){
            continue;
        }

        std::cout << "Defragmentierung der " << i+1 << ". File." << std::endl;

        BsCluster* current = pFat->files[i]->cluster;

        while(current != nullptr){
            if(pFat->status[counter] == FREE){
                current->positionInStatusArray = counter;
            } else{
                do{
                    counter++;
                }while(pFat->status[counter] != FREE);
                current->positionInStatusArray = counter;
            }
            pFat->status[counter] = OCCUPIED;
            counter++;
            current = current->nextElement;
        }
        std::cout << "Defragmentierung der " << i+1 << ". File abgeschlossen!" << std::endl;
    }
}*/

void defragDisk(struct BsFat* pFat){
    unsigned int counter = 0;

    for (int i = 0; i < pFat->numberOfFiles; i++) {
        if(pFat->files[i] != nullptr){
            BsCluster* current = pFat->files[i]->cluster;

            while (current != nullptr) {
                if (pFat->status[counter] == FREE) {
                    pFat->status[current->positionInStatusArray] = FREE;
                    current->positionInStatusArray = counter;
                    pFat->status[counter] = OCCUPIED;
                } else {

                    while (pFat->status[counter] == RESERVED || pFat->status[counter] == CORRUPTED) {
                        counter++;
                    }

                    if(pFat->status[counter] == FREE){
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
        }
    }

}

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