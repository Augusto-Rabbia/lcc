/// Routines to manage the overall operation of the file system.  Implements
/// routines to map from textual file names to files.
///
/// Each file in the file system has:
/// * a file header, stored in a sector on disk (the size of the file header
///   data structure is arranged to be precisely the size of 1 disk sector);
/// * a number of data blocks;
/// * an entry in the file system directory.
///
/// The file system consists of several data structures:
/// * A bitmap of free disk sectors (cf. `bitmap.h`).
/// * A directory of file names and file headers.
///
/// Both the bitmap and the directory are represented as normal files.  Their
/// file headers are located in specific sectors (sector 0 and sector 1), so
/// that the file system can find them on bootup.
///
/// The file system assumes that the bitmap and directory files are kept
/// “open” continuously while Nachos is running.
///
/// For those operations (such as `Create`, `Remove`) that modify the
/// directory and/or bitmap, if the operation succeeds, the changes are
/// written immediately back to disk (the two files are kept open during all
/// this time).  If the operation fails, and we have modified part of the
/// directory and/or bitmap, we simply discard the changed version, without
/// writing it back to disk.
///
/// Our implementation at this point has the following restrictions:
///
/// * there is no synchronization for concurrent accesses;
/// * files have a fixed size, set when the file is created;
/// * files cannot be bigger than about 3KB in size;
/// * there is no hierarchical directory structure, and only a limited number
///   of files can be added to the system;
/// * there is no attempt to make the system robust to failures (if Nachos
///   exits in the middle of an operation that modifies the file system, it
///   may corrupt the disk).
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.

#include "file_system.hh"
#include "file_header.hh"
#include "lib/bitmap.hh"
#include "threads/system.hh"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libgen.h> // https://pubs.opengroup.org/onlinepubs/000095399/basedefs/libgen.h.html

/// Sectors containing the file headers for the bitmap of free sectors, and
/// the directory of files.  These file headers are placed in well-known
/// sectors, so that they can be located on boot-up.
static const unsigned FREE_MAP_SECTOR = 0;
static const unsigned DIRECTORY_SECTOR = 1;

int getDirectorySector(const char* path);
int pathToSector(const char *path);


char* getBasename(const char *path) {
    char* dup = strdup(path);
    char* base = strdup(basename(dup)); // https://pubs.opengroup.org/onlinepubs/000095399/functions/basename.html
    free(dup);
    return base;
}

char* getDirectoryName(const char *path) {
    char* dup = strdup(path);
    char* dir = strdup(dirname(dup)); // https://pubs.opengroup.org/onlinepubs/000095399/functions/dirname.html
    free(dup);
    return dir;
}

int getDirectorySector(const char* path) {
    char* dirname = getBasename(path);
    int dirSector = pathToSector(dirname);
    free(dirname);
    return dirSector;
}

int pathToSector(const char *path) {
    List<char*>* pathTrail = new List<char*>;
    char* dup = strdup(path);
    char* base = dup;
    char* dir = dup;
    unsigned depth = 0;

    while (strcmp(dir, "/") != 0 && strcmp(dir, ".") != 0) {
        base = getBasename(dir);
        dir = dirname(dir);
        pathTrail->Append(base);
        depth += 1;
    }

    int sector = dir[0] == '/' ? DIRECTORY_SECTOR : currentThread->workingDirectory;

    for (unsigned i = 0; i < depth; i += 1) {
        char* component = pathTrail->Pop();
        if (sector != -1) {
            Directory* directory = new Directory();
            OpenFile* d = new OpenFile(sector, component);
            directory->FetchFrom(d);
            sector = directory->Find(component);
            delete d;
            delete directory;
        }
        delete component;
    }
    
    
    delete pathTrail;
    free(dup);

    return sector;
}





