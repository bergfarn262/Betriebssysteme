#include <iostream>
#include <cmath>

//richtig
void setBit(char* array, int bitToSet){
    array[bitToSet/8] = array[bitToSet/8] | ((int) pow(2, (bitToSet % 8)));
}
//richtig
void clrBit(char* array, int bitToClear){
    array[bitToClear/8] = array[bitToClear/8] & ~((int) pow(2, (bitToClear % 8)));
}
//richtig
int tstBit(const char* array, int bitToTest){
    return (array[bitToTest/8] & (int) pow(2, (bitToTest % 8))) ? 1 : 0;
}
//richtig
int getNumberParityBits(int numberDataBits){
    int counter = 1;
    while(true){
        if((int) pow(2, counter) > numberDataBits){
            return counter;
        }
        counter++;
    }
}
//richtig
int sizeOfHammingArrayInBytes(int numberDataBits){
    int sumBits = numberDataBits + getNumberParityBits(numberDataBits);
    return (sumBits % 8 == 0) ? (sumBits / 8) : (sumBits / 8) + 1;
}
//richtig
char* initHammingArray(int numberDataBits){
    char* hammingArray = new char[sizeOfHammingArrayInBytes(numberDataBits)];
    return hammingArray;
}

char* copyArrayToHammingArray(const char* byteArrayData, int numberDataBits, char * hammingArray){
    int counter = 0;
    int lengthOfHammingInBits = (numberDataBits + getNumberParityBits(numberDataBits));

    for (int i = 0; i < lengthOfHammingInBits; i++) {
        if(((int) pow(2, counter)) - 1 == i){
            counter++;
            clrBit(hammingArray, i);
            std::cout << " 0 ";
        } else{
            std::cout << "An der stelle i im Hamming array: " << i << ", bit aus byte array: " << tstBit(byteArrayData,i - counter) << ". ";
            if(tstBit(byteArrayData,i - counter)){
                setBit(hammingArray,i);
                std::cout << "1";
            }else{
                clrBit(hammingArray,i);
                std::cout << "0";
            }
            std::cout << "hamming an der Stelle i = " << tstBit(hammingArray, i) << std::endl;
        }
    }
    std::cout << std::endl;
    return hammingArray;
}

//char* copyArrayToHammingArray(const char* byteArrayData, int numberDataBits, char * hammingArray){
//    int counter = 0;
//    for (int i = 1; i <= (numberDataBits + getNumberParityBits(numberDataBits)); i++) {
//        if((int) pow(2, counter) == i){
//            counter++;
//            clrBit(hammingArray, i);
//        } else{
//            tstBit(byteArrayData,i - counter) ? setBit(hammingArray,i) : clrBit(hammingArray,i);
//        }
//    }
//    return hammingArray;
//}

int calculateParityBitOnHamming(int parityBit, const char* hammingArray, int numberOfBits){
    int counter = 0;
    for (int i = parityBit; i <= numberOfBits ; i += (parityBit + 1) * 2) {
        for (int j = i; j <= parityBit + i - 1 && j <= numberOfBits ; j++) {
            if(tstBit(hammingArray,j) == 1 && j != parityBit){
                counter++;
                std::cout << 1;
            } else {
                std::cout << 0;
            }
        }
    }
    std::cout << std::endl;
    return counter % 2 == 0 ? 0 : 1;
}

//richtig
void showArray(const char* byteArray, int numberBits){
    for (int i = 0; i < numberBits; i++) {
        (i % 8 == 7 && i != 0) ? (std::cout << tstBit(byteArray, i) << " ")
                     : (std::cout << tstBit(byteArray, i));
    }
    std::cout << std::endl;
}

int computeHammingCode(const char* byteArrayData, int numberDataBits, char * hammingArray){
    char* copiedHamming = copyArrayToHammingArray(byteArrayData, numberDataBits, hammingArray);

    for (int i = 0; i < getNumberParityBits(numberDataBits) ; i++) {
        if(calculateParityBitOnHamming((int) pow(2, i) - 1,copiedHamming,getNumberParityBits(numberDataBits) + numberDataBits) == 0){
            clrBit(copiedHamming, (int) pow(2,i));
        } else{
            setBit(copiedHamming, (int) pow(2,i));
        }
    }

    return 1;
}
 //richtig
bool isParityBit(int pos){
    if(pos == 0){
        return false;
    }
    return ((pos & (pos-1)) == 0);
}
//richtig
void showParityBits(const char* byteArray, int numberBits){
    for (int i = 0; i < numberBits; i++) {
        if(isParityBit(i+1)){
            std::cout << tstBit(byteArray, i);
        }
    }
    std::cout << std::endl;
}

