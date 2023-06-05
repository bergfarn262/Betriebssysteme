#include <iostream>
#include <cmath>

void setBit(char* array, int bitToSet){
    array[bitToSet/8] = array[bitToSet/8] | ((int) pow(2, (bitToSet % 8) - 1));
}

void clrBit(char* array, int bitToClear){
    array[bitToClear/8] = array[bitToClear/8] & ~((int) pow(2, (bitToClear % 8) - 1));
}

int tstBit(const char* array, int bitToTest){
    return (array[bitToTest/8] & (int) pow(2, (bitToTest % 8) - 1)) ? 1 : 0;
}

int getNumberParityBits(int numberDataBits){
    int counter = 1;
    while(1){
        if((int) pow(2, counter) > numberDataBits){
            return counter;
        }
        counter++;
    }
}

int sizeOfHammingArrayInBytes(int numberDataBits){
    int sumBits = numberDataBits + getNumberParityBits(numberDataBits);
    return (sumBits % 8 == 0) ? (sumBits / 8) : (sumBits / 8) + 1;
}

char* initHammingArray(int numberDataBits){
    char* hammingArray = new char[sizeOfHammingArrayInBytes(numberDataBits)];
    return hammingArray;
}

char* copyArrayToHammingArray(const char* byteArrayData, int numberDataBits, char * hammingArray){
    int counter = 0;
    for (int i = 1; i <= (numberDataBits + getNumberParityBits(numberDataBits)); i++) {
        if((int) pow(2, counter) == i){
            counter++;
            clrBit(hammingArray, i);
        } else{
            tstBit(byteArrayData,i - counter) ? setBit(hammingArray,i) : clrBit(hammingArray,i);
        }
    }
    return hammingArray;
}

int calculateParityBitOnHamming(int parityBit, const char* hammingArray, int numberOfBits){
    int counter = 0;
    for (int i = parityBit; i <= numberOfBits ; i += parityBit * 2) {
        for (int j = i; j <= parityBit + i - 1 && j <= numberOfBits ; j++) {
            if(tstBit(hammingArray,j) == 1 && j != parityBit){
                counter++;
            }
        }
    }
    return counter % 2 == 0 ? 0 : 1;
}

void showArray(const char* byteArray, int numberBits){
    for (int i = 1; i <= numberBits; i++) {
        (i % 8 == 0) ? (std::cout << tstBit(byteArray, i) << " ")
                     : (std::cout << tstBit(byteArray, i));
    }
    std::cout << std::endl;
}

int computeHammingCode(const char* byteArrayData, int numberDataBits, char * hammingArray){
    char* copiedHamming = copyArrayToHammingArray(byteArrayData, numberDataBits, hammingArray);

    //showArray(hammingArray, numberDataBits + getNumberParityBits(numberDataBits));

    int* parityBits = new int[getNumberParityBits(numberDataBits)];
    for (int i = 0; i < getNumberParityBits(numberDataBits) ; i++) {
        //std::cout << calculateParityBitOnHamming((int) pow(2, i),copiedHamming,getNumberParityBits(numberDataBits) + numberDataBits) << std::endl;
        if(calculateParityBitOnHamming((int) pow(2, i),copiedHamming,getNumberParityBits(numberDataBits) + numberDataBits) == 0){
            clrBit(copiedHamming, (int) pow(2,i));
        } else{
            setBit(copiedHamming, (int) pow(2,i));
        }
    }
    //showArray(hammingArray, numberDataBits + getNumberParityBits(numberDataBits));
    return 1;
}

bool isParityBit(int pos){
    if(pos == 0){
        return false;
    }
    return ((pos & (pos-1)) == 0);
}

void showParityBits(const char* byteArray, int numberBits){
    for (int i = 1; i <= numberBits; i++) {
        if(isParityBit(i)){
            std::cout << tstBit(byteArray, i);
        }
    }
    std::cout << std::endl;
}

void showDataBits(const char* byteArray, int numberBits){
    for (int i = 1; i <= numberBits; i++) {
        if(isParityBit(i) == 0){
            std::cout << tstBit(byteArray, i);
        }
    }
    std::cout << std::endl;
}

int checkAndCorrectHammingCode(char* hammingArray, int numberBits){
    int counter = 0;
    int errorBit = 0;
    for (int i = 1; i <= numberBits; i++) {
        if(isParityBit(i)){
            if((calculateParityBitOnHamming(i, hammingArray, numberBits) != tstBit(hammingArray,i))){
                std::cout << "i: " << i << std::endl;
                errorBit+= i;
            }
        }
    }
    if (errorBit < numberBits) {
        if (tstBit(hammingArray, errorBit)) {
            clrBit(hammingArray, errorBit);
        } else {
            setBit(hammingArray, errorBit);
        }
        std::cout << "Es wurden versucht den Fehler zu beheben. Es ist trotzdem moeglich, dass das Ergebnis falsch ist. " << std::endl;
    } else {
        std::cout << "Es wurden zu viele Bits geflippt, um den Fehler zu beheben." << std::endl;
    }
    return errorBit;
}

int main() {
    char array[] = {86, 'a', 'l', 'l', 'o'};

    int length = 8;
    int lengthHamming = length + getNumberParityBits(length);

    char * hammingArray = initHammingArray(length);
    computeHammingCode(array, length, hammingArray);

    showArray(array, length);
    showArray(hammingArray, lengthHamming);
    showParityBits(hammingArray, lengthHamming);

    setBit(hammingArray, 7);

    showArray(hammingArray, lengthHamming);

    std::cout << checkAndCorrectHammingCode(hammingArray, lengthHamming) << std::endl;

    showArray(hammingArray, lengthHamming);


    return 0;
}
