/*
Name: Daniel Maynard
Assignment: adventure
Info: This program will read dmaynard.buildrooms.PID, extract
the information needed and will then create a game for the user
based on the rooms in the buildrooms folder.
*/


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>

char dName[50];


//holds each file name
char* files[7];
//holds the number of lines per file
int roomLines[7];

char *roomData[7];
//holds room name
char roomName[15];

int startRoom;
int endRoom;
int currentRoom;
//user input
char input[15];
//holds the history
int history[50];
int counter = 0;
int game = 0;



time_t latest = 0;
char finalFolder[60];


//holds chars for roomtypes
char roomTypes[20];

char connections[20];

//memory for roomNames
char* roomNames[7];
//memory for types
char* types[7];
//memory for outBounds
char **outBounds[7];

//holds the room characters
char roomdata[500];

int gameOver = 0;





struct room
{
  int id;
  char* name;
  int numOutboundConnections;
  char* outboundConnections[7];
  char* type;
};
struct room rooms[7];


pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
//holds our time thread
pthread_t timeThread;

//holds game thread
pthread_t gameThread;

int time1;
int game1;




//read the directory files
//Resouce: https://stackoverflow.com/questions/3554120/open-directory-using-c
void getFileNames() 
{
    
    
    //holds name of the directory to enter
    char dirName[60] = "dmaynard.rooms.";

    

    
    
    //holds info in room directory
    struct dirent *pDirent;
    //subdirectory files
    struct stat subDirectoryFiles;
    
    //current directory
    DIR* dirCheck;
    //DIR* fp;
    char *ch;
    int file_descriptor;   
    
    ssize_t nread, nwritten;
    char readBuffer[1000];
    
    int i=0;
    
    dirCheck = opendir (".");
    
    
    
    
    
    
    //while reading directories of the parent
    while ((pDirent = readdir(dirCheck)) != NULL) {
            //if there is a sub directory   
            if (strstr(pDirent->d_name, dirName) != NULL )
            {
                
                //get the file name
                stat(pDirent->d_name, &subDirectoryFiles);
                
                //reference: https://stackoverflow.com/questions/42170824/use-stat-to-get-most-recently-modified-directory
                //check if file time is more recent then last
               if ((int)subDirectoryFiles.st_mtime > (int)latest) 
               
                    {
                            //clear char
                            memset(finalFolder, '\0', sizeof(finalFolder));
                            //copy name to finalFolder char
                            strcpy(finalFolder, pDirent->d_name);
                            //update the latest to compare to the next folder
                            latest = subDirectoryFiles.st_mtime;
                    }                
            }
    }
    
    //open the latest folder we just found
    dirCheck = opendir (finalFolder);
    struct dirent *sDirent;
    //read files in subdirectory
    while ((sDirent = readdir(dirCheck)) != NULL)
    {
        //loop through each file in the rooms folder
        if(strstr(sDirent->d_name, ".") == NULL)
        {
            //add name of the document to files[i]
            files[i] = sDirent->d_name;
            i++;
        }
                    
    }
    
    
}





//reference to get number of lines per file 
//https://www.geeksforgeeks.org/c-program-count-number-lines-file/
void readFiles()
{
    
    
    int i;
    int k = 0; // iter for file lines
    char c;
    DIR* dirCheck;
    dirCheck = opendir (".");

    
    //move to the directory we found in getFileNames function
    chdir(finalFolder);
    //holds info in room directory
    int count = 0;  // Line counter (result)
    char readBuffer[1000]; 

    // Get file name from user. The file should be 
    // either in current folder or complete path should be provided 
    
    FILE *fp;
    //loop through each file
    for (i = 0; i < 7; i++)
    {
    count = 0;
    strcpy(readBuffer, files[i]);
    // Open the file 
    fp = fopen(readBuffer, "r"); 
  
    // Check if file exists 
    if (fp == NULL) 
    { 
        printf("Could not open file %s", readBuffer); 
        exit(1);
    } 
  
    // Extract characters from file and store in character c 
    for (c = getc(fp); c != EOF; c = getc(fp)) 
    
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1; 
  
    // Close the file 
    fclose(fp); 
    //roomLines is the number of lines in this file
    roomLines[k] = count;
    k++;
    }
    
    i = 0;
    


}


