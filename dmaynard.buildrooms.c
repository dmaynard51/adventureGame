/*
Name: Daniel Maynard
Assignment: buildrooms
Info: This program will randomly create 7 rooms, in a buildrooms.pid
folder. It will make sure each room has at least 6 or less
*/


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <stdlib.h>

// holds all possible room names
char *roomNames[10];


//holds all possible room types
char *roomTypes[2];

//holds room names designated
char *roomNamesDesignation[7];


//holds our structure for rooms
//reference https://oregonstate.instructure.com/courses/1706555/pages/2-dot-2-program-outlining-in-program-2
struct room
{
  int id;
  char* name;
  int numOutboundConnections;
  struct room* outboundConnections[6];
  char* type;
};


//holds our rooms
struct room rooms[6];



void initializeRooms() {
    //holds all room options 10 rooms total
    roomNames[0] = "Dungeon";
    roomNames[1] = "Library";
    roomNames[2] = "Hallway";
    roomNames[3] = "Throne";
    roomNames[4] = "Kitchen";
    roomNames[5] = "Bedroom";
    roomNames[6] = "Cellar";
    roomNames[7] = "Dining";
    roomNames[8] = "Corridor";
    roomNames[9] = "Stable";


    int i = 0;
    int j = 0;    
    
    //designate ids
    for (i = 0; i < 7; i++) 
    {
        rooms[i].id = i;
    }

    
    
}