/// Initialize the file system.  If `format == true`, the disk has nothing on
/// it, and we need to initialize the disk to contain an empty directory, and
/// a bitmap of free sectors (with almost but not all of the sectors marked
/// as free).
///
/// If `format == false`, we just have to open the files representing the
/// bitmap and the directory.
///
/// * `format` -- should we initialize the disk?
FileSystem::FileSystem(bool format)
{
    this->rwLock = new RWLock("File System Lock");
    DEBUG('f', "Initializing the file system.\n");
    if (format) {
        Bitmap     *freeMap = new Bitmap(NUM_SECTORS);
        Directory  *dir     = new Directory();
        FileHeader *mapH    = new FileHeader;
        FileHeader *dirH    = new FileHeader;

        

        DEBUG('f', "Formatting the file system.\n");

        ASSERT(this->rwLock != nullptr);
        // First, allocate space for FileHeaders for the directory and bitmap
        // (make sure no one else grabs these!)
        freeMap->Mark(FREE_MAP_SECTOR);
        freeMap->Mark(DIRECTORY_SECTOR);

        // Second, allocate space for the data blocks containing the contents
        // of the directory and bitmap files.  There better be enough space!

        ASSERT(mapH->Allocate(freeMap, FREE_MAP_SECTOR, FREE_MAP_FILE_SIZE));
        ASSERT(dirH->Allocate(freeMap, DIRECTORY_SECTOR, sizeof(DirectoryEntry)*2));
      
      
        // Flush the bitmap and directory `FileHeader`s back to disk.
        // We need to do this before we can `Open` the file, since open reads
        // the file header off of disk (and currently the disk has garbage on
        // it!).

        DEBUG('f', "Writing headers back to disk.\n");
        dirH->WriteBack(DIRECTORY_SECTOR);
        mapH->WriteBack(FREE_MAP_SECTOR);
        

        // OK to open the bitmap and directory files now.
        // The file system operations assume these two files are left open
        // while Nachos is running.

        freeMapFile   = new OpenFile(FREE_MAP_SECTOR, "FreeMapFile");
        directoryFile = new OpenFile(DIRECTORY_SECTOR, "RootDirFile");

        // Once we have the files “open”, we can write the initial version of
        // each file back to disk.  The directory at this point is completely
        // empty; but the bitmap has been changed to reflect the fact that
        // sectors on the disk have been allocated for the file headers and
        // to hold the file data for the directory and bitmap.

        DEBUG('f', "Writing bitmap and directory back to disk.\n");
        freeMap->WriteBack(freeMapFile);     // flush changes to disk

        // [P5] Add the "." (current dir) and ".." (parent directory, current in this case though) directories to the root directory.
        dir->Add(".", DIRECTORY_SECTOR, T_DIRECTORY);
        DEBUG('5', "'.' folder added to the root directory.\n");
        dir->Add("..", DIRECTORY_SECTOR, T_DIRECTORY);
        DEBUG('5', "'..' folder added to the root directory.\n");

        dir->WriteBack(directoryFile);
        
        if (debug.IsEnabled('f')) {
            freeMap->Print();
            dir->Print();

            delete freeMap;
            delete dir;
            delete mapH;
            delete dirH;
        }
    } else {
        // If we are not formatting the disk, just open the files
        // representing the bitmap and directory; these are left open while
        // Nachos is running.
        freeMapFile   = new OpenFile(FREE_MAP_SECTOR, "FreeMapFile");
        directoryFile = new OpenFile(DIRECTORY_SECTOR, "RootDirFile");
    }
}

FileSystem::~FileSystem()
{
    delete freeMapFile;
    delete directoryFile;
    delete rwLock;
}

