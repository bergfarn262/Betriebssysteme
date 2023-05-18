#include <iostream>
#include <pthread.h>

void* numbersFroOneToTen(void* length){
    std::cout << *(int*) length << std::endl;
    for (int i = 0; i < *(int*) length; i++) {
        std::cout << i << std::endl;
    }
    return length;
}


int main(){
    pthread_t pthread;
    int number = 6;
    //numbersFroOneToTen(&number);

    pthread_create(&pthread, nullptr, &numbersFroOneToTen, &number);

}
