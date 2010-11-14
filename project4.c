#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memory
{
    int age;
    char *PID;
};

int main(void)
{
    void removeProc( struct memory temp[], int age );
    void insertProcess( struct memory temp[], int size, int startPos, char *PID, int age);
    int i, size, result, oldestProcess, newestProcess;
    char fileName[20], *PID;
    struct memory entries[64];
    FILE *filePtr;

    while(1)
    {
        printf("\nEnter the file to be read in (or type 'done' to exit): ");
        scanf("%s", &fileName);
        
        if ( !strcasecmp( fileName, "done" ) )
        {
            printf("\nGoodbye\n");
            return 0;
        }
        for ( i = 0; i < 8; i++ )
        {
            entries[i].PID = "OS";
            entries[i].age = -1;
        }
        for ( i = 8; i < 64; i++ )
        {
            entries[i].PID = "free";
            entries[i].age = 0;
        }
        filePtr = fopen(fileName, "r");
        oldestProcess = 1;
        newestProcess = 1;
        while( 1 )
        {
            if ( filePtr == NULL ) //If there's no file by that name
            {
                printf("File not found.\n");
                break;
            }
            else
            {
                if ( fscanf(filePtr, "%s %i", PID, &size) == EOF ) //If it hits the end of file, it's done reading it
                    break;
                else
                {   
                    for ( i = 0; i < 64; i++ ) //Otherwise it first scans for whether the process is in memory at all
                    {
                        if ( !strcasecmp( entries[i].PID, PID ))
                        {
                            printf("Process %s already in memory\n", PID);
                            break;
                        }
                    }
                    result = search( entries, size, PID);
                    while ( result == 0 )
                    {
                        printf("Not enough space...removing oldest process\n");
                        //removeProc(entries, oldestProcess);
                        //result = search( entries, size, PID);
                    }
                    printf("Placing process %s in memory...\n", PID);
                    //insertProcess( entries, size, result, PID, newestProcess);
                }
            }
        }
    }  
    
    return 0;
}

int search ( struct memory tempMem[], const int size, char *PID ) //First sweeps memory for whether there's enough free space at all
{                                                                 //If so, checks for enough continuous free space. If either condition isn't met,
    int i, freeCount = 0, start;                                   //returns a 0. Otherwise, returns starting address of where it'll fit
    
    for ( i = 0; i < 64; i++ )
        if ( !strcasecmp( tempMem[i].PID, "free" ))
            freeCount++;
    if ( freeCount < size )
        return 0;
    else
    {
        i = 0;
        while ( i < 64 )
        {
            for ( i; strcasecmp( tempMem[i].PID, "free" ); i++ );
            start = i;
            freeCount = 0;
            for ( i; !strcasecmp( tempMem[i].PID, "free" ); i++ )
                freeCount++;
            if ( freeCount >= size )
                return start;
        }
        return 0;
    }
}

void removeProc( struct memory temp[], int age )
{
    int i, *ptrAge;
    struct memory *tmp;
    tmp = temp;
    ptrAge = &age;
    
    for ( i = 0; i < 64; i++ )
        if ( &(tmp[i].age) == ptrAge )
        {
            temp[i].age = 0;
            temp[i].PID = "free";
        }
        
    ptrAge++;
}

void insertProcess( struct memory temp[], int size, int startPos, char *PID, int age)
{
    struct memory *memPtr;
    memPtr = temp;
    int i, *ptrAge;
    ptrAge = &age;
    
    for ( i = startPos; i < startPos + size; i++ )
    {
        memPtr[i].age = age;
        temp[i].PID = PID;
    }
    
    ptrAge++;
}
