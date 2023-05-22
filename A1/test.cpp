#include <iostream>
#include <pthread.h>

void* numbersFroOneToTen(void* length){
    for (int i = 0; i < *(int*) length; i++) {
        std::cout << i << std::endl;
    }
    return length;
}


int main(){
    pthread_t pthread;
    int number = 6;

    pthread_create(&pthread, NULL, &numbersFroOneToTen, &number);

}