void readRoom(int i)
{
    
    
//reference: https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c
memset(roomdata, '\0', sizeof(roomdata));
    FILE *fp;
        char readBuffer[1000]; 
    fp = fopen(readBuffer, "r"); 
    
char* ch;
long lSize;
char *buffer;
int p = 0;
//read in data from first file
    fp = fopen ( files[i] , "rb" );
    if( !fp ) perror(files[i]),exit(1);
    
    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );
    
    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);
    
    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
      fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);
    
    //move data to roomdata to be processed
    strcpy(roomdata, buffer);
    fclose(fp);
    free(buffer);


   
    
}


//the function will process the read file
//and set room names, types, and connections
void processData(int roomNum)
{
    
    int i =0;
    int j = 0;
    int p = 0;
    int q = 0;
    int lineCount = 0;
    memset(roomName, '\0',sizeof(roomName));
    //cycle through all file.
    while (roomdata[i] != '\0')
    {
        //if the first line
        memset(roomName, '\0',sizeof(roomName));
        if (lineCount == 0)
        {
            //move to the :
            while (roomdata[i] != ':')
            {
                i++;
            }
            i = i + 2;
            //grab the name of the word
            while(roomdata[i] != '\n')
            {
                roomName[p] = roomdata[i];
                i++;
                p++;
            }
            lineCount++;
           // memset(roomNames[roomNum], '\0',sizeof(roomName));
           
            strcpy(roomNames[roomNum], roomName);
            
            //reset room data
            memset(roomName, '\0',sizeof(roomNames[roomNum]));
            printf("%s", roomName);
            

        }
        i++;
    }
    
    

    lineCount = 0;
    //reset i    
    i= 0;
    p = 0;
    //holds our iterator to count the chars of each connections
    int l = 0;
    //holds the array of each connection
    int m = 0;
    
    
    //grab types and connections
    
    while (roomdata[i] != '\0')
    {
        //this is the name, we already got from above.
        if (lineCount ==  0)
        {
            i++;
            while (roomdata[i] != ':')
            {
                i++;
            }
            i = i + 2;            
            while (roomdata[i] != '\n')
            {

                i++;
            }
            
            lineCount++;
            i++;
            
        }    
        
        
        
        //find everything below lineCount 0 and above the last lineCount
        if (lineCount < roomLines[roomNum]-1 && lineCount > 0)
        {
            while (roomdata[i] != ':')
            {
                i++;
                
            }
            i = i+2;
            
            
            while(roomdata[i] != '\n')
            {
                
                //printf("%c", roomdata[i]);
                connections[l] = roomdata[i];
                i++;
                l++;
            }
            //allocate to dynamic memory for outbounds
            strcpy(outBounds[roomNum][m], connections);
            m++;
            //clear the connections buffer

            lineCount++;
            i++;
            //reset connection buffer
            l= 0;
            memset(connections, '\0',sizeof(connections));
            
        }
        
        //if last line we allocate the type to dynamic memory.
        if (lineCount == roomLines[roomNum]-1)
        {
            
            i++;
            while (roomdata[i] != ':')
            {
                i++;
            }
            i = i + 2;
            
            //find the end of the line, then set the room type
            while (roomdata[i] != '\n')
            {
                 
                roomTypes[p] = roomdata[i];
                i++;
                p++;
                
            }            
            

            
            //allocate to dynamic memory
            strcpy(types[roomNum], roomTypes);
            
     
            //resets the buffer
            memset(roomTypes, '\0',sizeof(roomTypes));  
            m=0;            
            i++;
        }


        
        
    }

}



