#include "threads/system.hh"
#include <stdio.h>
#include <string.h>
#include "open_file.hh"
#include "file_system.hh"
#include "lib/utility.hh"
#include "machine/disk.hh"
#include "machine/statistics.hh"
#include "threads/thread.hh"
#include "threads/system.hh"

const unsigned LEN_FILE = 15;

/// Script para probar la capacidad de concurrencia del sistema de archivos.
/// Correr nachos con las opciones -ct y -rs 10 varias veces para ver un poco de
/// interleaving.
void
Productor(void *arg)
{
    DEBUG('f', "Empieza Productor.\n");
    OpenFile *file = fileSystem->Open("prodcons");
    ASSERT(file);
    int i = 0;
    while (i < 10) {
        char num = '0' + i;
        file->Write(&num, 1);
        printf("Productor produjo: %d\n", i);
        i++;
    }
    delete file;
}

void
Consumidor(void *arg)
{
    DEBUG('f', "Empieza Consumidor.\n");
    OpenFile *file = fileSystem->Open("prodcons");
    ASSERT(file);
    char contents[LEN_FILE] = "\0";
    while (strlen(contents) < 10) {
        file->Read(contents, LEN_FILE);
        file->Seek(0);
        printf("Consumidor consumio: %s\n", contents);
    }
    delete file;
}

void fsProductorConsumidor()
{
    fileSystem->Create("prodcons");

    Thread *producer = new Thread("Producer", true);
    Thread *consumer = new Thread("Consumer", true);

    producer->Fork(Productor, nullptr);
    consumer->Fork(Consumidor, nullptr);

    producer->Join();
    consumer->Join();
    fileSystem->Remove("prodcons");
    currentThread->Finish();
}