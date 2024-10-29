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

static const unsigned NUMBER_OF_FILES = 10;  // Make it more smaller, just to be
                                           // even difficulter.

void
FilesysPathTest()
{
    DEBUG('5', "FilesysPathTest Initialized\n");
    char dir[10] = "/testdir0";
    // Create directories
    for(unsigned i = 0; i < NUMBER_OF_FILES;) {
        printf("Creating directory %s\n", dir);
        if(!fileSystem->Create(dir, 0,T_DIRECTORY)) {
            fprintf(stderr, "Perf test: cannot create %s\n", dir);
            return;
        }
        dir[8] = '0' + ++i;
    }

    char path[16] = "/testdir0/File0";
    printf("Creating file %s\n", path);
    if(!fileSystem->Create(path)) {
        fprintf(stderr, "Perf test: cannot create %s\n", path);
        return;
    }
    printf("Opening file\n");
    OpenFile *tFile = fileSystem->Open(path);
    
    if (tFile == nullptr) {
        fprintf(stderr, "Path test: cannot open %s\n", path);
    }else {

        char str[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi sit amet pulvinar sem, vel sollicitudin mauris. Phasellus pharetra pulvinar odio, laoreet eleifend mi rhoncus tincidunt. Morbi mattis velit id nibh venenatis, at tempor tellus pulvinar. Sed at dapibus felis. Aenean efficitur, magna non fringilla sodales, mauris ex sagittis enim, vitae semper purus urna sit amet sem. Duis a augue vitae nisi interdum hendrerit. Sed et pulvinar magna, sed condimentum augue. Nulla facilisi. Suspendisse massa sem, sollicitudin eget interdum ac, placerat in lacus. Aliquam et interdum elit, at vehicula lacus. Donec egestas enim et lectus ornare bibendum. Nam ultrices odio vitae tempor sodales. Phasellus vehicula lectus in neque interdum varius. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vivamus rhoncus massa quis diam lacinia, vitae bibendum augue pellentesque. Nunc euismod eleifend mi at maximus. Fusce enim est, placerat non dui nec, efficitur consectetur nulla. Aliquam massa leo, semper sit amet sagittis nec, consectetur ut tellus. Aenean pretium auctor rhoncus. Cras tortor ex, commodo sed diam ac, tincidunt semper purus. Aenean pharetra, ex vitae vulputate malesuada, est nunc bibendum elit, nec vestibulum nisi sapien in ligula. Donec semper quam eget interdum gravida. Aenean blandit nunc quis metus rhoncus, vitae semper velit egestas. In porta, ipsum ac tincidunt volutpat, erat elit dapibus est, in porta ligula orci sed orci. Nulla facilisi. Aenean a nisi in enim suscipit consequat. Quisque fermentum, lorem ut sodales dictum, velit arcu sagittis sapien, a semper metus elit eget nunc. Pellentesque felis mauris, rutrum nec accumsan sit amet, euismod a ex. Donec ipsum metus, aliquam et odio eu, auctor efficitur nunc. Nam id eleifend ante. Curabitur accumsan nisl sed nisi congue, eget tempus odio venenatis. Curabitur diam massa, ornare sit amet purus nec, viverra fringilla mauris. Etiam ac enim ornare nisi semper tincidunt. Sed malesuada dolor libero, sit amet laoreet lorem lobortis at. Sed sapien dui, consequat eget ex eget, aliquam porta elit. Integer vehicula volutpat justo, in porta tellus aliquet in. Phasellus sed nibh sed ipsum gravida aliquam vitae vitae metus.";

        printf("Writing file\n");
        tFile->Write(str, strlen(str));
    }



    //Create files
    path[14] = '1';
    for(unsigned i = 1; i < NUMBER_OF_FILES;) {
        printf("Creating file %s\n", path);
        if(!fileSystem->Create(path)) {
            fprintf(stderr, "Perf test: cannot create %s\n", path);
            return;
        }
        printf("Opening file\n");
        OpenFile *tFile = fileSystem->Open(path);
        printf("Writing file\n");
        tFile->Write("Test", 4);

        delete tFile;
        path[8] = '0' + ++i;
        path[14] = '0' + i;
    }

    
    dir[8] = '0';
    printf("Removing directory %s\n", dir);

    
    // Remove directories
    for(unsigned i = 0; i < NUMBER_OF_FILES;) {
        printf("Removing directory %s\n", dir);
        if(!fileSystem->Remove(dir)) {
            fprintf(stderr, "Perf test: cannot remove %s\n", dir);
            return;
        }
        dir[8] = '0' + ++i;
    }

    stats->Print();
}
