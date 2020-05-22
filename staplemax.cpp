#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <pthread.h>
using namespace std;

//product id
#define PEN 0
#define PAPER 1
#define TONER 2
#define LAPTOP 3

const int PEN_RESTOCK = 100;
const int PAPER_RESTOCK = 200;
const int TONER_RESTOCK = 40;
const int LAPTOP_RESTOCK = 15;

//initial shelf: program starts with shelves full
const int NUM_ITEMS = 4;
unsigned int inventory[NUM_ITEMS]{ PEN_RESTOCK, PAPER_RESTOCK, TONER_RESTOCK, LAPTOP_RESTOCK };

const int NUM_THREADS = 20; //total number of sales person / threads
const string DIRECTORY = "/home/fac/lillethd/cpsc3500/projects/sales_data/";
const string FILENAMES[NUM_THREADS]{ "sales1.txt", "sales2.txt", "sales3.txt", "sales4.txt", "sales5.txt", "sales6.txt", "sales7.txt",
    "sales8.txt", "sales9.txt", "sales10.txt", "sales11.txt", "sales12.txt", "sales13.txt", "sales14.txt",
    "sales15.txt", "sales16.txt", "sales17.txt", "sales18.txt", "sales19.txt", "sales20.txt" };

int total_sales = 0;
pthread_mutex_t mutex;

void* processFile(void* threadArg)
{
  int thread_id;
  thread_id = (intptr_t)threadArg;

  //read file
  string line;
  string file_loc = DIRECTORY + FILENAMES[thread_id];
  ifstream file;
  file.open(file_loc);

  while (getline(file, line))
    {
      pthread_mutex_lock(&mutex);
      string item = line;
      if (item == "pen")
        {
          inventory[PEN]--;
          if (inventory[PEN] == 0)
            inventory[PEN] = PEN_RESTOCK;
        }
      else if (item == "paper")
        {
          inventory[PAPER]--;
          if (inventory[PAPER] == 0)
            inventory[PAPER] = PAPER_RESTOCK;
        }
      else if (item == "toner")
        {
          inventory[TONER]--;
          if (inventory[TONER] == 0)
            inventory[TONER] = TONER_RESTOCK;
        }
      else if (item == "laptop")
        {
          inventory[LAPTOP]--;
          if (inventory[LAPTOP] == 0)
            inventory[LAPTOP] = LAPTOP_RESTOCK;
        }
      else
        {
          printf("Error: Item not found");
          return NULL;
        }

      total_sales++;
      pthread_mutex_unlock(&mutex);
    }
  return NULL;
}

int main(int argc, char** argv) {
  int i;
  int error = -1;
  pthread_t* thread_id = new pthread_t[NUM_THREADS];

  //initializing mutex
  pthread_mutex_init(&mutex, NULL);

  //create threads
  for (i = 0; i < NUM_THREADS; i++)
    {
      error = pthread_create(&thread_id[i], NULL, processFile, (void*)(intptr_t)i);
      if (error != 0)
        {
          printf("Error creating thread");
          exit(1);
        }

    }

  //join threads
  for (i = 0; i < NUM_THREADS; i++)
    {
      error = pthread_join(thread_id[i], NULL);
      if (error != 0)
        {
          printf("Error joining thread");
          exit(1);
        }
    }

  //prints how many items are currently on the shelves and total number of sales
  printf("Pen: %i\n", inventory[PEN]);
  printf("Paper: %i\n", inventory[PAPER]);
  printf("Toner: %i\n", inventory[TONER]);
  printf("Laptop: %i\n", inventory[LAPTOP]);
  printf("Total Sales: %i\n", total_sales);
  
  //clean up
  delete[] thread_id; 

  return 0;
}
