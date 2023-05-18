#include <iostream>
#include <cmath>

/*
 * 24 Bit Adresse
 * Page ist 1 KibiByte groß
 * virtueller Adressraum 24 Bit
 * Ram ist 1 MegiByte groß, 1 MegiByte = 8388608 Bit
 */

#define SIZE_PAGE 1024
#define SIZE_PAGE_FRAME 1024
#define SIZE_RAM_IN_BIT (int) pow(2, 20)
#define SIZE_VM_IN_BIT (int) pow(2, 24)
#define NUMBER_OF_PAGES (int) pow(2, 14)
#define NUMBER_OF_PAGE_FRAMES (int) pow(2, 10)
//#define FRAME_TLB '1';
//#define FRAME_REFERENCED '2';
//#define FRAME_TLB_AND_REFERENCED '3';

struct PageTableEntry{
    unsigned int page_frame_index;
    unsigned char frame_attributes;
};

struct PageTable{
    struct PageTableEntry** entries;
    unsigned int size;
};

struct TLBEntry{
    unsigned int page_index;
    unsigned int page_frame_index;
    unsigned char frame_attributes;
};

struct TLB{
    struct TLBEntry** entries;
    unsigned int size;
};

struct Process{
    unsigned char process_id;
    struct PageTable* page_table;
};

struct MMU{
    struct TLB* tlb;
    struct Process* process;
};

struct ED_209{
    struct MMU* mmu;
};

TLB* tlb;
const unsigned char FRAME_TLB = '1';
const unsigned char FRAME_REFERENCED = '2';
const unsigned char FRAME_AND_REFERENCED = '3';
int numberOfEntriesInTLB = 0;

int* numberTo24BitBinary(int number){
    int* numberTo24BitBinaryArray = new int[24];
    int counter = 23;
    int a = number;

    while (a != 0){
        numberTo24BitBinaryArray[counter] = a % 2;
        a = a / 2;
        counter--;
    }

    for (int i = 0; i <= counter; i++){
        numberTo24BitBinaryArray[i] = 0;
    }

    return numberTo24BitBinaryArray;
}

int getPageIndex14Bit(const int* number24BitBinaryArray){
    int sum = 0;
    int counter = 0;

    for (int i = 13; i >= 0; i--){
        if (number24BitBinaryArray[i] == 1){
            sum += (int) pow(2, counter);
        }
        counter++;
    }

    return sum;
}

void printSystemInfo(){
    std::cout << "Size of Page: " << SIZE_PAGE << " Bit" << std::endl;
    std::cout << "Size of Page Frame: " << SIZE_PAGE_FRAME << " Bit" << std::endl;
    std::cout << "Size of RAM: " << SIZE_RAM_IN_BIT << " Bit" << std::endl;
    std::cout << "Size of Virtualmemory: " << SIZE_VM_IN_BIT << " Bit" << std::endl;
    std::cout << "Number of Pages: " << NUMBER_OF_PAGES << std::endl;
    std::cout << "Number of Page Frames: " << NUMBER_OF_PAGE_FRAMES << std::endl;
}

PageTable* createPageTable(){
    PageTable* pageTable = new PageTable;
    PageTableEntry** entries = new PageTableEntry*[NUMBER_OF_PAGES];
    pageTable->size = NUMBER_OF_PAGES;
    pageTable->entries = entries;
    return pageTable;
}

TLB* createTLB(unsigned int size){
    TLB* tlb = new TLB;
    TLBEntry** entries = new TLBEntry*[size];
    tlb->size = size;
    tlb->entries = entries;
    return tlb;
}

Process* createProcess(unsigned char id){
    Process* process = new Process;
    PageTable* pageTable = new PageTable;
    process->process_id = id;
    process->page_table = pageTable;
    return process;
}

unsigned int countTLBEntries(struct Process* p){
    unsigned  int numberOfEntriesAlsoInTlb = 0;

    for (int i = 0; i < p->page_table->size; i++) {
        if((p->page_table->entries[i]->frame_attributes) == FRAME_TLB){
            numberOfEntriesAlsoInTlb++;
            numberOfEntriesInTLB++;
            tlb->entries[numberOfEntriesInTLB]->frame_attributes = p->page_table->entries[i]->frame_attributes;
            tlb->entries[numberOfEntriesInTLB]->page_index = p->page_table->entries[i]->page_frame_index;
        }
    }
    return numberOfEntriesAlsoInTlb;
}

int main() {
//    printSystemInfo();
//    PageTable* pageTable = createPageTable();
//    tlb = createTLB(64);
//    Process* process = createProcess(160);
//
//    int adresse = 0xBDDE10;
//    PageTableEntry* entry = (PageTableEntry*) &adresse;
//    std::cout << (int) entry->frame_attributes<<std::endl;
//    std::cout << entry->page_frame_index<<std::endl;

    int* array = numberTo24BitBinary(0xBDDE10);
    for (int i = 0; i < 24; i++) {
        std::cout << array[i] << ", ";
    }

    std::cout << std::endl <<  getPageIndex14Bit(array);

    return 0;
}