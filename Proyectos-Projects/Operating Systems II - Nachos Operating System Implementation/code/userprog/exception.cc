/// Entry points into the Nachos kernel from user programs.
///
/// There are two kinds of things that can cause control to transfer back to
/// here from user code:
///
/// * System calls: the user code explicitly requests to call a procedure in
///   the Nachos kernel.  Right now, the only function we support is `Halt`.
///
/// * Exceptions: the user code does something that the CPU cannot handle.
///   For instance, accessing memory that does not exist, arithmetic errors,
///   etc.
///
/// Interrupts (which can also cause control to transfer from user code into
/// the Nachos kernel) are handled elsewhere.
///
/// For now, this only handles the `Halt` system call.  Everything else core-
/// dumps.
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "transfer.hh"
#include "syscall.h"
#include "filesys/directory_entry.hh"
#include "filesys/file_system.hh"
#include "threads/system.hh"
#include "address_space.hh"
#include "args.hh"

#include <stdio.h>


static void
IncrementPC()
{
    unsigned pc;

    pc = machine->ReadRegister(PC_REG);
    machine->WriteRegister(PREV_PC_REG, pc);
    pc = machine->ReadRegister(NEXT_PC_REG);
    machine->WriteRegister(PC_REG, pc);
    pc += 4;
    machine->WriteRegister(NEXT_PC_REG, pc);
}

/// Do some default behavior for an unexpected exception.
///
/// NOTE: this function is meant specifically for unexpected exceptions.  If
/// you implement a new behavior for some exception, do not extend this
/// function: assign a new handler instead.
///
/// * `et` is the kind of exception.  The list of possible exceptions is in
///   `machine/exception_type.hh`.
static void
DefaultHandler(ExceptionType et)
{
    int exceptionArg = machine->ReadRegister(2);

    fprintf(stderr, "Unexpected user mode exception: %s, arg %d.\n",
            ExceptionTypeToString(et), exceptionArg);
    ASSERT(false);
}




void ExecProg(void *args) {
    currentThread->space->InitRegisters();  // Set the initial register values.
    currentThread->space->RestoreState();


    if (args != nullptr) {
        unsigned argc = WriteArgs((char**)args);
        int stackPointer = machine->ReadRegister(STACK_REG);
        machine->WriteRegister(4, argc);
        machine->WriteRegister(5, stackPointer);
        machine->WriteRegister(STACK_REG, stackPointer - 24); // Corregimos el sp como se indica el archivo args.hh
    }


    machine->Run();// Jump to the user progam.
    ASSERT(false);   // `machine->Run` never returns; the address space
                     // exits by doing the system call `Exit`.
}