/// Create a file in the Nachos file system (similar to UNIX `create`).
/// Since we cannot increase the size of files dynamically, we have to give
/// `Create` the initial size of the file.
///
/// The steps to create a file are:
/// 1. Make sure the file does not already exist.
/// 2. Allocate a sector for the file header.
/// 3. Allocate space on disk for the data blocks for the file.
/// 4. Add the name to the directory.
/// 5. Store the new file header on disk.
/// 6. Flush the changes to the bitmap and the directory back to disk.
///
/// Return true if everything goes ok, otherwise, return false.
///
/// Create fails if:
/// * file is already in directory;
/// * no free space for file header;
/// * no free entry for file in directory;
/// * no free space for data blocks for the file.
///
/// Note that this implementation assumes there is no concurrent access to
/// the file system!
///
/// * `name` is the name of file to be created.
/// * `initialSize` is the size of file to be created.
bool
FileSystem::Create(const char *path, unsigned initialSize, t_directoryType type)
{
    ASSERT(path != nullptr);
    ASSERT(initialSize < MAX_FILE_SIZE);



    if (type == T_DIRECTORY) {
        return CreateDirectory(path, initialSize);
    }

    DEBUG('w', "Creating file %s, size %u\n", path, initialSize);

    rwLock->AcquireWriter();

    Directory *dir = new Directory();
    char* name = getBasename(path);
    char* dirname = getDirectoryName(path);
    int dirSector = pathToSector(dirname);

    DEBUG('5', "\n------------------------\n");
    DEBUG('5', "--- ADDING FILE ---\n");
    DEBUG('5', "path = %s\n", path);
    DEBUG('5', "name = %s\n", name);
    DEBUG('5', "dirname = %s\n", dirname);
    DEBUG('5', "dirSector = %d\n", dirSector);
    

    if (dirSector == -1) {
        DEBUG('f', "Directory not found\n");
        delete dir;
        rwLock->ReleaseWriter();
        return false;
    }

    OpenFile *dirFile = new OpenFile(dirSector, dirname);
    dir->FetchFrom(dirFile);
    
    bool success;

    if (dir->Find(name) != -1) {
        success = false;  // File is already in directory.
    } else {
        Bitmap *freeMap = new Bitmap(NUM_SECTORS);
        Bitmap *freeMapOriginal = new Bitmap(NUM_SECTORS);
        freeMap->FetchFrom(freeMapFile);
        int sector = freeMap->Find();
          // Find a sector to hold the file header.
        if (sector == -1) {
            success = false;  // No free block for file header.
        } else {
            dir->Add(name, sector);
            FileHeader *h = new FileHeader;
            if(initialSize == 0) {
                h->Initialize(sector);
                DEBUG('w', "Initialized file header with numBytes = %d\n", h->GetRaw()->numBytes);
                success = h->Allocate(freeMap, sector, initialSize);
            } else {
                success = h->Allocate(freeMap, sector, initialSize);
            }
            
            // Fails if no space on disk for data.
            DEBUG('x', "success = %d\n", success);
            if (success) {
                // Everything worked, flush all changes back to disk.
                freeMap->WriteBack(freeMapFile);
                if (!dir->WriteBack(dirFile)) {                   // Usa el freemap, por lo que debemos actualizarlo antes
                    freeMapOriginal->WriteBack(freeMapFile);         // pero si falla, volvemos a la versión original
                    success = false;
                }else {
                    h->WriteBack(sector);
                }
            }
            delete h;
        }
        delete freeMapOriginal;
        delete freeMap;
    }
    delete dirFile;
    delete dir;
    rwLock->ReleaseWriter();

    DEBUG('5', "------------------------\n\n");
    return success;
}



