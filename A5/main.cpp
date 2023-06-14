#include <iostream>
#include <cmath>


void setBit(char* array, int bitToSet){
    array[bitToSet/8] = array[bitToSet/8] | ((int) pow(2, (bitToSet % 8)));
}

void clrBit(char* array, int bitToClear){
    array[bitToClear/8] = array[bitToClear/8] & ~((int) pow(2, (bitToClear % 8)));
}

int tstBit(const char* array, int bitToTest){
    return (array[bitToTest/8] & (int) pow(2, (bitToTest % 8))) ? 1 : 0;
}

int getNumberParityBits(int numberDataBits){
    int counter = 1;
    while(true){
        if((int) pow(2, counter) > numberDataBits){
            if(numberDataBits + counter == (int) pow(2, counter)){
                counter++;
            }
            return counter;
        }
        counter++;
    }
}

bool isParityBit(int position){
    position++;
    if(position == 0){
        return false;
    }
    return ((position & (position - 1)) == 0) ;
}

int getNumberDataBits(int numberBits) {
    int counter = 0;
    for (int i = 0; i < numberBits; i++) {
        if(!isParityBit(i)){
            counter++;
        }
    }
    return counter;
}

int sizeOfHammingArrayInBytes(int numberDataBits){
    int sumBits = numberDataBits + getNumberParityBits(numberDataBits);
    return (sumBits % 8 == 0) ? (sumBits / 8) : (sumBits / 8) + 1;
}

char* initHammingArray(int numberDataBits){
    char* hammingArray = new char[sizeOfHammingArrayInBytes(numberDataBits)];
    return hammingArray;
}

void showArray(const char* byteArray, int numberBits){
    for (int i = 0; i < numberBits; i++) {
        (i % 8 == 7 && i != 0) ? (std::cout << tstBit(byteArray, i) << " ")
                               : (std::cout << tstBit(byteArray, i));
    }
    std::cout << std::endl;
}

void showParityBits(const char* byteArray, int numberBits){
    int counter = 0;
    for(int i = 0; i < numberBits; i++) {
        if(isParityBit(i)) {
            (counter % 8 == 7) ? (std::cout << tstBit(byteArray, i) << " ")
                               : (std::cout << tstBit(byteArray, i));
            counter++;
        }
    }
    std::cout << std::endl;
}

void showDataBits (const char* byteArray, int numberBits){
    int counter = 0;
    for(int i = 0; i < numberBits; i++) {
        if(!isParityBit(i)) {
            (counter % 8 == 7 && counter != 0) ? (std::cout << tstBit(byteArray, i) << " ")
                                               : (std::cout << tstBit(byteArray, i));
            counter++;
        }
    }
    std::cout << std::endl;
}

char* copyArrayToHammingArray(const char* byteArray, int numberDataBits, char * hammingArray){
    int counter = 0;
    int lengthHamming = numberDataBits + getNumberParityBits(numberDataBits);

    for (int i = 0; i < lengthHamming; i++) {
        if(isParityBit(i)){
            counter++;
            clrBit(hammingArray, i);
        } else{
            if(tstBit(byteArray, i - counter)){
                setBit(hammingArray, i);
            }else{
                clrBit(hammingArray, i);
            }
        }
    }
    return hammingArray;
}

int calculateParityBitOnHamming(int parityBit, const char* hammingArray, int numberHammingBits){
    int counter = 0;
    for(int i = parityBit; i < numberHammingBits; i += ((parityBit + 1) * 2)){
        for (int j = i; j < parityBit + i + 1 && j < numberHammingBits; j++) {
            if(tstBit(hammingArray,j) && j != parityBit){
                counter++;
            }
        }
    }
    return counter % 2 == 0 ? 0 : 1;
}

