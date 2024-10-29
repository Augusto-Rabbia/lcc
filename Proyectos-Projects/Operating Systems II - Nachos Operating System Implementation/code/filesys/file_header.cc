/// Routines for managing the disk file header (in UNIX, this would be called
/// the i-node).
///
/// The file header is used to locate where on disk the file's data is
/// stored.  We implement this as a fixed size table of pointers -- each
/// entry in the table points to the disk sector containing that portion of
/// the file data (in other words, there are no indirect or doubly indirect
/// blocks). The table size is chosen so that the file header will be just
/// big enough to fit in one disk sector,
///
/// Unlike in a real system, we do not keep track of file permissions,
/// ownership, last modification date, etc., in the file header.
///
/// A file header can be initialized in two ways:
///
/// * for a new file, by modifying the in-memory data structure to point to
///   the newly allocated data blocks;
/// * for a file already on disk, by reading the file header from disk.
///
/// Copyright (c) 1992-1993 The Regents of the University of California.
///               2016-2021 Docentes de la Universidad Nacional de Rosario.
/// All rights reserved.  See `copyright.h` for copyright notice and
/// limitation of liability and disclaimer of warranty provisions.


#include "file_header.hh"
#include "threads/system.hh"

#include <ctype.h>
#include <stdio.h>

static int min(int a, int b) {
    return a < b ? a : b;
}

void 
FileHeader::Initialize(unsigned sector) {
    raw.numBytes = 0;
    raw.numSectors = 0;
    raw.indirectionSector = 0;
    raw.doubleIndirectionSect = 0;
    raw.headerSector = sector;
}


/// Initialize a fresh file header for a newly created file.  Allocate data
/// blocks for the file out of the map of free disk blocks.  Return false if
/// there are not enough free blocks to accomodate the new file.
///
/// * `freeMap` is the bit map of free disk sectors.
/// * `fileSize` is the bit map of free disk sectors.
bool
FileHeader::Allocate(Bitmap *freeMap, unsigned sector, unsigned fileSize)
{
    ASSERT(freeMap != nullptr);

    if (fileSize > MAX_FILE_SIZE) {
        return false;
    }

    raw.headerSector = sector;
    raw.numBytes = fileSize;
    raw.numSectors = DivRoundUp(fileSize, SECTOR_SIZE);
    if (freeMap->CountClear() < raw.numSectors) {
        return false;  // Not enough space.
    }

    unsigned directos = min(raw.numSectors, NUM_DIRECT);
    for (unsigned i = 0; i < directos; i++) { // Alocamos los directos
        raw.dataSectors[i] = freeMap->Find();  
    }
    
    if (raw.numSectors > NUM_DIRECT) {
        unsigned indirectos = min(NUM_IND, raw.numSectors - NUM_DIRECT);
        raw.indirectionSector = freeMap->Find();
        unsigned buff[indirectos];
        for(unsigned i = 0; i < indirectos; i++) {
            buff[i] = freeMap->Find();
        }
        synchDisk->WriteSector(raw.indirectionSector, (char *)buff);
        
        if (raw.numSectors > NUM_DIRECT + NUM_IND) {
            unsigned dIndirectos = DivRoundUp((raw.numSectors - NUM_DIRECT - NUM_IND), NUM_IND); // Vemos cuantos punteros a  
            unsigned buff_dIndirectos[dIndirectos];                                       // sectores indirectos se necesitan
            
            raw.doubleIndirectionSect = freeMap->Find();                                  // Obtenemos lugar para almacenar los punteros indirectos
            
            unsigned buff_ind[NUM_IND];
            
            for(unsigned i = 0; i < dIndirectos-1; i++) {      
                buff_dIndirectos[i] = freeMap->Find();         // Almacenamos los punteros indirectos
                for(unsigned j = 0; j < NUM_IND; j++) {
                    buff_ind[j] = freeMap->Find();              // Almacenamos los punteros directos
                }
                
                synchDisk->WriteSector(buff_dIndirectos[i], (char *)buff_ind);          // Escribimos los punteros directos en el sector correspondiente
            } // El ultimo sector indirecto puede tener que no usarse completo
            buff_dIndirectos[dIndirectos-1] = freeMap->Find();
            unsigned restantes = raw.numSectors - NUM_DIRECT - NUM_IND - (dIndirectos-1)*NUM_IND;
            
            for(unsigned i = 0; i < restantes; i++) {
                buff_ind[i] = freeMap->Find();
            }
            for (unsigned i = restantes; i < NUM_IND; i++){
                buff_ind[i] = 0;
            }
            
            synchDisk->WriteSector(buff_dIndirectos[dIndirectos-1], (char *)buff_ind);
            synchDisk->WriteSector(raw.doubleIndirectionSect, (char *)buff_dIndirectos);
            
        }
    }
    
    return true;
}