bool
FileSystem::CreateDirectory(const char *path, unsigned initialSize) {
/*
   █████████                                                █████  ███                               █████                        ███          
  ███░░░░░███                                              ░░███  ░░░                               ░░███                        ░░░           
 ███     ░░░  ████████   ██████   ██████   ████████      ███████  ████  ████████   ██████   ██████  ███████    ██████  ████████  ████   ██████ 
░███         ░░███░░███ ███░░███ ░░░░░███ ░░███░░███    ███░░███ ░░███ ░░███░░███ ███░░███ ███░░███░░░███░    ███░░███░░███░░███░░███  ███░░███
░███          ░███ ░░░ ░███████   ███████  ░███ ░░░    ░███ ░███  ░███  ░███ ░░░ ░███████ ░███ ░░░   ░███    ░███ ░███ ░███ ░░░  ░███ ░███ ░███
░░███     ███ ░███     ░███░░░   ███░░███  ░███        ░███ ░███  ░███  ░███     ░███░░░  ░███  ███  ░███ ███░███ ░███ ░███      ░███ ░███ ░███
 ░░█████████  █████    ░░██████ ░░████████ █████       ░░████████ █████ █████    ░░██████ ░░██████   ░░█████ ░░██████  █████     █████░░██████ 
  ░░░░░░░░░  ░░░░░      ░░░░░░   ░░░░░░░░ ░░░░░         ░░░░░░░░ ░░░░░ ░░░░░      ░░░░░░   ░░░░░░     ░░░░░   ░░░░░░  ░░░░░     ░░░░░  ░░░░░░  
                                                                                                                                                                                                                                                       
*/
    rwLock->AcquireWriter();

    char* name = getBasename(path);
    char* parentDirName = getDirectoryName(path);
    int parentDirSector = pathToSector(parentDirName);

    DEBUG('5', "\n------------------------\n");
    DEBUG('5', "--- ADDING FOLDER ---\n");
    DEBUG('5', "path = %s\n", path);
    DEBUG('5', "name = %s\n", name);
    DEBUG('5', "parentDirName = %s\n", parentDirName);
    DEBUG('5', "parentDirSector = %d\n", parentDirSector);
    

    if (parentDirSector == -1) {
        DEBUG('f', "Directory not found\n");
        rwLock->ReleaseWriter();
        return false;
    }

    Directory *parentDir = new Directory();
    OpenFile* parentDirFile = new OpenFile(parentDirSector, parentDirName);
    parentDir->FetchFrom(parentDirFile);

    if (parentDir->Find(name) != -1) {
        // Directory already exists in parent directory.
        delete parentDir;
        delete name;
        rwLock->ReleaseWriter();
        return false;
    }
    
    Bitmap *freeMap = new Bitmap(NUM_SECTORS);
    freeMap->FetchFrom(freeMapFile);

    int sector = freeMap->Find();
    if (sector == -1) {
        // Out of space on disk to create header.
        delete freeMap;
        delete parentDir;
        delete name;
        rwLock->ReleaseWriter();
        return false;
    }

    FileHeader *header = new FileHeader;
    header->Initialize(sector);
    header->Allocate(freeMap, sector, initialSize);
    
    header->WriteBack(sector);
    freeMap->WriteBack(freeMapFile);

    parentDir->Add(name, sector, T_DIRECTORY); // Add directory to parent directory's entries.

    // Create new directory
    OpenFile* dirFile = new OpenFile(sector, name);
    Directory *directory = new Directory();
    DEBUG('5', "Adding folder .-> %d and .. -> %d to directory in sector : %d\n", sector, parentDirSector, sector);

    directory->Add(".", sector, T_DIRECTORY);
    directory->Add("..", parentDirSector, T_DIRECTORY);


    directory->WriteBack(dirFile);


    delete dirFile;
    delete directory;
    
    parentDir->WriteBack(parentDirFile);
    delete parentDirFile;
    delete parentDir;

    rwLock->ReleaseWriter();
    DEBUG('5', "------------------------\n\n");

    return true;
}



bool
FileSystem::ExtendFile(OpenFile* file, unsigned newSize)
{
    ASSERT(file != nullptr);

    Bitmap *freeMap = new Bitmap(NUM_SECTORS);
    FileHeader *header = new FileHeader;
    header->FetchFrom(file->GetHeader()->GetRaw()->headerSector);

    freeMap->FetchFrom(freeMapFile);
    
    bool result = header->Extend(freeMap, newSize);
    header->GetRaw()->numBytes = newSize;
    
    header->WriteBack(header->GetRaw()->headerSector);
    
    freeMap->WriteBack(freeMapFile);
    
    
    delete freeMap;
    delete header;
    return result;
}

