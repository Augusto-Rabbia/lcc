/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.

#ifndef NACHOS_FILESYS_RAWFILEHEADER__HH
#define NACHOS_FILESYS_RAWFILEHEADER__HH


#include "machine/disk.hh"


static const unsigned NUM_DIRECT
    = (SECTOR_SIZE - 5 * sizeof (int)) / sizeof (int);
static const unsigned NUM_IND = SECTOR_SIZE / sizeof(int);
static const unsigned NUM_DOBLE_IND = NUM_IND * NUM_IND;
const unsigned MAX_FILE_SIZE = (NUM_DIRECT + NUM_IND + NUM_DOBLE_IND)  * SECTOR_SIZE;

struct RawFileHeader {
    unsigned numBytes;  ///< Number of bytes in the file.
    unsigned numSectors;  ///< Number of data sectors in the file.
    unsigned dataSectors[NUM_DIRECT];  ///< Disk sector numbers for each data block in the file.
    unsigned indirectionSector;          // Indireccion
    unsigned doubleIndirectionSect;      // Doble indireccion
    unsigned headerSector;
};


#endif