/// De-allocate all the space allocated for data blocks for this file.
///
/// * `freeMap` is the bit map of free disk sectors.
void
FileHeader::Deallocate(Bitmap *freeMap)
{
    ASSERT(freeMap != nullptr);
    unsigned directos = min(raw.numSectors, NUM_DIRECT);
    for (unsigned i = 0; i < directos; i++) {
        ASSERT(freeMap->Test(raw.dataSectors[i]));  // ought to be marked!
        freeMap->Clear(raw.dataSectors[i]);
    }
    DEBUG('w', "dou\n");
    if (raw.numSectors > NUM_DIRECT) {
        unsigned indirectos = min(NUM_IND, raw.numSectors - NUM_DIRECT);
        unsigned buff[NUM_DIRECT];
        
        synchDisk->ReadSector(raw.indirectionSector, (char*)buff);
        unsigned sector;
        for(unsigned i = 0; i < indirectos; i++) {
            sector = buff[i];
            ASSERT(freeMap->Test(sector));
            freeMap->Clear(sector);
        }
        freeMap->Clear(raw.indirectionSector);
        if (raw.numSectors > NUM_DIRECT + NUM_IND) {
            unsigned dIndirectos = DivRoundUp((raw.numSectors - NUM_DIRECT - NUM_IND), NUM_IND);
            unsigned buff_dIndirectos[NUM_IND];
            
            synchDisk->ReadSector(raw.doubleIndirectionSect, (char*)buff_dIndirectos);
            unsigned buff_ind[NUM_IND];
            unsigned sector_ind;
            unsigned sector_dir;
                        
            for(unsigned i = 0; i < dIndirectos-1; i++) {
                
                sector_ind = buff_dIndirectos[i];
                
                synchDisk->ReadSector(sector_ind, (char*)buff_ind);
                for(unsigned j = 0; j < NUM_IND; j++) {
                    sector_dir = buff_ind[j];
                    
                    ASSERT(freeMap->Test(sector_dir));
                    freeMap->Clear(sector_dir);
                }
            }
            sector_ind = buff_dIndirectos[dIndirectos-1];
            synchDisk->ReadSector(sector_ind, (char*)buff_ind);
            unsigned restantes = raw.numSectors - NUM_DIRECT - NUM_IND - (dIndirectos-1)*NUM_IND;
            for(unsigned i = 0; i < restantes; i++) {
                sector_dir = buff_ind[i];
                ASSERT(freeMap->Test(sector_dir));
                freeMap->Clear(sector_dir);
            }
            freeMap->Clear(sector_ind);
        }
    }
}
/// Fetch contents of file header from disk.
///
/// * `sector` is the disk sector containing the file header.
void
FileHeader::FetchFrom(unsigned sector_)
{
    synchDisk->ReadSector(sector_, (char *) &raw);
}

/// Write the modified contents of the file header back to disk.
///
/// * `sector` is the disk sector to contain the file header.
void
FileHeader::WriteBack(unsigned sector_)
{
    synchDisk->WriteSector(sector_, (char *) &raw);
}

/// Return which disk sector is storing a particular byte within the file.
/// This is essentially a translation from a virtual address (the offset in
/// the file) to a physical address (the sector where the data at the offset
/// is stored).
///
/// * `offset` is the location within the file of the byte in question.
unsigned
FileHeader::ByteToSector(unsigned offset)
{
    unsigned i = offset / SECTOR_SIZE;
    if(i < NUM_DIRECT) {
        return raw.dataSectors[i];
    } else if (i < NUM_DIRECT + NUM_IND) {
        char buf[SECTOR_SIZE];
        synchDisk->ReadSector(raw.indirectionSector, buf);
        unsigned num = buf[(i-NUM_DIRECT)*sizeof(int)];
        return num;
    }
    else {
        
        unsigned buf_ind[SECTOR_SIZE];
        synchDisk->ReadSector(raw.doubleIndirectionSect, (char*)buf_ind);
        
        unsigned sect = (i-NUM_DIRECT-NUM_IND)/NUM_IND; 
        unsigned buf_dir[SECTOR_SIZE];
        synchDisk->ReadSector(buf_ind[sect], (char*)buf_dir);
        unsigned num = buf_dir[(i-NUM_DIRECT-NUM_IND-sect*NUM_IND)];
        
        return num;
    }
}