//create dynamic memory for names, types and outbound names
void allocateData()
{
    int i;
    int p;
    for (i=0; i < 7; i++)
    {
        //dynamically create rooms for the room names
        roomNames[i] = malloc(50* sizeof(*roomNames[i]));
        memset(roomNames[i],'\0',50* sizeof(*roomNames [i]));
        		
        //dynamically create memory for the types
        types[i] = malloc(50* sizeof(*types [i]));
        memset(types[i],'\0',50* sizeof(*types [i] ));
        		
        //create an array of array of chars, for all outbounds
        outBounds[i] = malloc(50 *sizeof(*outBounds [i] ));
        memset(outBounds[i],'\0',50 *sizeof(*outBounds [i] ));
        
        //create 7 outbound chars of memory
        for (p=0; p < 7; p++)
        {	
            outBounds[i][p] = malloc(50 * sizeof(*outBounds[i][p]));
        	memset(outBounds[i][p],'\0',50 * sizeof(*outBounds[i][p]));
    	}

		
    }
}

void setupRooms()
{
    int i,p;
    //check all the rooms for start room and end room and
    //set the start/end room.
    //current room will be set to start room.
    for (i = 0; i < 7; i++)
    {
        rooms[i].name = roomNames[i];

        rooms[i].type = types[i];

        //check if room is a start room
        if (strcmp(rooms[i].type, "START_ROOM") == 0)
        {
            startRoom = i;
            //printf("start room is: %s\n", rooms[i].name);
        }
        //check if room is an end room
        if (strcmp(rooms[i].type, "END_ROOM") == 0)
        {
            endRoom = i;

        }        
        
        //reduce number of lines by 2, because the first line is room name, last line is the room type
        rooms[i].numOutboundConnections = roomLines[i]-2;
      
        //populate outbound connections
        for (p = 0; p < roomLines[i]-2; p++)
        {
            rooms[i].outboundConnections[p] = outBounds[i][p];
            
        }
    }

        
        
    
    
    
}

void *writeTime()
{
    //locks time thread
    pthread_mutex_lock(&myMutex);
        
        
    char buffer[100];
    memset(buffer,'\0', sizeof(buffer));
    //reference https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program
    //reference for each strftime value http://strftime.org/
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    
    
    
    
    FILE *fp;
    fp = fopen("currentTime.txt", "w");
    
    
    strftime(buffer, 10, "%-H", tm);
    //get hour
    fprintf(fp, "%s" , buffer);
    //add colon
    fprintf(fp, "%s" , ":");
    //clear buffer
    memset(buffer,'\0', sizeof(buffer));
    //get minutes
    strftime(buffer, 10, "%M", tm);
    fprintf(fp, "%s" , buffer);
    //add am/pm
    memset(buffer,'\0', sizeof(buffer));
    strftime(buffer, 10, "%p", tm);
    fprintf(fp, "%s" , buffer);
    fprintf(fp, "%s", ", ");
    //clear buffer
    memset(buffer,'\0', sizeof(buffer));
    //get day
    strftime(buffer, 10, "%A", tm);
    fprintf(fp, "%s" , buffer);
    fprintf(fp, "%s", ", ");
    memset(buffer,'\0', sizeof(buffer));
    
    //get month
    
    strftime(buffer, 10, "%B", tm);
    fprintf(fp, "%s" , buffer);
    fprintf(fp, "%s", " ");
    memset(buffer,'\0', sizeof(buffer));
    
    //get day date
    
    strftime(buffer, 10, "%d", tm);
    fprintf(fp, "%s" , buffer);
    fprintf(fp, "%s", ", ");
    memset(buffer,'\0', sizeof(buffer));

    //add year
    strftime(buffer, 10, "%Y", tm);
    fprintf(fp, "%s" , buffer);
    fprintf(fp, "%s" , "\n");
    //close file
    fclose(fp);
    
    //unlocks time thread
    
    pthread_mutex_unlock(&myMutex);
    
    
    
    
 
}

//reference: https://stackoverflow.com/questions/3747086/reading-the-whole-text-file-into-a-char-array-in-c
void readTime()
{
    long lSize;
    char *buffer;
    FILE *fp;
    
    //read whats in currentTime.txt
    fp = fopen("currentTime.txt", "r");
    
    if (fp)
    {
    //move to end of file
    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    //move back
    rewind( fp );
    
    /* allocate memory for entire content */
    buffer = calloc( 1, lSize+1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);
    
    /* copy the file into the buffer */
    if( 1!=fread( buffer , lSize, 1 , fp) )
      fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);        
        
    }
    printf("\n%s\n", buffer);
    printf("WHERE TO? >");
    
    fclose(fp);
    
    
}

