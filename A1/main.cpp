#include <iostream>
#include "math.h"

struct FluxCapacitor
{
    unsigned char* c_string; // Hierbei handelt es sich bei 'c_string' um einen C-String (d.h. ein '\0' terminiertes char Array).
    unsigned int value;
};

FluxCapacitor* createFluxCapacitor(){
    int value = rand();

    int length = (rand() % 20) + 1;
    unsigned char array[length];

    for (int i = 0; i < length-1; ++i) {
        int randomNumber = rand() % 3;
        if(randomNumber == 0){
            array[i] = (unsigned char) (rand() % 26 + 65);
        } else if (randomNumber == 1){
            array[i] = (unsigned char) (rand() % 26 + 97);
        }else {
            array[i] = (unsigned char) (rand() % 10 + 48);
        }
    }
    array[length-1] = '\0';

    FluxCapacitor* fluxCapacitor = new FluxCapacitor;
    fluxCapacitor->value = value;
    fluxCapacitor->c_string = new unsigned char [length];
    memcpy(fluxCapacitor->c_string, array, sizeof(unsigned char) * length);

    return fluxCapacitor;
}

void printFluxCapacitor(FluxCapacitor* fluxCapacitor){
    std::cout << fluxCapacitor->value <<std::endl;
    std::cout << fluxCapacitor->c_string << std::endl;
}



int main() {

    FluxCapacitor* fluxCapacitor = createFluxCapacitor();
    printFluxCapacitor(fluxCapacitor);


//    unsigned char array[10];
//
//    for (int i = 0; i < 9; ++i) {
//        array[i] = (unsigned char) (rand() % 25 + 65);
//    }
//
//    std::cout << array << std::endl;
}
