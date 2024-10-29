#ifndef OPEN_FILES_TABLE_HH
#define OPEN_FILES_TABLE_HH

#include "threads/lock.hh"
#include "rwLock.hh"

#define TABLE_SIZE 20

class OpenFileTableEntry {
public:
    unsigned fNode;
    unsigned links;
    bool toBeDeleted;
    RWLock* rwLock;
    Lock* infoLock;
    const char *name;
    OpenFileTableEntry(unsigned fNode, const char *name);
    ~OpenFileTableEntry();
private:

};


class OpenFilesTable {
private:
    OpenFileTableEntry** table;
    Lock* lockTable;
    
public:
    OpenFilesTable();
    ~OpenFilesTable();
    OpenFileTableEntry* OpenFile(unsigned fNode, const char *name);
    int CloseFile(unsigned fNode);
    int MarkForRemove(unsigned sector);
};


#endif // OPEN_FILES_TABLE_HH