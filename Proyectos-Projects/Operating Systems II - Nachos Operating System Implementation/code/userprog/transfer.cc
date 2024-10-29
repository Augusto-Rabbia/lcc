/// Copyright (c) 2019-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "transfer.hh"
#include "lib/utility.hh"
#include "threads/system.hh"


void ReadBufferFromUser(int userAddress, char *outBuffer,
                        unsigned byteCount) {
    ASSERT(userAddress != 0);
    ASSERT(outBuffer != nullptr);
    ASSERT(byteCount != 0);


#if !defined(USE_TLB) && !defined(DEMAND_LOADING)
    unsigned count = 0;
    do {
        int temp;
        count++;
        ASSERT(machine->ReadMem(userAddress++, 1, &temp));
        *(outBuffer++) = (unsigned char) temp;
    } while (count < byteCount);
#else
    unsigned count = 0;
    do {
        int temp;
        count++;
        bool read;
        do {
            read = (machine->ReadMem(userAddress, 1, &temp));
        } while (!read);
        ++userAddress;
        *(outBuffer++) = (unsigned char) temp;
    } while (count < byteCount);

#endif
}

bool ReadStringFromUser(int userAddress, char *outString,
                        unsigned maxByteCount)
{
    ASSERT(userAddress != 0);
    ASSERT(outString != nullptr);
    ASSERT(maxByteCount != 0);

#if !defined(USE_TLB) && !defined(DEMAND_LOADING)
    unsigned count = 0;
    do {
        int temp;
        count++;
        ASSERT(machine->ReadMem(userAddress++, 1, &temp));
        *outString = (unsigned char) temp;
    } while (*outString++ != '\0' && count < maxByteCount);
#else
    unsigned count = 0;
    do {
        int temp;
        count++;
        bool read;
        do {
            read = (machine->ReadMem(userAddress, 1, &temp));
        } while (!read);
        ++userAddress;
        *outString = (unsigned char) temp;
    } while (*outString++ != '\0' && count < maxByteCount);
#endif

    return *(outString - 1) == '\0';
}

void WriteBufferToUser(const char *buffer, int userAddress,
                       unsigned byteCount)
{
    ASSERT(userAddress != 0);
    ASSERT(byteCount != 0);
    ASSERT(buffer != nullptr);

    int temp;
    unsigned count = 0;

#if !defined(USE_TLB) && !defined(DEMAND_LOADING)
    do {
        temp = *buffer++;
        ASSERT(machine->WriteMem(userAddress++, 1, temp));
    } while (++count < byteCount);
#else
    bool wrote;
    do {
        temp = *buffer++;
        do {
            wrote = machine->WriteMem(userAddress, 1, temp);
        } while (!wrote);
        ++userAddress;
    } while (++count < byteCount);
#endif
}

void WriteStringToUser(const char *string, int userAddress)
{
    ASSERT(userAddress != 0);
    ASSERT(string != nullptr);
    int temp;

#if !defined(USE_TLB) && !defined(DEMAND_LOADING)
    do {
        temp = *string;
        ASSERT(machine->WriteMem(userAddress++, 1, temp));
    } while (*(string++) != '\0');
#else
    do {
        bool wrote = false;
        temp = *string;
        do {
            wrote = machine->WriteMem(userAddress, 1, temp);
        } while (!wrote);
        ++userAddress;
    } while (*(string++) != '\0');
#endif
}