/// Handle a system call exception.
///
/// * `et` is the kind of exception.  The list of possible exceptions is in
///   `machine/exception_type.hh`.
///
/// The calling convention is the following:
///
/// * system call identifier in `r2`;
/// * 1st argument in `r4`;
/// * 2nd argument in `r5`;
/// * 3rd argument in `r6`;
/// * 4th argument in `r7`;
/// * the result of the system call, if any, must be put back into `r2`.
///
/// And do not forget to increment the program counter before returning. (Or
/// else you will loop making the same system call forever!)
static void
SyscallHandler(ExceptionType _et)
{
    int scid = machine->ReadRegister(2);

    switch (scid) {
        case SC_HALT:
            DEBUG('e', "Shutdown, initiated by user program.\n");
            interrupt->Halt();
            break;

        case SC_CREATE: { // int Create(const char *name);
            int filenameAddr = machine->ReadRegister(4); // Tenemos el 1° arg (puntero al nombre del archivo) en el registro r4 
            if (filenameAddr == 0) {    // Si el puntero es nulo...
                DEBUG('e', "Error: address to filename string is null.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }
            char filename[FILE_NAME_MAX_LEN + 1];
            if (!ReadStringFromUser(filenameAddr, filename, sizeof filename)) { // Si el nombre de archivo provisto es muy largo...
                DEBUG('e', "Error: filename string too long (maximum is %u bytes).\n", FILE_NAME_MAX_LEN);
                machine->WriteRegister(2, -1);
                break;
            }

            if (fileSystem->Create(filename, 0)) {
                DEBUG('e', "`Create` requested for file `%s`.\n", filename);
                machine->WriteRegister(2, 0);
            } else {
                DEBUG('e', "Error: could not create file `%s`.\n", filename);
                machine->WriteRegister(2, -1);
            }
            break;
        }

        case SC_REMOVE: { // int Remove(const char *name);
            int filenameAddr = machine->ReadRegister(4); // Tenemos el 1° arg (puntero al nombre del archivo) en el registro r4 
            if (filenameAddr == 0) {    // Si el puntero es nulo...
                DEBUG('e', "Error: address to filename string is null.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }

            char filename[FILE_NAME_MAX_LEN + 1];
            if (!ReadStringFromUser(filenameAddr, filename, sizeof filename)) { // Si el nombre de archivo provisto es muy largo...
                DEBUG('e', "Error: filename string too long (maximum is %u bytes).\n", FILE_NAME_MAX_LEN);
                machine->WriteRegister(2, -1);
                break;
            }

            if (fileSystem->Remove(filename)) {
                DEBUG('e', "`Remove` requested for file `%s`.\n", filename);
                machine->WriteRegister(2, 0);
            } else {
                DEBUG('e', "Error: could not remove file `%s`.\n", filename);
                machine->WriteRegister(2, -1);
            }
            break;
        }

        case SC_OPEN: { // OpenFileId Open(const char *name);
            int filenameAddr = machine->ReadRegister(4); // Tenemos el 1° arg (puntero al nombre del archivo) en el registro r4

            if (filenameAddr == 0) {    // Si el puntero es nulo...
                DEBUG('e', "Error: address to filename string is null.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }

            char filename[FILE_NAME_MAX_LEN + 1];
            if (!ReadStringFromUser(filenameAddr, filename, sizeof filename)) { // Si el nombre de archivo provisto es muy largo...
                DEBUG('e', "Error: filename string too long (maximum is %u bytes).\n", FILE_NAME_MAX_LEN);
                machine->WriteRegister(2, -1);
                break;
            }

            OpenFile* file = fileSystem->Open(filename);

            if (file != nullptr) {
                DEBUG('e', "`Open` requested for file `%s`.\n", filename);
                OpenFileId fd = currentThread->AddFd(file);
                if (fd != -1) {
                    DEBUG('e', "File `%s` opened with fd %d.\n", filename, fd);
                    machine->WriteRegister(2, fd);
                } else {
                    DEBUG('e', "Error: no space is left to add the file `%s` to the fd table.\n", filename);
                    machine->WriteRegister(2, -1);
                }
            } else {
                DEBUG('e', "Error: could not open file `%s`.\n", filename);
                machine->WriteRegister(2, -1);
            }
            break;
        }

        case SC_CLOSE: { // int Close(OpenFileId id);
            int fid = machine->ReadRegister(4);
            DEBUG('e', "`Close` requested for fd %u.\n", fid);

            if (fid <= 1) {
                DEBUG('e', "Error: invalid fd.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }
            OpenFile* file = currentThread->RemoveFd(fid);
            if (file == nullptr) {
                DEBUG('e', "Error: fd %u does not exist in the fd table.\n", fid);
                machine->WriteRegister(2, -1);
            }else {
                delete file;
                machine->WriteRegister(2, 0);
            }
            break;
        }

        case SC_READ: { // int Read(char *buffer, int size, OpenFileId id);
            int buffer = machine->ReadRegister(4);
            int size = machine->ReadRegister(5);
            int fileId = machine->ReadRegister(6);

            if (buffer == 0) {
                DEBUG('e', "Error: address to buffer is null.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }
            if (size == 0) {
                DEBUG('e', "Error: read size is null.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }
            if (fileId < 0 || fileId == 1) {
                DEBUG('e', "Error: file descriptor is invalid.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }

            // Read(char *into, unsigned numBytes);
            char buff[size];
            int bytesRead = 0;

            if (fileId == CONSOLE_INPUT) {
                DEBUG('e', "Reading from the console.\n", fileId);
                while (bytesRead < size) {
                    buff[bytesRead++] = synchConsole->GetChar();
                }
                DEBUG('e', "Successful reading\n");
                WriteBufferToUser(buff, buffer, size);
            } else {
                OpenFile* file = currentThread->Lookup(fileId);
                if (file == nullptr) {
                    DEBUG('e', "Error: fd %u does not exist in the file descriptors table.\n", fileId);
                    machine->WriteRegister(2, -1);
                    break;
                }
                bytesRead = file->Read(buff, size);
                if (bytesRead == 0) {
                    DEBUG('e', "Nothing was read\n");
                }else {
                    DEBUG('e', "Successful reading\n");
                    WriteBufferToUser(buff, buffer, size);
                }
            }
            machine->WriteRegister(2, bytesRead);
            break;
        }


        case SC_WRITE: { // int Write(const char *buffer, int size, OpenFileId id);
            int buffer = machine->ReadRegister(4);
            int size = machine->ReadRegister(5);
            int fileId = machine->ReadRegister(6);

            if (buffer == 0) {
                DEBUG('e', "Error: address to buffer is null.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }
            if (size == 0) {
                DEBUG('e', "Error: read size is null.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }
            if (fileId <= 0) {
                DEBUG('e', "Error: file descriptor is invalid.\n");
                machine->WriteRegister(2, -1); // Cargamos el valor -1 en el registro r2
                break;
            }

            char* buff = new char[size];
            ReadBufferFromUser(buffer, buff, size);
            
            int bytesWritten = 0;
            DEBUG('e', "Write request to file with file descriptor %u...\n", fileId);

            if (fileId == CONSOLE_OUTPUT) {
                DEBUG('e', "Writing `%s` to the console.\n", buff);
                while (bytesWritten < size) {
                    synchConsole->PutChar(buff[bytesWritten++]);
                }
            } else {
                OpenFile* file = currentThread->Lookup(fileId);
                if (file == nullptr) {
                    DEBUG('e', "Error: fd %u does not exist in the file descriptors table.\n", fileId);
                    machine->WriteRegister(2, -1);
                    break;
                }
                bytesWritten = file->Write(buff, size);
            }
            DEBUG('e', "Writing %s to the file with file descriptor %u...\n", buff, fileId);
            machine->WriteRegister(2, bytesWritten);
            delete buff;
            break;
        }


        case SC_EXIT: { // void Exit(int status);
            int status = machine->ReadRegister(4);
            DEBUG('s', "Program ended with status %d.\n ================================\n\n", status);
            currentThread->Finish(status);
            break; // Not reached
        }

        case SC_EXEC: { // SpaceId Exec(char *name); 
            int programPath = machine->ReadRegister(4);
            char buff[1024];
            ReadStringFromUser(programPath, buff, 1024);

            OpenFile* programToExec = fileSystem->Open(buff);
            if (programToExec == nullptr){
                DEBUG('e', "Error: EXEC could not open the file `%s`.\n", buff);
                machine->WriteRegister(2, -1);
                break;
            }
            DEBUG('e', "Request to exec the program: `%s`.\n", buff);
            AddressSpace* addS = new AddressSpace(programToExec);

            //delete programToExec;   lo liberamos cuando liberamos el AddressSpace correspondiente al programa

            Thread* newThread = new Thread(buff, true);
            
            newThread->space = addS;
            newThread->Fork(ExecProg, NULL);

            DEBUG('e', "Program `%s` forked.\n", buff);
            
            SpaceId spaceID = processTable->Add(newThread);
            machine->WriteRegister(2, spaceID);

            break;
        }

        
        case SC_EXEC2: { // SpaceId Exec2(char *name, char** args); 
            int programPath = machine->ReadRegister(4);
            int argsv = machine->ReadRegister(5);
            char buff[1024];
            ReadStringFromUser(programPath, buff, 1024);
            
            OpenFile* programToExec = fileSystem->Open(buff);
            if (programToExec == nullptr){
                DEBUG('e', "Error: EXEC could not open the file `%s`.\n", buff);
                machine->WriteRegister(2, -1);
                break;
            }
            DEBUG('e', "Request to exec the program: `%s`.\n", buff);
            AddressSpace* addS = new AddressSpace(programToExec);

            //delete programToExec;     lo liberamos cuando liberamos el AddressSpace correspondiente al programa

            Thread* newThread = new Thread(buff, true);
            
            newThread->space = addS;

            char **argv;
            argv = (argsv == 0) ? nullptr : SaveArgs(argsv);

            newThread->Fork(ExecProg, argv);

            DEBUG('e', "Program `%s` forked.\n", buff);
            
            SpaceId spaceID = processTable->Add(newThread);
            machine->WriteRegister(2, spaceID);

            break;
        }

        case SC_JOIN: { // int Join(SpaceId id);
            int spaceID = machine->ReadRegister(4);

            Thread* threadToJoin = processTable->Get(spaceID);

            if (threadToJoin == nullptr) {
                DEBUG('e', "Error: program trying to join a non-existent process\n");
                machine->WriteRegister(2, -1);
                break;
            }

            int status;
            DEBUG('e', "Joining...\n");
            threadToJoin->Join(&status);
            DEBUG('e', "Joined\n");
            machine->WriteRegister(2, status);

            processTable->Remove(spaceID);

            break;
        }
        default:
            fprintf(stderr, "Unexpected system call: id %d.\n", scid);
            ASSERT(false);
    }

    IncrementPC();
}

int tlbReplaceIndex = 0;

static void
PageFaultExceptionHandler(ExceptionType _et) {
    for(unsigned i=0; i < currentThread->space->GetNumPages(); i++) {
        ASSERT(currentThread->space->GetPageTable()[i].virtualPage == i || (int) currentThread->space->GetPageTable()[i].virtualPage == -1);
    }
    int virtualAddress = machine->ReadRegister(BAD_VADDR_REG); // machine\machine.hh BAD_VADDR_REG: The failing virtual address on an exception.
    unsigned virtualPageNumber = virtualAddress / PAGE_SIZE;
    
    TranslationEntry entry;
    ASSERT(virtualPageNumber < currentThread->space->GetNumPages());
    entry = currentThread->space->GetPageTable()[virtualPageNumber];
    DEBUG('x', "vpn: %u\n", virtualPageNumber);

#ifdef DEMAND_LOADING
    
    if(!entry.valid) { // La página no se encuentra en memoria
        
        currentThread->space->LoadPage(virtualPageNumber);
        entry = currentThread->space->GetPageTable()[virtualPageNumber];
        
    }
#endif
#ifdef USE_TLB
    TranslationEntry *tlb = machine->GetMMU()->tlb;
    tlb[tlbReplaceIndex] = entry;
    tlb[tlbReplaceIndex].valid = true;
    currentThread->space->GetPageTable()[tlb[tlbReplaceIndex].virtualPage] = tlb[tlbReplaceIndex];
    tlbReplaceIndex = (tlbReplaceIndex + 1) % TLB_SIZE; // ...Para decidir qué entrada de la TLB reemplazar, puede utilizar un índice circular.
#endif   
}


/// By default, only system calls have their own handler.  All other
/// exception types are assigned the default handler.
void
SetExceptionHandlers()
{
    machine->SetHandler(NO_EXCEPTION,            &DefaultHandler);
    machine->SetHandler(SYSCALL_EXCEPTION,       &SyscallHandler);
#if defined(DEMAND_LOADING) || defined(USE_TLB)
    machine->SetHandler(PAGE_FAULT_EXCEPTION,    &PageFaultExceptionHandler);
#else
    machine->SetHandler(PAGE_FAULT_EXCEPTION,    &DefaultHandler);
#endif

    machine->SetHandler(READ_ONLY_EXCEPTION,     &DefaultHandler);
    machine->SetHandler(BUS_ERROR_EXCEPTION,     &DefaultHandler);
    machine->SetHandler(ADDRESS_ERROR_EXCEPTION, &DefaultHandler);
    machine->SetHandler(OVERFLOW_EXCEPTION,      &DefaultHandler);
    machine->SetHandler(ILLEGAL_INSTR_EXCEPTION, &DefaultHandler);
}
