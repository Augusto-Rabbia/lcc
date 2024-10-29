/// Routines to manage a directory of file names.
///
/// The directory is a table of fixed length entries; each entry represents a
/// single file, and contains the file name, and the location of the file
/// header on disk.  The fixed size of each directory entry means that we
/// have the restriction of a fixed maximum size for file names.
///
/// The constructor initializes an empty directory of a certain size; we use
/// ReadFrom/WriteBack to fetch the contents of the directory from disk, and
/// to write back any modifications back to disk.
///
/// Also, this implementation has the restriction that the size of the
/// directory cannot expand.  In other words, once all the entries in the
/// directory are used, no more files can be created.  Fixing this is one of
/// the parts to the assignment.
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.

#include "directory.hh"
#include "directory_entry.hh"
#include "file_header.hh"
#include "lib/utility.hh"

#include <stdio.h>
#include <string.h>



/// Initialize a directory; initially, the directory is completely empty.  If
/// the disk is being formatted, an empty directory is all we need, but
/// otherwise, we need to call FetchFrom in order to initialize it from disk.
///
/// * `size` is the number of entries in the directory.
Directory::Directory()
{
    raw.table = new DirectoryEntry[1];
    raw.tableSize = 1;
    raw.table[0].inUse = false;
    raw.table[0].unopenable = false;
}



/// De-allocate directory data structure.
Directory::~Directory()
{
    delete [] raw.table;
}

/// Read the contents of the directory from disk.
///
/// * `file` is file containing the directory contents.
void
Directory::FetchFrom(OpenFile *file)
{
    ASSERT(file != nullptr);

    delete[] raw.table;
    raw.tableSize = file->Length() / sizeof(DirectoryEntry);
    raw.table = new DirectoryEntry[raw.tableSize];

    // Fetch the table from disk.
    file->ReadAt((char *) raw.table,
                 raw.tableSize * sizeof (DirectoryEntry), 0);
}

/// Write any modifications to the directory back to disk.
///
/// * `file` is a file to contain the new directory contents.
bool 
Directory::WriteBack(OpenFile *file)
{
    ASSERT(file != nullptr);
    
    if (file->WriteAt((char *) raw.table, raw.tableSize * sizeof (DirectoryEntry), 0) == 0)
        return false;
    else return true;
}

/// Look up file name in directory, and return its location in the table of
/// directory entries.  Return -1 if the name is not in the directory.
///
/// * `name` is the file name to look up.
int
Directory::FindIndex(const char *name)
{
    ASSERT(name != nullptr);

    for (unsigned i = 0; i < raw.tableSize; i++) {
        if (raw.table[i].inUse
              && !strncmp(raw.table[i].name, name, FILE_NAME_MAX_LEN)) {
            return i;
        }
    }
    return -1;  // name not in directory
}

/// Look up file name in directory, and return the disk sector number where
/// the file's header is stored.  Return -1 if the name is not in the
/// directory.
///
/// * `name` is the file name to look up.
int
Directory::Find(const char *name)
{
    ASSERT(name != nullptr);

    int i = FindIndex(name);
    if (i != -1) {
        if (!raw.table[i].unopenable) {
            return raw.table[i].sector;
        }
    }
    return -1;
}

/// Add a file into the directory.  Return true if successful; return false
/// if the file name is already in the directory, or if the directory is
/// completely full, and has no more space for additional file names.
///
/// * `name` is the name of the file being added.
/// * `newSector` is the disk sector containing the added file's header.
void
Directory::Add(const char *name, int newSector, t_directoryType type)
{
    ASSERT(name != nullptr);

    for (unsigned i = 0; i < raw.tableSize; i++) {
        if (!raw.table[i].inUse) {
            raw.table[i].inUse = true;
            raw.table[i].unopenable = false;
            strncpy(raw.table[i].name, name, FILE_NAME_MAX_LEN);
            raw.table[i].sector = newSector;
            raw.table[i].isDirectory = type == T_DIRECTORY; // [P5] We indicate whether it is a directory
            return;
        }
    }

    
    raw.ResizeTable(raw.tableSize+1);
    raw.table[raw.tableSize].inUse = true;
    raw.table[raw.tableSize].unopenable = false;
    raw.table[raw.tableSize].sector = newSector;
    raw.table[raw.tableSize].isDirectory = type == T_DIRECTORY; // [P5] We indicate whether it is a directory
    strncpy(raw.table[raw.tableSize].name, name, FILE_NAME_MAX_LEN);
    raw.tableSize++;
    return;
}

void
RawDirectory::ResizeTable(unsigned newSize) {
    DirectoryEntry *newTable = new DirectoryEntry[newSize];
    for (unsigned i = 0; i < tableSize; i++) {
        newTable[i].inUse = table[i].inUse;
        newTable[i].unopenable = table[i].unopenable;
        strncpy(newTable[i].name, table[i].name, FILE_NAME_MAX_LEN);
        newTable[i].sector = table[i].sector;
        newTable[i].isDirectory = table[i].isDirectory;
    }
    delete[] table;
    table = newTable;

}

/// Remove a file name from the directory.   Return true if successful;
/// return false if the file is not in the directory.
///
/// * `name` is the file name to be removed.
void
Directory::Remove(int index)
{
    ASSERT(index > 1);

    raw.table[index].inUse = false;
    raw.table[index].isDirectory = false;
}

/// List all the file names in the directory.
void
Directory::List() const
{
    for (unsigned i = 0; i < raw.tableSize; i++) {
        if (raw.table[i].inUse) {
            printf("%s\n", raw.table[i].name);
        }
    }
}

/// List all the file names in the directory, their `FileHeader` locations,
/// and the contents of each file.  For debugging.
void
Directory::Print() const
{
    FileHeader *hdr = new FileHeader;
    DEBUG('w', "raw.tableSize %d\n", raw.tableSize);
                    
    printf("Directory contents:\n");
    for (unsigned i = 0; i < raw.tableSize; i++) {
        printf("\n\n===============================================\n");
        printf("===============================================\n");
        printf("===============================================\n");
        if (raw.table[i].inUse) {
            printf("\nDirectory entry:\n"
                "    name: %s\n"
                "    sector: %u\n"
                "    isDirectory?: %d\n",
                raw.table[i].name, raw.table[i].sector, raw.table[i].isDirectory == true);
            hdr->FetchFrom(raw.table[i].sector);
            hdr->Print(raw.table[i].name);
            if (raw.table[i].isDirectory && strcmp(raw.table[i].name, ".") && strcmp(raw.table[i].name, "..")) {
                OpenFile *file = new OpenFile(raw.table[i].sector, raw.table[i].name);
                Directory* dir = new Directory();
                dir->FetchFrom(file);
                dir->Print();
                delete file;
                delete dir;
            }

        printf("===============================================\n");
        printf("===============================================\n");
        printf("===============================================\n\n");
        }
    }
    printf("\n");
    delete hdr;
}

const RawDirectory *
Directory::GetRaw() const
{
    return &raw;
}

void
Directory::SetUnopenable(int index)
{
    ASSERT(index >= 0);
    raw.table[index].unopenable = true;
}