int computeHammingCode(const char* byteArrayData, int numberDataBits, char * hammingArray){
    char* copiedHamming = copyArrayToHammingArray(byteArrayData, numberDataBits, hammingArray);

    for (int i = 0; i < getNumberParityBits(numberDataBits) ; i++) {
        if(calculateParityBitOnHamming((int) pow(2, i) - 1,copiedHamming,getNumberParityBits(numberDataBits) + numberDataBits) == 0){
            clrBit(copiedHamming, (int) pow(2,i) - 1);
        } else{
            setBit(copiedHamming, (int) pow(2,i) - 1);
        }
    }
    return 1;
}

char* hammingToData(char* hammingArray, int numberHammingBits){
    int length = (int) (ceil(getNumberDataBits(numberHammingBits) / 8.0) + 1);
    char* byteArray = new char[length];
    int counter = 0;

    for (int i = 0; i < numberHammingBits; i++) {
        if(!isParityBit(i)){
            tstBit(hammingArray, i) ? setBit(byteArray, i-counter) : clrBit(byteArray, i-counter);
        } else{
            counter++;
        }
    }
    byteArray[length - 1] = '\n';
    return byteArray;
}

int checkAndCorrectHammingCode(char* hammingArray, int numberHammingBits){
    int errorBit = 0;
    for (int i = 0; i < numberHammingBits; i++) {
        if(isParityBit(i)) {
            if ((calculateParityBitOnHamming(i, hammingArray, numberHammingBits) != tstBit(hammingArray, i))) {
                errorBit += i + 1;
            }
        }
    }
    errorBit--;

    if(errorBit == -1){
        std::cout << "Es wurde kein Fehler gefunden!" << std::endl;
    } else if (errorBit < numberHammingBits) {
        if (tstBit(hammingArray, errorBit)) {
            clrBit(hammingArray, errorBit);
        } else {
            setBit(hammingArray, errorBit);
        }
        std::cout << "Es wurde versucht den Fehler zu beheben. Es ist trotzdem moeglich, dass das Ergebnis falsch ist." << std::endl;
    } else if(errorBit >= numberHammingBits){
        std::cout << "Es wurden zu viele Bits geflippt. Der Fehler kann nicht behoben werden." << std::endl;
    }

    return errorBit;
}

void printHammingProcess(char* byteArray, int numberDataBits, char* hammingArray, int numberHammingBits) {
    computeHammingCode(byteArray, numberDataBits, hammingArray);

    std::cout << "Ausgangsbitfolge:                 ";
    showArray(byteArray, numberDataBits);

    std::cout << "Databits:                         ";
    showDataBits(hammingArray, numberHammingBits);

    std::cout << "Paritybits:                       ";
    showParityBits(hammingArray, numberHammingBits);

    std::cout << "Hammingcode:                      ";
    showArray(hammingArray, numberHammingBits);
}

void printHammingResult (char* byteArray, int numberDataBits, char* hammingArray, int numberHammingBits) {
    std::cout << "Hammingcode (changed):            ";
    showArray(hammingArray, numberHammingBits);

    int errorBit = checkAndCorrectHammingCode(hammingArray, numberHammingBits);

    errorBit == -1 ? std::cout << "" : std::cout << "Das geflippte Bit befindet sich an der Stelle: " << errorBit << std::endl;

    if(numberDataBits % 8 == 0){
        std::cout << "Das Ausgangswort lautet:          ";
        for (int i = 0; i < numberDataBits / 8; i++) {
            std::cout << byteArray[i];
        }
        std::cout << std::endl;

        std::cout << "Das korrigierte Wort lautet:      ";
        char* hammingToDataArray = hammingToData(hammingArray, numberHammingBits);
        for (int i = 0; i < numberDataBits / 8; i++) {
            std::cout << hammingToDataArray[i];
        }
    } else{
        std::cout << "Das korrigierte Hammingarray ist: ";
        showArray(hammingArray, numberHammingBits);

        std::cout << "Das Ausgangswort war:             ";
        showArray(byteArray, numberDataBits);

        std::cout << "Das korrigierte Wort lautet:      ";
        showArray(hammingToData(hammingArray, numberHammingBits), numberDataBits);
    }
}