// Returns true if a connection can be added from Room x (< 6 outbound connections), false otherwise
int CanAddConnectionFrom(struct room* input) 
{
    
    if (input->numOutboundConnections < 6) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



// Resource: https://www.geeksforgeeks.org/generating-random-number-range-c/
//returns a random number
int printRandoms(int lower, int upper) 
{ 
        int num = (rand() % (upper - lower + 1)) + lower; 
        
        return num;
    
} 



// Returns a random Room, does NOT validate if connection can be added
struct room GetRandomRoom()
{
    
    int i; 
    int h;
    //randomly choose between 7 rooms
    i = printRandoms(0,6);
    return rooms[i];
   
}


// Returns true if Rooms x and y are the same Room, false otherwise
int IsSameRoom(struct room* x, struct room* y) 
{
    
    if (strcmp(x->name, y->name) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}


// Returns true if a connection from Room x to Room y already exists, false otherwise
int ConnectionAlreadyExists(struct room *x, struct room *y)
{
    //check if connection of x exists in y
    int i;
    
    char tempName;
    
    
    for (i = 0; i < y->numOutboundConnections; i++)
    {
        
        
        struct room *A;
        A = y->outboundConnections[i];
        
        if (y->numOutboundConnections == 0)
        {
            return 0;
        }
        
        else if (strcmp(x->name, y->outboundConnections[i]->name) == 0)
        {

            return 1;
        }
    }
    
    //finish loop then return 0
    return 0;
    
}

void ConnectRoom(struct room* x, struct room* y) 
{
    int i;
    int xlastConnection = 0;
    int ylastConnection = 0;    
    
    
    //makes x outbound connection y
    x->outboundConnections[x->numOutboundConnections]=y;
    
    //increase size of numOutBoundConnections
    x->numOutboundConnections++;
    
}


//checks if graph is full. need 7 rooms
// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
int IsGraphFull()  
{
    int i;
    fflush( stdout );
    
    
    int full = 0;
    //check 7 rooms have at least 3 outbound connections
    for (i = 0; i < 7; i++)
    {
        
        if (rooms[i].numOutboundConnections > 2)
        {
            full++;

            
            
        }
        else
        {
            //if number of outbound connections is less than 3 then
            //add another random connection
            return 0;
        }
    }
    
    
    //if we have 7 rooms with 3 outbound connections we're full
    if (full == 7)
    {
        return 1;
    }
    else
    {
        
        return 0;
    }
}



//reference https://oregonstate.instructure.com/courses/1706555/pages/2-dot-2-program-outlining-in-program-2
// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection()  
{
  struct room A;  // Maybe a struct, maybe global arrays of ints
  struct room B;
  
  

  while(1)
  {
      //gets a random room
    A = GetRandomRoom();
    //check if we can add a connection
    if (CanAddConnectionFrom(&A) == 1)
    
      break;
  }

  do
  {
      //get a random room for B
    B = GetRandomRoom();
  }
  
  //keep running the following if A&B are the same room (change the room)
  //if we can't add a connection from B
  //if a connection already exists
  while(CanAddConnectionFrom(&B) == 0 || IsSameRoom(&A, &B) == 1 || ConnectionAlreadyExists(&A, &B) == 1);
  
  int i, j;
  int aName, bName;
  
  
  //populate A's connections
  for (i=0; i < 7; i++)
  {
      if (A.name == rooms[i].name)
      {
          aName = i;
      }
  }
  
  
  //populate B's connections
  for (i=0; i < 7; i++)
  {
      if (B.name == rooms[i].name)
      {
          bName = i;
      }
  }  

    //connect a and B
    //Connect b to a
  ConnectRoom(&rooms[aName], &rooms[bName]);  // TODO: Add this connection to the real variables, 
  ConnectRoom(&rooms[bName], &rooms[aName]);  // TODO: Add this connection to the real variables,   
  //ConnectRoom(&B, &A);  //  because this A and B will be destroyed when this function terminates

  
}


//makes sure start and end do not equal each other
//adds random rooms to the buildrooms folder
//as we add each room, tag a 0 next to the roomName
//so we don't choose it again
void randomize()
{
    int i, j;
    int randomRoom;
    int iter = 0;
    int start = 1;
    int end = 1;
        
    
    for (i = 0; i < 7; i++)
    {
        while (iter == 0)
        {
            //get random room betwen 1-10
            //sets our middle rooms
            randomRoom= printRandoms(0, 9);
            if (roomNames[randomRoom] != "0")
            {
                rooms[i].name = roomNames[randomRoom];
                roomNames[randomRoom] = "0";
                iter = 1;
                rooms[i].numOutboundConnections = 0;
                rooms[i].type = "MID_ROOM";
            }
        }
        iter = 0;
    }
    

        
    //if start equals end we need to randomize until we get
    //another room
    while (start == end)
    {
        start = printRandoms(0, 6);
        end = printRandoms(0, 6);
    }
    //set start room
    rooms[start].type = "START_ROOM";
    //set end room
    rooms[end].type = "END_ROOM";
    
    

    
    
}


void createFile()
{
    char dirName[20];
    char fileData[100];
    ssize_t nread, nwritten;
    
    //get pid
    char pidVar[100];
    memset(pidVar, '\0', sizeof(pidVar));
    sprintf(pidVar, "%d", getpid());
    
    int file_descriptor;
    //makes sure all 20 chars are empty
    memset(dirName, '\0', sizeof(dirName)); // Clear out the array before using it
    //add name to dirName
    strcpy(dirName, "dmaynard.rooms.");
    //adds PID to the end of the directory name
    strcat(dirName, pidVar);
    //create the concatenated directory
    mkdir(dirName, 0755);
    //move into new folder
    chdir(dirName);
    int i, k;
    for (i = 0; i < 7; i++)
    {
        memset(dirName, '\0', sizeof(dirName)); // Clear out the array before using it
        strcpy(dirName, rooms[i].name);
        //for each room add _room to the end of the file
        strcat(dirName, "_room");
        
        //creates a file
        file_descriptor = open(dirName, O_WRONLY | O_CREAT, 0600);            
        
        //clear file data
        memset(fileData, '\0', sizeof(dirName)); // Clear out the array before using it
        //prints the name of the room
        sprintf(fileData, "ROOM NAME: %s\n", rooms[i].name);
        
        //writes into file descriptor, file data
        
        nwritten = write(file_descriptor, fileData, strlen(fileData) * sizeof(char));
        
        
        //create the file in the rooms folder
        for (k = 0; k < rooms[i].numOutboundConnections; k++)
        {
            memset(fileData, '\0', sizeof(dirName)); // Clear out the array before using it
            //prints out connections
            sprintf(fileData, "Connection %i: %s\n", k + 1, rooms[i].outboundConnections[k]->name);
            
            nwritten = write(file_descriptor, fileData, strlen(fileData) * sizeof(char));
        }
        memset(fileData, '\0', sizeof(dirName)); // Clear out the array before using it
        //prints the type of room it is
        sprintf(fileData, "Room Type: %s\n", rooms[i].type);
        nwritten = write(file_descriptor, fileData, strlen(fileData) * sizeof(char));
        
    }

    
}



int main () {
    

    initializeRooms();
    
    //for random function
    srand(time(NULL));   // Initialization, should only be called once.
    
    int i,j;
    //get random middle,start, end rooms
    //make sure start and end do not equal each other
    randomize();
    
    
   
    
    //check if graph is full and add random connections
    while (IsGraphFull() == 0)
    {
        
        AddRandomConnection();
        
    }   
    
    
    
    //change directory to dmaynard.rooms.PID
    //create the files
    createFile();
    
  
    return 0;
}