void *printData()
{

    
    //read current location
    printf("\nCURRENT LOCATION: %s\n", rooms[currentRoom].name);
    
    int i;
    //read possible connections
    printf("POSSIBLE CONNECTIONS: ");
    for (i = 0; i < rooms[currentRoom].numOutboundConnections; i++ )
    {
        //if not last connection add comma
        if (i <= rooms[currentRoom].numOutboundConnections-2){
            printf("%s, ", rooms[currentRoom].outboundConnections[i]);}
        //if the last connection add a period
        else
        {
            printf("%s.\n", rooms[currentRoom].outboundConnections[i]);
        }
    }
    

    
    
}

void printHistory()
{
    int i;
    //counter holds the steps taken
    printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!");
    printf("\nYOU TOOK %i STEPS. YOUR PATH TO VICTORY WAS:\n", counter);
    for (i = 0; i < counter; i++)
    {
        if(i <= counter-2)
        {
            printf("%s \n", rooms[history[i]].name);
        }
        else
            printf("%s ", rooms[history[i]].name);
    }
    
    printf("\n");   
}





void *inputValidation()
{

    //time thread will be run in write time function
    time1 = pthread_create( &timeThread, NULL,writeTime,NULL);
    printData();

    printf("WHERE TO? >");
    int validation=2;
    
    
    
    
    
    //if 0 then input doesnt match time or any of the rooms
    //if 2 then it matches time and reads the time.txt
    while (validation == 2 || validation == 0){
        
    validation = 0;
    //clear input
    memset(input,'\0', sizeof(input));
    //read user input
    scanf("%s", input);
    
    //check if input matches time
    if (strcmp(input, "time") == 0) 
    {
        readTime();
        memset(input,'\0', sizeof(input));
        validation = 2;
    }
    
    
    
    //check if its in the current outbound connections
    int i;
    for (i = 0; i < rooms[currentRoom].numOutboundConnections; i++)
    {
        //check if input matches any of the current rooms outbound
        //connections
        if (strcmp(input, rooms[currentRoom].outboundConnections[i]) == 0)
        {
            validation = 1;
            
            
        }
    }
    
    //holds the number of rooms
    int p = 0;
 

    //if validation equal 1, then set the new current room
    if (validation == 1)
    {
        for (p = 0; p < 7; p++)
        {
            if (strcmp(rooms[p].name, input) == 0)
            {

                currentRoom = p;
                history[counter] = p;

                counter++;
                
            }
        }
    }
    //if validation is 0 then output try again
    if (validation == 0)
    {
        printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n");
        printData();
        printf("WHERE TO? >");
    }
    //ends while loop
    }
    
    //joins time thread
    pthread_join(timeThread,NULL );

    return NULL;
    
}

void *playGame()
{
    //check if we reached end room
    if (currentRoom == endRoom)
    {
        gameOver = 1;
    }
    
    //if not end room then continue inputValidation
    if (gameOver == 0)
    {
        inputValidation();
    }

}




int main () {
    //dynamically create space for rooms, types and outbound rooms
    allocateData();
    //get each file name
    getFileNames();
    //read in data from each file
    readFiles();
    int i;
    for (i = 0; i < 7; i++)
    {
    //read each room
    readRoom(i); 
    //process the data from each room
    processData(i);
    
    
    }


    printf("\n");
    
    
    setupRooms();
    //move back out of file to make currentTime.txt
    chdir("..");
    //set currentRoom to startRoom
    currentRoom = startRoom;
    
    //game thread will be run
    game1 = pthread_create(&gameThread, NULL, playGame, NULL);


    pthread_join(gameThread,NULL ); 

    while (gameOver == 0)
    {
        playGame();
        //conect game thread
        pthread_join(gameThread,NULL ); 
    }
              
    //print out rooms taken
    printHistory();
    
    return 0;
}