#ifndef NACHOS_VMEM_COREMAP__HH
#define NACHOS_VMEM_COREMAP__HH

#include "lib/bitmap.hh"
#include "threads/thread.hh"
#include "lib/list.hh"
#ifdef SWAP

class Entry
{
public:
   unsigned virtualPage;    // Pag virtual que aloja
   Thread* process;         // Espacio de direcciones (proceso) al que pertenece
};


class Coremap
{
public:
    Coremap(unsigned numPages_);
    ~Coremap();
    unsigned Find_with_Replacement(unsigned virtualPage);
    void Clear(unsigned physicalPage);
    void Get(unsigned physicalPage);
    unsigned PickVictim();
#ifdef PRPOLICY_CLOCK
    int GoThroughList(bool TurnOffDirtyBit);
#endif
private:
    Bitmap *freePhys;          // Bitmap
    Entry* used;                // Marco ocupado
    List<unsigned>* usedPhys;   // Lista de marcos ocupados
    unsigned listsz;            // Tamanio de la lista 

    unsigned numPages;
};
#endif
#endif