/// Open a file for reading and writing.
///
/// To open a file:
/// 1. Find the location of the file's header, using the directory.
/// 2. Bring the header into memory.
///
/// * `name` is the text name of the file to be opened.
OpenFile *
FileSystem::Open(const char *path)
{
    ASSERT(path != nullptr);
    rwLock->AcquireReader();

    Directory *dir = new Directory();

    DEBUG('f', "Opening file %s\n", path);
    char* name = getBasename(path);
    char* pathParent = getDirectoryName(path);
    int dirSector = pathToSector(pathParent);
    if (dirSector == -1) {
        DEBUG('w', "Path parent directory not found\n");
        rwLock->ReleaseReader();
        delete dir;
        return nullptr;  // Return null if not found.
    }

    DEBUG('6', "------\n");
    DEBUG('6', "path %s\n", path);
    DEBUG('6', "name %s\n", name);
    DEBUG('6', "pathParent %s\n", pathParent);
    DEBUG('6', "dirSector %d\n", dirSector);
    DEBUG('6', "------\n");
    
    OpenFile* parentDir = new OpenFile(dirSector, pathParent);
    
    dir->FetchFrom(parentDir);
    
    delete parentDir;
    int sector = dir->Find(name);
    
    OpenFile *openFile = nullptr;
    
    if (sector >= 0) {
        openFile = new OpenFile(sector, name);
    }
    free(name);
    free(pathParent);
    delete dir;
    rwLock->ReleaseReader();
    
    DEBUG('w', "Found file %s in sector %d.\n", path, sector);
    return openFile;  // Return null if not found.
}

/// Delete a file from the file system.
///
/// This requires:
/// 1. Remove it from the directory.
/// 2. Delete the space for its header.
/// 3. Delete the space for its data blocks.
/// 4. Write changes to directory, bitmap back to disk.
///
/// Return true if the file was deleted, false if the file was not in the
/// file system.
///
/// * `name` is the text name of the file to be removed.
bool
FileSystem::Remove(const char *path)
{
    ASSERT(path != nullptr);
    rwLock->AcquireWriter();
    Directory *dir = new Directory();

    DEBUG('f', "Removing file %s\n", path);
    char* name = getBasename(path);
    char* pathParent = getDirectoryName(path);
    int dirSector = pathToSector(pathParent);
    bool success = true;
    DEBUG('7', "------\n");
    DEBUG('7', "path %s\n", path);
    DEBUG('7', "name %s\n", name);
    DEBUG('7', "pathParent %s\n", pathParent);
    DEBUG('7', "dirSector %d\n", dirSector);
    DEBUG('7', "------\n");

    OpenFile* dirFile = new OpenFile(dirSector, pathParent);

    dir->FetchFrom(dirFile);
    
    int idx = dir->FindIndex(name);
    if (idx == -1) {
        success = false;
    } else { 
        Remove_with_lock(dir, dirFile, idx);
    }
    delete dir;
    delete dirFile;
    rwLock->ReleaseWriter();
    return success;
}
void
FileSystem::Remove_with_lock(Directory* dir, OpenFile* dirFile, int idx) {
    
    
    int sector = dir->GetRaw()->table[idx].sector;
    if (dir->GetRaw()->table[idx].isDirectory) {

        OpenFile *file = new OpenFile(sector, dir->GetRaw()->table[idx].name);
        ASSERT(file != nullptr);
        Directory* subDir = new Directory();
        subDir->FetchFrom(file);
        
        for (unsigned j = 2; j < subDir->GetRaw()->tableSize; j++) { // Skip "." and ".."
            if (subDir->GetRaw()->table[j].inUse) {
                fileSystem->Remove_with_lock(subDir, file, (int) j);
            }
        }

        delete subDir;
        delete file;
    }
    dir->Remove(idx);
    dir->WriteBack(dirFile);

    if (openFilesTable->MarkForRemove(sector) == -1) { // File not currently open
        Deallocate(sector);
    } else {
        dir->SetUnopenable(idx);
        dir->WriteBack(dirFile);
    }
  
    
}


