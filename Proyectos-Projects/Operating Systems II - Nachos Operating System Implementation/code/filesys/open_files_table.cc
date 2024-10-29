#include "open_files_table.hh"
#include "threads/system.hh"

OpenFileTableEntry::OpenFileTableEntry(unsigned fNode, const char *name) {
    this->fNode = fNode;
    this->links = 0;
    this->toBeDeleted = false;
    this->rwLock = new RWLock("OpenFileTableEntry_RWLock");
    this->infoLock = new Lock("OpenFileTableEntry_Lock");
    this->name = name;
}

OpenFileTableEntry::~OpenFileTableEntry() {
    delete rwLock;
    delete infoLock;
}

OpenFilesTable::OpenFilesTable() {
    this->table = new OpenFileTableEntry*[TABLE_SIZE];
    this->lockTable = new Lock("OpenFilesTable_Lock");
    for (int i = 0; i < TABLE_SIZE; i++) {
        this->table[i] = nullptr;
    }
}

OpenFilesTable::~OpenFilesTable() {
    delete[] this->table;
}

OpenFileTableEntry*
OpenFilesTable::OpenFile(unsigned fNode, const char *name) {
    this->lockTable->Acquire();
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (this->table[i] == nullptr) continue;

        if (this->table[i]->fNode == fNode) {
            this->table[i]->infoLock->Acquire();
            if (this->table[i]->toBeDeleted){
                ASSERT(this->table[i]->infoLock != nullptr && this->lockTable != nullptr);
                this->table[i]->infoLock->Release();
                this->lockTable->Release();
                return nullptr;
            }

            this->table[i]->links++;
            this->table[i]->infoLock->Release();
            this->lockTable->Release();
            return this->table[i];
        }
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (this->table[i] == nullptr) {
            this->table[i] = new OpenFileTableEntry(fNode, name);
            this->table[i]->links++;
            this->lockTable->Release();
            return this->table[i];
        }
    }
    this->lockTable->Release();
    return nullptr;
}

int
OpenFilesTable::CloseFile(unsigned fNode) {
    this->lockTable->Acquire();
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (this->table[i] == nullptr) continue;

        if (this->table[i]->fNode == fNode) {
            this->table[i]->infoLock->Acquire();
            this->table[i]->links--;
            if (this->table[i]->links <= 0) {
                if (this->table[i]->toBeDeleted){
                    fileSystem->Deallocate(this->table[i]->fNode);
                }
                this->table[i]->infoLock->Release();
                this->lockTable->Release();
                this->table[i] = nullptr;
                return 1;
            }
            this->table[i]->infoLock->Release();
            this->lockTable->Release();
            return 1;
        }
    }
    
    this->lockTable->Release();
    return -1;
}

int
OpenFilesTable::MarkForRemove(unsigned fNode) {
    this->lockTable->Acquire();
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (this->table[i] == nullptr) continue;
        
        if (this->table[i]->fNode == fNode) {
            this->table[i]->infoLock->Acquire();
            this->table[i]->toBeDeleted = true;
            this->table[i]->infoLock->Release(); 
            this->lockTable->Release();
            return 1;
        }
    }
    this->lockTable->Release();
    return -1;  // File not currently open
}