char* generateRandomByteArray(int randDataBits){
    int randArrayLength = (int) (ceil(randDataBits / 8.0));
    char* randByteArray = new char[randArrayLength];

    for (int i = 0; i < randArrayLength; ++i) {
        int randChar = rand() % 3;
        if(randChar == 0){
            randByteArray[i] = (char)((rand() % 25) + 65);
        } else if (randChar == 1){
            randByteArray[i] = (char)((rand() % 25) + 97);
        } else{
            randByteArray[i] = (char)((rand() % 10) + 48);
        }
    }
    return randByteArray;
}

int flipRandomBits(char* hammingArray, int numberHammingBits){
    int counterFlippedBits = 0;
    bool flip = rand() % 2;

    for (int i = 0; i < numberHammingBits; i++) {
        if(flip){
            tstBit(hammingArray, i) ? clrBit(hammingArray, i) : setBit(hammingArray, i);
            counterFlippedBits++;
        }
        flip = rand() % 2;
    }
    return counterFlippedBits;
}

int main(){
    int length = 16;
    char byteArray[] = {'a', 86 , 'l', 'l', 'o'};
    int numberHammingBits1 = length + getNumberParityBits(length);
    char * hammingArray1 = initHammingArray(length);

    std::cout << "Beispiel 1: ein Bit geflippt und korrigiert" << std::endl;
    printHammingProcess(byteArray, length, hammingArray1, numberHammingBits1);
    clrBit(hammingArray1, 2);
    printHammingResult(byteArray, length, hammingArray1, numberHammingBits1);


    std::cout << "\n" << std::endl;
    std::cout << "Beispiel 2: ersten 3 Bits geflippt -> kein Fehler entdeckt, aber trotzdem falsch" << std::endl;

    int numberHammingBits2 = length + getNumberParityBits(length);
    char * hammingArray2 = initHammingArray(length);

    printHammingProcess(byteArray, length, hammingArray2, numberHammingBits2);
    clrBit(hammingArray2, 0);
    setBit(hammingArray2, 1);
    clrBit(hammingArray2, 2);
    printHammingResult(byteArray, length, hammingArray2, numberHammingBits2);

    std::cout << "\n" << std::endl;
    std::cout << "Beispiel 3: random Bits geflippt" << std::endl;


    //int randDataBits1 = rand() % 48 + 1;
    int randDataBits1 = 48;
    char* randByteArray1 = generateRandomByteArray(randDataBits1);

    int numberHammingBits3 = randDataBits1 + getNumberParityBits(randDataBits1);
    char* hammingArray3 = initHammingArray(randDataBits1);

    printHammingProcess(randByteArray1, randDataBits1, hammingArray3 , numberHammingBits3);
    flipRandomBits(hammingArray3, numberHammingBits3);
    printHammingResult(randByteArray1, randDataBits1, hammingArray3, numberHammingBits3);


    std::cout << "\n" << std::endl;
    std::cout << "Beispiel 4: random Bit geflippt --> korrigiert" <<  std::endl;

    int randDataBits2 = rand() % 48 + 1;
    //int randDataBits2 = 16;
    char* randByteArray2 = generateRandomByteArray(randDataBits2);

    int numberHammingBits4 = randDataBits2 + getNumberParityBits(randDataBits2);
    char* hammingArray4 = initHammingArray(randDataBits2);

    printHammingProcess(randByteArray2, randDataBits2, hammingArray4 , numberHammingBits4);

    int bitToFlip = rand() % randDataBits2;
    std::cout << "Das random geflippte Bit befindet sich an Stelle: " << bitToFlip << std::endl;
    tstBit(hammingArray4, bitToFlip) ? clrBit(hammingArray4, bitToFlip) : setBit(hammingArray4, bitToFlip);

    printHammingResult(randByteArray2, randDataBits2, hammingArray4, numberHammingBits4);
}