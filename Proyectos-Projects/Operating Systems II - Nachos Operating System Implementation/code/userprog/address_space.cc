/// Routines to manage address spaces (memory for executing user programs).
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.

#include "address_space.hh"
#include "threads/system.hh"
#include <cstdlib>

#include <cstdio>
#include <string.h>

static int getPhysPageBase(TranslationEntry* pageTable, uint32_t virtualAddr) {
	return (pageTable[virtualAddr / PAGE_SIZE].physicalPage) * PAGE_SIZE;
}

static int min(int a, int b) {
    return a < b ? a : b;
}

AddressSpace::AddressSpace(OpenFile *executable_file)
{
    ASSERT(executable_file != nullptr);
    Executable exe (executable_file);
    ASSERT(exe.CheckMagic());

    // How big is address space?
    unsigned size = exe.GetSize() + USER_STACK_SIZE;
      // We need to increase the size to leave room for the stack.
    numPages = DivRoundUp(size, PAGE_SIZE);
    size = numPages * PAGE_SIZE;
#ifndef SWAP
    ASSERT(numPages <= freeMem->CountClear());
#else
    swapName = nullptr;
    swapFile = nullptr;
#endif
    executable = executable_file;// Guardamos el ejecutable para luego poder hacer carga de páginas individuales
    
    DEBUG('a', "Initializing address space, num pages %u, size %u\n",
          numPages, size);
    // First, set up the translation.
    
    pageTable = new TranslationEntry[numPages];
    for (unsigned i = 0; i < numPages; i++) {
        pageTable[i].virtualPage  = i;
#ifdef DEMAND_LOADING
        pageTable[i].physicalPage = -1;
        pageTable[i].valid        = false;
#else
        pageTable[i].valid        = false;
#ifdef SWAP
        pageTable[i].physicalPage = freeMem->Find_with_Replacement(i);
#else
        pageTable[i].physicalPage = freeMem->Find(); // Chequear si es -1 (???)
#endif
#endif
        pageTable[i].use          = false;
        pageTable[i].dirty        = false;
        pageTable[i].readOnly     = false;
        // If the code segment was entirely on a separate page, we could
        // set its pages to be read-only.
    }
    char *mainMemory = machine->mainMemory;
    // Zero out the entire address space, to zero the unitialized data
    // segment and the stack segment.
    // Para multiprogramación...
#ifndef DEMAND_LOADING
    for (unsigned i = 0; i < numPages; i++) {
      memset(mainMemory + (pageTable[i].physicalPage * PAGE_SIZE), 0, PAGE_SIZE);
    }

    // Then, copy in the code and data segments into memory.
    uint32_t codeSize = exe.GetCodeSize();
    if (codeSize > 0) {
        uint32_t virtualAddr = exe.GetCodeAddr();
        DEBUG('a', "Initializing code segment, at 0x%X, size %u\n",
              virtualAddr, codeSize);

        int read;
        for (uint32_t totalRead = 0; totalRead < codeSize; totalRead += read) {
            int base = getPhysPageBase(pageTable, virtualAddr + totalRead);
            int offset = (virtualAddr + totalRead) % PAGE_SIZE;

            read = min(codeSize - totalRead, PAGE_SIZE - offset);

            exe.ReadCodeBlock(mainMemory + base + offset, read, totalRead);
        }
    }

    uint32_t initDataSize = exe.GetInitDataSize();
    if (initDataSize > 0) {
        uint32_t virtualAddr = exe.GetInitDataAddr();
        DEBUG('a', "Initializing data segment, at 0x%X, size %u\n",
              virtualAddr, initDataSize);

        int read;
        for (uint32_t totalRead = 0; totalRead < initDataSize; totalRead += read) {
            int base = getPhysPageBase(pageTable, virtualAddr + totalRead);
            int offset = (virtualAddr + totalRead) % PAGE_SIZE;

            read = min(initDataSize - totalRead, PAGE_SIZE - offset);

            exe.ReadDataBlock(mainMemory + base + offset, read, totalRead);
        }
    }
#endif
}

/// Deallocate an address space.
AddressSpace::~AddressSpace()
{

    for (unsigned i = 0; i < numPages; i++) {
        if (pageTable[i].valid)
            freeMem->Clear(pageTable[i].physicalPage);
    }
    delete [] pageTable;
    delete executable;

#ifdef SWAP
    if (swapFile != nullptr) {
        delete swapFile;
        fileSystem->Remove(swapName);
        delete[] swapName;
    }
#endif

}