bool
FileHeader::Extend(Bitmap* freeMap, unsigned newSize)
{
    DEBUG('w', "newSize = %d, raw.numB = %d\n", newSize, raw.numBytes); 

    if (newSize > MAX_FILE_SIZE) {
        return false;
    }

    unsigned oldTotalSects = SizeToSectors(raw.numBytes);
    unsigned newTotalSects = SizeToSectors(newSize);
    unsigned addedTotalSects = newTotalSects - oldTotalSects;
    
    if(addedTotalSects > freeMap->CountClear()){
        return false;           // No hay suficientes sectores libres
    }
    

    unsigned newDataSectors = DivRoundUp(newSize, SECTOR_SIZE);
    unsigned oldDataSectors = DivRoundUp(raw.numBytes, SECTOR_SIZE);
    if (newDataSectors - oldDataSectors != 0)
        AllocateN(freeMap, newDataSectors - oldDataSectors);

    raw.numBytes = newSize;
    DEBUG('w', "raw.numBytes luego = %d\n", raw.numBytes);

    return true;
}

unsigned
FileHeader::SizeToSectors(unsigned numBytes)
{
    unsigned dataSectors = DivRoundUp(numBytes, SECTOR_SIZE);
    unsigned indSectors;
    if (dataSectors <= NUM_DIRECT) // No hay indireccion
        indSectors = 0;                 
    else if (dataSectors <= NUM_DIRECT+NUM_IND) // Hay indireccion simple
        indSectors = 1;
    else // Hay doble indireccion
        indSectors = 2 + DivRoundUp(dataSectors-NUM_DIRECT-NUM_IND, NUM_IND);
    return dataSectors + indSectors;
}

void
FileHeader::AllocateN(Bitmap* freeMap, unsigned N) {
    ASSERT(freeMap != nullptr);
    // Alocar los sectores de datos
    // Directos)
    int directos = min(N, NUM_DIRECT - raw.numSectors);
    
    if (directos < 0)
        directos = 0;
    for (unsigned i = raw.numSectors; i<raw.numSectors + directos; i++) { // Alocamos los directos;
        raw.dataSectors[i] = freeMap->Find();
    }
    
    raw.numSectors += directos;
    
    if (N == (unsigned) directos) // Solo se necesitaban directos
        return;
    
    // Indirectos
    int indirectos = min(N-directos, NUM_DIRECT + NUM_IND - raw.numSectors);
    if (indirectos < 0) 
        indirectos = 0;
    
    bool hasIndSector = raw.numSectors > NUM_DIRECT;
    unsigned buff[NUM_IND];
    if (!hasIndSector) 
        raw.indirectionSector = freeMap->Find();
    else
        synchDisk->ReadSector(raw.indirectionSector, (char *) buff);
    
    for (unsigned i = raw.numSectors - NUM_DIRECT; i < raw.numSectors - NUM_DIRECT + indirectos; i++) { // Alocamos los de indireccion simple
        buff[i] = freeMap->Find();
    }
    synchDisk->WriteSector(raw.indirectionSector, (char *)buff);
    raw.numSectors += indirectos;



    int dIndirectosSimples = N - indirectos - directos; // Esta es la cantidad a alocar de los N en doble indireccion.
    if(dIndirectosSimples <= 0) // No necesita doble indireccion
        return;

    // Doble indireccion

    unsigned dIndYaAlocados = DivRoundUp(raw.numSectors - NUM_DIRECT - NUM_IND, NUM_IND);

    unsigned dIndBuff[NUM_IND];
    unsigned dBuff[NUM_IND];
    if(dIndYaAlocados == 0){ // Todavia no existe el sector de doble indireccion, lo creamos.
        raw.doubleIndirectionSect = freeMap->Find();
        dIndYaAlocados = 1;
        dIndBuff[0] = freeMap->Find();
    }else {// Si ya existe, lo leemos
        synchDisk->ReadSector(raw.doubleIndirectionSect, (char*) dIndBuff); 
        synchDisk->ReadSector(dIndBuff[dIndYaAlocados-1], (char*) dBuff);
    }
    unsigned dIndAlocadosFinal = DivRoundUp(raw.numSectors + dIndirectosSimples - NUM_DIRECT - NUM_IND, NUM_IND);
    
    // bool hasDoubleIndSector = raw.numSectors > NUM_DIRECT + NUM_IND;

    
    // if (!hasDoubleIndSector){ // Si no existe aun el sector de doble indireccion, lo creamos.
    //     raw.doubleIndirectionSect = freeMap->Find();
    //     dIndBuff[0] = freeMap->Find(); 
    // }
    
    unsigned offsetInd = (raw.numSectors - NUM_DIRECT - NUM_IND)%NUM_IND; // A partir de que indice se deben alojar 
    unsigned menor;                                                       // nuevos punteros en el último sector de indirección simple
    raw.numSectors += dIndirectosSimples;
    if (!(dIndYaAlocados != dIndAlocadosFinal && offsetInd == 0)) { // El último no estaba completo, lo rellenamos

        
        menor = min(dIndirectosSimples, NUM_IND - offsetInd);     // cuanto falta para llenar el sector de indirección simple
        for(unsigned j = offsetInd; j < offsetInd + menor; j++) {
            dBuff[j] = freeMap->Find();
        }

        synchDisk->WriteSector(dIndBuff[dIndYaAlocados-1], (char*) dBuff);
        dIndirectosSimples -= menor;
        menor = min(dIndirectosSimples, NUM_IND);
    } else
        menor = min(dIndirectosSimples, NUM_IND);
    // Completamos el ultimo sector de indireccion del de doble
    for(unsigned i = dIndYaAlocados; i < dIndAlocadosFinal; i++) {
        
        dIndBuff[i] = freeMap->Find();
        for(unsigned j = 0; j < menor; j++) {
            dBuff[j] = freeMap->Find();
            
        }
        dIndirectosSimples -= menor;
        menor = min(dIndirectosSimples, NUM_IND);
        
        synchDisk->WriteSector(dIndBuff[i], (char*) dBuff);
    }
    
    synchDisk->WriteSector(raw.doubleIndirectionSect, (char*) dIndBuff);
    return;
}

