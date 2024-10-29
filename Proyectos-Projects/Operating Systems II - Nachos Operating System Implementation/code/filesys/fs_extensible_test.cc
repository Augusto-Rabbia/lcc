/// Simple test routines for the file system.
///
/// We implement:
///
/// Copy
///     Copy a file from UNIX to Nachos.
/// Print
///     Cat the contents of a Nachos file.
/// Perftest
///     A stress test for the Nachos file system read and write a really
///     really large file in tiny chunks (will not work on baseline system!)
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.

#include "file_system.hh"
#include "lib/utility.hh"
#include "machine/disk.hh"
#include "machine/statistics.hh"
#include "threads/thread.hh"
#include "threads/system.hh"

#include <stdio.h>
#include <string.h>

static const unsigned NUMBER_OF_FILES = 10;  // Make it small, just to be
                                           // difficult.

void
ExtensibleTest()
{
    char filename[10] = "TestF0\0";
    OpenFile *tFile;
    for(unsigned i = 0; i < NUMBER_OF_FILES;) {
        if(!fileSystem->Create(filename)) {
            fprintf(stderr, "Perf test: cannot create %s\n", filename);
            return;
        }
        printf("Creating %s\n", filename);
        tFile = fileSystem->Open(filename);
        tFile->Write("Test", 4);
        delete tFile;
        filename[5] = '0' + ++i;
    }
    printf("Creating %s\n", filename);
    fileSystem->Remove("TestF5");
    fileSystem->Create("TestBorr");
    tFile = fileSystem->Open("TestBorr");

    OpenFile *tFiles = fileSystem->Open("TestF9");
    
    tFile->Write("Test", 4);
    delete tFile;
    filename[5] = '7';
    if (!fileSystem->Remove(filename)) {
        printf("Perf test: unable to remove %s\n", filename);
        return;
    }
    stats->Print();
}
