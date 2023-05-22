#include <iostream>
#include <cmath>

#define SIZE_PAGE 10
#define SIZE_PAGE_FRAME 10
#define SIZE_RAM_IN_BIT 8388608
#define SIZE_VM_IN_BIT (int) pow(2, 24)
#define NUMBER_OF_PAGES (int) pow(2, 14)
#define NUMBER_OF_PAGE_FRAMES (int) pow(2, 10)

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

const unsigned char FRAME_TLB = '1';
const unsigned char FRAME_REFERENCED = '2';
const unsigned char TLB_AND_REFERENCED = '3';

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

    for (int i = 0; i < NUMBER_OF_PAGES; i++)  {
        entries[i] = new PageTableEntry;
    }

    pageTable->size = NUMBER_OF_PAGES;
    pageTable->entries = entries;

    return pageTable;
}

TLB* createTLB(unsigned int size){
    TLB* tlb = new TLB;
    TLBEntry** entries = new TLBEntry*[size];

    for (int i = 0; i < size; i++)  {
        entries[i] = new TLBEntry;
    }

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
    unsigned  int numberOfEntriesInTlb = 0;

    for (int i = 0; i < p->page_table->size; i++) {
        if((p->page_table->entries[i]->frame_attributes) == FRAME_TLB){
            numberOfEntriesInTlb++;
        }
    }

    return numberOfEntriesInTlb;
}

void copyTLBEntries(Process* p, TLB* tlb){
    tlb->size = countTLBEntries(p);
    int counter = 0;

    for (int i = 0; i < p->page_table->size; i++) {
        if(p->page_table->entries[i]->frame_attributes == FRAME_TLB || p->page_table->entries[i]->frame_attributes == TLB_AND_REFERENCED){
            tlb->entries[counter]->page_index = i;
            tlb->entries[counter]->page_frame_index = p->page_table->entries[i]->page_frame_index;
            tlb->entries[counter]->frame_attributes = p->page_table->entries[i]->frame_attributes;

            counter++;
        }
    }
}

unsigned int translate(unsigned int virtual_address, PageTable* pt){
    unsigned int vt = ( virtual_address>>10 );
    pt->entries[vt]->frame_attributes = TLB_AND_REFERENCED;
    return vt;
}

int preprocessInstructions(MMU* mmu, unsigned int* instructions, unsigned int instruction_count){
    for (int i = 0; i < instruction_count; i++){
        unsigned int t = translate(instructions[i], mmu->process->page_table);
        mmu->process->page_table->entries[t]->page_frame_index = i;
    }

    copyTLBEntries(mmu->process, mmu->tlb);

    for (int i = 0; i < mmu->process->page_table->size; i++) {
        if(mmu->process->page_table->entries[i]->frame_attributes == FRAME_REFERENCED || mmu->process->page_table->entries[i]->frame_attributes == TLB_AND_REFERENCED){
            return 0;
        }
    }

    return -1;
}

void startED209(struct ED_209* ed){
    //check if mmu contains a tlb with the valid instructions

    if (
        ed->mmu->tlb->entries[0]->page_frame_index == 0x3 && ed->mmu->tlb->entries[0]->page_index == 0x15 &&
        ed->mmu->tlb->entries[1]->page_frame_index == 0x1 && ed->mmu->tlb->entries[1]->page_index == 0x17f &&
        ed->mmu->tlb->entries[2]->page_frame_index == 0x4 && ed->mmu->tlb->entries[2]->page_index == 0x3ff &&
        ed->mmu->tlb->entries[3]->page_frame_index == 0x2 && ed->mmu->tlb->entries[3]->page_index == 0x152b &&
        ed->mmu->tlb->entries[4]->page_frame_index == 0x0 && ed->mmu->tlb->entries[4]->page_index == 0x2f77)
    {
        printf("ED-209 is working within normal parameters -> SIMULATION SUCCEEDED\n");

    }
    else
    {
        printf("ERROR: ED-209 is out of control -> SIMULATION FAILED\n");
    }
}

int main() {
    printSystemInfo();
    std::cout << "\n";

    unsigned int edSimulation[5] = {0xBDDE10, 0x5FFFE, 0x54AC01, 0x540C, 0xFFFFE};

    ED_209* ed209 = new ED_209;
    ed209->mmu = new MMU;
    ed209->mmu->tlb = createTLB(5);
    ed209->mmu->process = createProcess(0);
    ed209->mmu->process->page_table = createPageTable();
    std::cout << ((preprocessInstructions(ed209->mmu, edSimulation, 5) == 0) ? "Es wurden referenzierte Eintraege gefunden." : "Es wurden keine referenzierten Eintraege gefunden." ) << "\n" << std::endl;

    startED209(ed209);


    return 0;
}