void
FileSystem::Deallocate(int sector) {
    bool yaTomado;
    if(!(yaTomado = rwLock->lck->IsHeldByCurrentThread())) {
        rwLock->AcquireWriter();
    }
    

    FileHeader *fileH = new FileHeader;
    fileH->FetchFrom(sector);

    Bitmap *freeMap = new Bitmap(NUM_SECTORS);
    freeMap->FetchFrom(freeMapFile);

    fileH->Deallocate(freeMap);  // Remove data blocks.
    freeMap->Clear(sector);      // Remove header block.
    
    freeMap->WriteBack(freeMapFile);  // Flush to disk.
    
    delete fileH;
    delete freeMap;
    if (!yaTomado) {
        rwLock->ReleaseWriter();
    }
}


/// List all the files in the file system directory.
void
FileSystem::List()
{   
    rwLock->AcquireReader();
    Directory *dir = new Directory();

    dir->FetchFrom(directoryFile);
    dir->List();
    delete dir;
    rwLock->ReleaseReader();
}

static bool
AddToShadowBitmap(unsigned sector, Bitmap *map)
{
    ASSERT(map != nullptr);

    if (map->Test(sector)) {
        DEBUG('f', "Sector %u was already marked.\n", sector);
        return false;
    }
    map->Mark(sector);
    DEBUG('f', "Marked sector %u.\n", sector);
    return true;
}

static bool
CheckForError(bool value, const char *message)
{
    if (!value) {
        DEBUG('f', "Error: %s\n", message);
    }
    return !value;
}

static bool
CheckSector(unsigned sector, Bitmap *shadowMap)
{
    if (CheckForError(sector < NUM_SECTORS,
                      "sector number too big.  Skipping bitmap check.")) {
        return true;
    }
    return CheckForError(AddToShadowBitmap(sector, shadowMap),
                         "sector number already used.");
}

static bool
CheckFileHeader(const RawFileHeader *rh, unsigned num, Bitmap *shadowMap)
{
    ASSERT(rh != nullptr);

    bool error = false;

    DEBUG('f', "Checking file header %u.  File size: %u bytes, number of sectors: %u.\n",
          num, rh->numBytes, rh->numSectors);
    error |= CheckForError(rh->numSectors >= DivRoundUp(rh->numBytes,
                                                        SECTOR_SIZE),
                           "sector count not compatible with file size.");
    error |= CheckForError(rh->numSectors < NUM_DIRECT,
                           "too many blocks.");
    for (unsigned i = 0; i < rh->numSectors; i++) {
        unsigned s = rh->dataSectors[i];
        error |= CheckSector(s, shadowMap);
    }
    return error;
}

static bool
CheckBitmaps(const Bitmap *freeMap, const Bitmap *shadowMap)
{
    bool error = false;
    for (unsigned i = 0; i < NUM_SECTORS; i++) {
        DEBUG('f', "Checking sector %u. Original: %u, shadow: %u.\n",
              i, freeMap->Test(i), shadowMap->Test(i));
        error |= CheckForError(freeMap->Test(i) == shadowMap->Test(i),
                               "inconsistent bitmap.");
    }
    return error;
}

static bool
CheckDirectory(const RawDirectory *rd, Bitmap *shadowMap)
{
    ASSERT(rd != nullptr);
    ASSERT(shadowMap != nullptr);

    bool error = false;
    unsigned nameCount = 0;
    const char *knownNames[NUM_DIR_ENTRIES];

    for (unsigned i = 0; i < NUM_DIR_ENTRIES; i++) {
        DEBUG('f', "Checking direntry: %u.\n", i);
        const DirectoryEntry *e = &rd->table[i];

        if (e->inUse) {
            if (strlen(e->name) > FILE_NAME_MAX_LEN) {
                DEBUG('f', "Filename too long.\n");
                error = true;
            }

            // Check for repeated filenames.
            DEBUG('f', "Checking for repeated names.  Name count: %u.\n",
                  nameCount);
            bool repeated = false;
            for (unsigned j = 0; j < nameCount; j++) {
                DEBUG('f', "Comparing \"%s\" and \"%s\".\n",
                      knownNames[j], e->name);
                if (strcmp(knownNames[j], e->name) == 0) {
                    DEBUG('f', "Repeated filename.\n");
                    repeated = true;
                    error = true;
                }
            }
            if (!repeated) {
                knownNames[nameCount] = e->name;
                DEBUG('f', "Added \"%s\" at %u.\n", e->name, nameCount);
                nameCount++;
            }

            // Check sector.
            error |= CheckSector(e->sector, shadowMap);

            // Check file header.
            FileHeader *h = new FileHeader;
            const RawFileHeader *rh = h->GetRaw();
            h->FetchFrom(e->sector);
            error |= CheckFileHeader(rh, e->sector, shadowMap);
            delete h;
        }
    }
    return error;
}

