#include <iostream>
#include "math.h"
#include "string.h"
#include "stdlib.h"
#include <pthread.h>

struct FluxCapacitor {
    unsigned char* c_string; // Hierbei handelt es sich bei 'c_string' um einen C-String (d.h. ein '\0' terminiertes char Array).
    unsigned int value;
};

struct DeLorean {
    double speed_in_kmh;
    double current_power_level_in_mega_watts;
};

struct IndexContainer {
    unsigned int* cap_indices;
    unsigned int array_length;
};

struct DeLorean* delorean__;
struct FluxCapacitor** capacitors__;

FluxCapacitor* createFluxCapacitor(){
//    int value = rand() % 1024;
    int value = 10;

    int length = (rand() % 20) + 1;
    unsigned char array[length];

    for (int i = 0; i < length-1; ++i){
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

FluxCapacitor** createFluxCapacitorArray(unsigned int n){
    FluxCapacitor** array = new FluxCapacitor* [n];
    for (int i=0; i<n; i++) {
        array[i] = createFluxCapacitor();
    }
    return array;
}

void printFluxCapacitor(FluxCapacitor* fluxCapacitor){
    std::cout << "value:" << fluxCapacitor->value << std::endl;
    std::cout << "c_string:" << fluxCapacitor->c_string << std::endl;
}

void printFluxCapacitorArray(struct FluxCapacitor** array, unsigned int n){
    for (int i=0; i<n; i++) {
        printFluxCapacitor(array[i]);
        std::cout << "----------------------" << std::endl;
    }
}

void bubbleSortFluxCapacitorArray(struct FluxCapacitor** array, unsigned int n){
    FluxCapacitor* temp;
    for (int i=n-1; i>1; i--) {
        for (int j=0; j<i; j++) {
            if (array[j]->value > array [j+1]->value){
                temp = array [j+1];
                array[j+1] = array[j];
                array[j] = temp;
            }
        }
    }
}

void* assembleDeLorean(void* indices_) {
    double sum = 0;
    IndexContainer* indices = (IndexContainer*) indices_ ;
    for (int i = 0; i < 121 ; i++) {
        for (int j = 0; j < indices->array_length; j++) {
            if ( i == indices->cap_indices[j]) {
                sum += capacitors__[i]->value;
            }
        }
    }
    delorean__->current_power_level_in_mega_watts = sum;
}

int main(){
    capacitors__ = createFluxCapacitorArray(121);

    IndexContainer* indexContainer1 = new IndexContainer;
    indexContainer1->array_length = 21;
    indexContainer1->cap_indices = new unsigned int[indexContainer1->array_length];
    for (int i = 0; i < indexContainer1->array_length; i++) {
        indexContainer1->cap_indices[i] = i;
    }

    IndexContainer* indexContainer2 = new IndexContainer;
    indexContainer2->array_length = 100;
    indexContainer2->cap_indices = new unsigned int[indexContainer2->array_length];
    for (int i = 0; i < indexContainer2->array_length; i++) {
        indexContainer2->cap_indices[i] = i+21;
    }
}