/// Return the number of bytes in the file.
unsigned
FileHeader::FileLength() const
{
    return raw.numBytes;
}

/// Print the contents of the file header, and the contents of all the data
/// blocks pointed to by the file header.
void
FileHeader::Print(const char *title)
{
    char *data = new char [SECTOR_SIZE];

    if (title == nullptr) {
        printf("File header:\n");
    } else {
        printf("%s file header:\n", title);
    }

    printf("    size: %u bytes\n"
           "    block indexes: ",
           raw.numBytes);
    unsigned directos = min(raw.numSectors, NUM_DIRECT);
    for (unsigned i = 0; i < directos; i++) {
        printf("%u ", raw.dataSectors[i]);
    }

    if (raw.numSectors > NUM_DIRECT) {
        printf("\n(indirection sector %u)\n", raw.indirectionSector);
        unsigned data[NUM_IND];
        synchDisk->ReadSector(raw.indirectionSector, (char*)data);
        unsigned printear = min(NUM_IND, raw.numSectors - NUM_DIRECT);
        for (unsigned i = 0; i<printear; i++) {
            printf("%u ", data[i]);
        }
    }
    if (raw.numSectors > NUM_DIRECT + NUM_IND) {
        printf("\n(double indirection sector %u)\n With simple indirection sectors: ", raw.doubleIndirectionSect);
        unsigned data[NUM_IND];
        synchDisk->ReadSector(raw.doubleIndirectionSect, (char*)data);
        unsigned grandes = DivRoundUp(raw.numSectors - NUM_DIRECT - NUM_IND, NUM_IND);
        unsigned data2[NUM_IND];
        for (unsigned i = 0; i < grandes-1; i++) {
            printf("\nSimple: %u", data[i]);
            printf("\nPointing to the following sectors:");
            synchDisk->ReadSector(data[i], (char*)data2);
            for (unsigned i = 0; i < NUM_IND; i++) {
                printf("%u ", data2[i]);
            }
        }
        printf("\nLast simple: %u ", data[grandes-1]);
        printf("\nPointing to the following sectors: ");
        unsigned pequeños = (raw.numSectors - NUM_DIRECT - NUM_IND)%NUM_IND;
        synchDisk->ReadSector(data[grandes-1], (char*)data2);
        for (unsigned i = 0; i < pequeños; i++) {
            printf("%u ", data2[i]);
        }
    }
    printf("\n");
    
    directos = min(raw.numSectors, NUM_DIRECT);
    for (unsigned i = 0, k = 0; i < directos; i++) {
        printf("    contents of block %u:\n", raw.dataSectors[i]);
        synchDisk->ReadSector(raw.dataSectors[i], data);
        for (unsigned j = 0; j < SECTOR_SIZE && k < raw.numBytes; j++, k++) {
            if (isprint(data[j])) {
                printf("%c", data[j]);
            } else {
                printf("\\%X", (unsigned char) data[j]);
            }
        }
        printf("\n");
    }
    delete [] data;
}

RawFileHeader *
FileHeader::GetRaw()
{
    return &raw;
}