bool
FileSystem::Check()
{
    DEBUG('f', "Performing filesystem check\n");
    bool error = false;

    rwLock->AcquireReader();


    Bitmap *shadowMap = new Bitmap(NUM_SECTORS);
    shadowMap->Mark(FREE_MAP_SECTOR);
    shadowMap->Mark(DIRECTORY_SECTOR);

    DEBUG('f', "Checking bitmap's file header.\n");

    FileHeader *bitH = new FileHeader;
    const RawFileHeader *bitRH = bitH->GetRaw();
    bitH->FetchFrom(FREE_MAP_SECTOR);
    DEBUG('f', "  File size: %u bytes, expected %u bytes.\n"
               "  Number of sectors: %u, expected %u.\n",
          bitRH->numBytes, FREE_MAP_FILE_SIZE,
          bitRH->numSectors, FREE_MAP_FILE_SIZE / SECTOR_SIZE);
    error |= CheckForError(bitRH->numBytes == FREE_MAP_FILE_SIZE,
                           "bad bitmap header: wrong file size.");
    error |= CheckForError(bitRH->numSectors == FREE_MAP_FILE_SIZE / SECTOR_SIZE,
                           "bad bitmap header: wrong number of sectors.");
    error |= CheckFileHeader(bitRH, FREE_MAP_SECTOR, shadowMap);
    delete bitH;

    DEBUG('f', "Checking directory.\n");

    FileHeader *dirH = new FileHeader;
    const RawFileHeader *dirRH = dirH->GetRaw();
    dirH->FetchFrom(DIRECTORY_SECTOR);
    error |= CheckFileHeader(dirRH, DIRECTORY_SECTOR, shadowMap);
    delete dirH;

    Bitmap *freeMap = new Bitmap(NUM_SECTORS);
    freeMap->FetchFrom(freeMapFile);
    Directory *dir = new Directory();
    const RawDirectory *rdir = dir->GetRaw();
    dir->FetchFrom(directoryFile);
    error |= CheckDirectory(rdir, shadowMap);
    delete dir;

    // The two bitmaps should match.
    DEBUG('f', "Checking bitmap consistency.\n");
    error |= CheckBitmaps(freeMap, shadowMap);
    delete shadowMap;
    delete freeMap;

    DEBUG('f', error ? "Filesystem check failed.\n"
                     : "Filesystem check succeeded.\n");

    rwLock->ReleaseReader();
    return !error;

}

/// Print everything about the file system:
/// * the contents of the bitmap;
/// * the contents of the directory;
/// * for each file in the directory:
///   * the contents of the file header;
///   * the data in the file.
void
FileSystem::Print()
{
    FileHeader *bitH    = new FileHeader;
    FileHeader *dirH    = new FileHeader;
    Bitmap     *freeMap = new Bitmap(NUM_SECTORS);
    Directory  *dir     = new Directory();

    ASSERT(this->rwLock != nullptr);
    this->rwLock->AcquireReader();

    printf("--------------------------------\n");
    bitH->FetchFrom(FREE_MAP_SECTOR);
    bitH->Print("Bitmap");

    printf("--------------------------------\n");
    dirH->FetchFrom(DIRECTORY_SECTOR);
    dirH->Print("Directory");

    printf("--------------------------------\n");
    freeMap->FetchFrom(freeMapFile);
    freeMap->Print();

    printf("--------------------------------\n");
    dir->FetchFrom(directoryFile);
    dir->Print();
    printf("--------------------------------\n");

    delete bitH;
    delete dirH;
    delete freeMap;
    delete dir;
    this->rwLock->ReleaseReader();
}