/// Set the initial values for the user-level register set.
///
/// We write these directly into the “machine” registers, so that we can
/// immediately jump to user code.  Note that these will be saved/restored
/// into the `currentThread->userRegisters` when this thread is context
/// switched out.
void
AddressSpace::InitRegisters()
{
    for (unsigned i = 0; i < NUM_TOTAL_REGS; i++) {
        machine->WriteRegister(i, 0);
    }

    // Initial program counter -- must be location of `Start`.
    machine->WriteRegister(PC_REG, 0);

    // Need to also tell MIPS where next instruction is, because of branch
    // delay possibility.
    machine->WriteRegister(NEXT_PC_REG, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we do not
    // accidentally reference off the end!
    machine->WriteRegister(STACK_REG, numPages * PAGE_SIZE - 16);
    DEBUG('a', "Initializing stack register to %u\n",
          numPages * PAGE_SIZE - 16);
}

/// On a context switch, save any machine state, specific to this address
/// space, that needs saving.
void
AddressSpace::SaveState()
{}

/// On a context switch, restore the machine state so that this address space
/// can run.
///
/// For now, tell the machine where to find the page table.
void
AddressSpace::RestoreState()
{
#ifdef USE_TLB
    // Si hacemos uso de la TLB, invalidamos sus entradas    
    for (unsigned i = 0; i < TLB_SIZE; i++) {
        machine->GetMMU()->tlb[i].valid = false;
    }
#else
    machine->GetMMU()->pageTable     = pageTable;
    machine->GetMMU()->pageTableSize = numPages;
#endif
}

unsigned
AddressSpace::GetNumPages() {
    return this->numPages;
}

TranslationEntry*
AddressSpace::GetPageTable() {
    return this->pageTable;
}

#ifdef DEMAND_LOADING   // Cargar pagina
void
AddressSpace::LoadPage(unsigned vpn) {
    for(unsigned i=0; i < numPages; i++) {
        ASSERT(pageTable[i].virtualPage == i || (int) pageTable[i].virtualPage == -1);
    }

    this->pageTable[vpn].valid = true;
    char *mainMemory = machine->mainMemory;
    unsigned virtualAddr = vpn * PAGE_SIZE;
    unsigned physicalAddr;
    
#ifdef SWAP
    int frame = freeMem->Find_with_Replacement(vpn);   // Buscamos el marco en memoria, swappeando el marco de ser necesario
    pageTable[vpn].physicalPage = frame;
    physicalAddr = frame * PAGE_SIZE;    
    if ((int) pageTable[vpn].virtualPage == -1) { // la página se encuentra en la swap
        DEBUG('d', "Loading page %u\n", vpn);
        swapFile->ReadAt(mainMemory + physicalAddr, PAGE_SIZE, virtualAddr);
        pageTable[vpn].virtualPage = vpn;
        DEBUG('d', "Memory retrieved from swap\n");
        return;
    }
#else
    int frame = freeMem->Find();
    ASSERT(frame != -1);
    pageTable[vpn].physicalPage = frame;
    physicalAddr = frame * PAGE_SIZE;
#endif
    Executable exe (executable);
    pageTable[vpn].virtualPage = vpn;   // La página se encuentra en el espacio del archivo ejecutable
    ASSERT(exe.CheckMagic());
    if (virtualAddr >= exe.GetCodeSize() + exe.GetInitDataSize()) { // en el stack
        memset(mainMemory + physicalAddr, 0, PAGE_SIZE);
        return;
    } else  { // .text o .initData
        unsigned read = 0;
        if(virtualAddr < exe.GetCodeSize()) { // tenemos que leer, al menos en parte, código
            read = PAGE_SIZE + min(0, exe.GetCodeSize() - (virtualAddr + PAGE_SIZE)); // leemos lo que falte de la página de texto
            exe.ReadCodeBlock(mainMemory + physicalAddr, read, vpn * PAGE_SIZE);
            unsigned offset = read;
            if (read < PAGE_SIZE) { // Tenemos que leer parte del .data
                read = min(PAGE_SIZE - read, exe.GetInitDataSize());
                if(exe.GetInitDataSize() > 0)
                    exe.ReadDataBlock(mainMemory + physicalAddr + offset, PAGE_SIZE - read, 0);
            }
        } else { // Solo leemos .data
            read = PAGE_SIZE + min(0, (exe.GetCodeSize() + exe.GetInitDataSize() -(virtualAddr + PAGE_SIZE))); // Para no pasarnos al stack
            exe.ReadDataBlock(mainMemory + physicalAddr, read, virtualAddr - exe.GetCodeSize());
        }
    }

}
#endif

#ifdef SWAP     // Swapping
bool AddressSpace::Swap(unsigned vpn) {
    for(unsigned i=0; i < numPages; i++) {
        ASSERT(pageTable[i].virtualPage == i || (int) pageTable[i].virtualPage == -1);
    }
    if (swapFile == nullptr) { // Creamos el archivo que corresponde al swap del proceso
        swapName = new char[120];
        sprintf(swapName, "SWAP.%p", currentThread->space + std::rand() % 100000);
        DEBUG('d', "Creating swap file %s\n", swapName);
        if (!fileSystem->Create(swapName, numPages * PAGE_SIZE)) {
            return false;
        }
        swapFile = fileSystem->Open(swapName);
        if (swapFile == nullptr) {
            return false;
        }
    }

    char* mainMemory = machine->mainMemory;
    unsigned virtualAddr = vpn * PAGE_SIZE;
    unsigned physicalAddr = pageTable[vpn].physicalPage * PAGE_SIZE;

    swapFile->WriteAt(mainMemory + physicalAddr, PAGE_SIZE, virtualAddr);
    pageTable[vpn].valid = false;
    pageTable[vpn].virtualPage = -1;   // Marcamos que la página se encuentra en la swap
    DEBUG('d', "Page %d saved in swap\n", vpn);

#ifdef USE_TLB
    TranslationEntry *tlb = machine->GetMMU()->tlb; 
    for (unsigned i = 0; i < TLB_SIZE; i++)
    {
      if(tlb[i].physicalPage == pageTable[vpn].physicalPage) { // limpiamos la pagina q mandamos a la swap de la tlb
        tlb[i].valid = false;
      }
    }
#endif
    return true;
}
#endif