void showDataBits(const char* byteArray, int numberBits){
    for (int i = 0; i < numberBits; i++) {
        if(isParityBit(i + 1) == 0){
            std::cout << tstBit(byteArray, i);
        }
    }
    std::cout << std::endl;
}

int checkAndCorrectHammingCode(char* hammingArray, int numberBits){
    int errorBit = 0;
    for (int i = 1; i <= numberBits; i++) {
        if(isParityBit(i)){
            if((calculateParityBitOnHamming(i, hammingArray, numberBits) != tstBit(hammingArray,i))){
                errorBit+= i;
            }
        }
    }

    if(errorBit == 0){
        std::cout << "Es wurde kein Fehler gefunden!" << std::endl;
    } else if (errorBit < numberBits) {
        if (tstBit(hammingArray, errorBit)) {
            clrBit(hammingArray, errorBit);
        } else {
            setBit(hammingArray, errorBit);
        }
        std::cout << "Es wurde versucht den Fehler zu beheben. Es ist trotzdem moeglich, dass das Ergebnis falsch ist." << std::endl;
    } else if(errorBit >= numberBits){
        std::cout << "Es wurden zu viele Bits geflippt. Der Fehler kann nicht behoben werden." << std::endl;
    }

    return errorBit;
}

int getDataBits(const char* byteArray, int numberBits){
    int counter = 0;
    for (int i = 1; i <= numberBits; i++) {
        if(isParityBit(i) == 1){
            counter++;
        }
    }
    return counter;
}

char* hammingToData(char* hammingArray, int numberOfBits){
    int counter = 0;
    int length = (numberOfBits - getNumberParityBits(getDataBits(hammingArray, numberOfBits)))/8 + 1;
    char* dataArray = new char [length];
    for (int i = 1; i < numberOfBits; i++) {
        if (!isParityBit(i)){
            tstBit(hammingArray, i) ? setBit(dataArray, i-counter) : clrBit(dataArray, i-counter);
        } else {
            counter++;
        }
    }
    dataArray[length-1] = '\0';
    return dataArray;
}

void printHammingProcess(char* array, int length, char* hammingArray, int lengthHamming) {

    computeHammingCode(array, length, hammingArray);

    std::cout << "Ausgangsbitfolge:         ";
    showArray(array, length);

    std::cout << "Hammingcode:              ";
    showArray(hammingArray, lengthHamming);

    std::cout << "Databits:                 ";
    showDataBits(hammingArray, lengthHamming);

    std::cout << "Paritybits:               ";
    showParityBits(hammingArray, lengthHamming);

}

void printHammingResult (char* array, int length, char* hammingArray, int lengthHamming) {
    std::cout << "Hammingcode (changed):    ";
    showArray(hammingArray, lengthHamming);

    int errorBit = checkAndCorrectHammingCode(hammingArray, lengthHamming);
    errorBit == 0 ? std::cout << "" : std::cout << "Das geflippte Bit befindet sich an der Stelle: " << errorBit << std::endl;
    std::cout << "Das urspruengliche Wort lautet: ";
    for (int i = 0; i < lengthHamming  / 8; i++) {
        std::cout << array[i];
    }
    std::cout << std::endl;
    std::cout << "Das Wort lautet: " << hammingToData(hammingArray, lengthHamming) << std::endl;


    std::cout << "Hammingcode (correct):    ";
    showArray(hammingArray, lengthHamming);
}

int main() {
    char array[] = {'a',86 , 'l', 'l', 'o'};

    //Bsp 1 (Bitfolge geflippt, aber als richtig erkannt)
    int length = 16;

    int lengthHamming1 = length + getNumberParityBits(length);
    char * hammingArray1 = initHammingArray(length);

    showArray(hammingArray1, lengthHamming1);

    printHammingProcess(array, length, hammingArray1, lengthHamming1);



////    setBit(hammingArray1, 2);
////    clrBit(hammingArray1, 1);
////    setBit(hammingArray1, 3);
//
//    printHammingResult(array, length, hammingArray1, lengthHamming1);
//
//    std::cout << std::endl;
//
//
//    //Bsp 2
//    length = 17;
//    int lengthHamming2 = length + getNumberParityBits(length);
//    char * hammingArray2 = initHammingArray(length);
//
//    printHammingProcess(array, length, hammingArray2, lengthHamming2);
//
////    setBit(hammingArray2, 7);
//
//    printHammingResult(array, length, hammingArray2, lengthHamming2);
//
//    std::cout << std::endl;
//
//    //Bsp 3
//    length = 40;
//    int lengthHamming3 = length + getNumberParityBits(length);
//    char * hammingArray3 = initHammingArray(length);
//
//    printHammingProcess(array, length, hammingArray3, lengthHamming3);
//
////    setBit(hammingArray3, 7);
//
//    printHammingResult(array, length, hammingArray3, lengthHamming3);
//
//    return 0;
}