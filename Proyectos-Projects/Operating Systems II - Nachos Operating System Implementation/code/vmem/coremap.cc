#include "coremap.hh"
#include <limits.h>
#include "threads/system.hh"
#include <cstdlib>

#ifdef SWAP
Coremap::Coremap(unsigned numPages_)
{
    numPages = numPages_;
    freePhys = new Bitmap(numPages);
    usedPhys = new List<unsigned>();
    used = new Entry[numPages];
    listsz= 0;
}

Coremap::~Coremap()
{
    delete freePhys;
    delete usedPhys;
    delete [] used;
}

unsigned
Coremap::Find_with_Replacement(unsigned virtualPage) {
    unsigned page = freePhys->Find();
    if((int) page < 0) {       // Bitmap esta lleno, hacemos reemplazo de páginas
        page = PickVictim(); // Pagina física a ser swappeada
        DEBUG('d', "Victim process: %s\n",used[page].process->GetName());
        DEBUG('d', "The physical page is %d and the virtual page is %d\n", page, used[page].virtualPage);
        ASSERT(used[page].process->space != nullptr);
        used[page].process->space->Swap(used[page].virtualPage); // Swappeamos la página física elegida, según su página virtual
    }
    used[page].process = currentThread;
    used[page].virtualPage = virtualPage;
    usedPhys->Append(page);
    listsz++;
    return page;
}

void
Coremap::Clear(unsigned physicalPage){

    if(!freePhys->Test(physicalPage)){
       return;
    }
    listsz--;
    used[physicalPage].process = nullptr;
    usedPhys->Remove(physicalPage);
    freePhys->Clear(physicalPage);
}

void Coremap::Get(unsigned physicalPage){
    if(!freePhys->Test(physicalPage)){
       return;
    }
    return;
}

unsigned
Coremap::PickVictim(){
#ifdef PRPOLICY_FIFO
    listsz--;
    return usedPhys->Pop();
#else
#ifdef PRPOLICY_CLOCK
    int vict = GoThroughList(false); // Buscamos (0,0)
    if(vict != -1)
        return (unsigned)vict;
    vict = GoThroughList(true);      // Buscamos (0,1) y apagamos bit de referencia
    if (vict != -1) {
        return (unsigned)vict;
    }
    vict = GoThroughList(false);    // Buscamos (0, 0) nuevamente
    if (vict != -1) {
        return (unsigned)vict;
    }
    vict= usedPhys->Pop();
    listsz--;
    return (unsigned) vict;         // Devolvemos el primero
#else
    int vict = std::rand() % listsz;
    listsz--;
    return usedPhys->PopN(vict);
#endif
#endif
}

#ifdef PRPOLICY_CLOCK
int
Coremap::GoThroughList(bool TurnOffUseBit) {
    int victim;
    TranslationEntry* entry;
    if (TurnOffUseBit) {
        for(unsigned i = 0; i < listsz; i++) {
            victim = usedPhys->Pop();
            entry = &(currentThread->space->GetPageTable()[used[victim].virtualPage]);
            if ((entry->dirty) && !(entry->use)) { // El marco está en (0,1)
                entry->use = false;
                listsz--;
                return victim;
            }
            entry->use = false;
            usedPhys->Append(victim);    // Lo metemos al final de la lista, para que el puntero apunte al próximo marco
        }
        return -1;
    }
    else {
        for(unsigned i = 0; i < listsz; i++){
            victim = usedPhys->Pop();
            entry = &(currentThread->space->GetPageTable()[used[victim].virtualPage]);
            if (!(entry->dirty) && !(entry->use)){
                listsz--;

                return victim;
            }
            usedPhys->Append(victim);
        }
        return -1;
    }
}
#endif

#endif