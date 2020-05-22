//  sections1.c: mutual exclusion model sections

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "sections.h"
#include "mdat.h"

//global variables
pthread_mutex_t* chopsticks_mutex;
static int numPhil;

void
sectionInitGlobals(int numPhilosophers)
{
  const int MAX_CHAR = 6;
  int i;
  char** philosophers;
  numPhil = numPhilosophers;
  philosophers = malloc(numPhilosophers * sizeof(char*));
  chopsticks_mutex = malloc(numPhilosophers * sizeof(pthread_mutex_t));
  for (i = 0; i < numPhilosophers; i++)
    {
      philosophers[i] = malloc(MAX_CHAR * sizeof(char));
      sprintf(philosophers[i], "%d", i); 
      mdat_mutex_init(philosophers[i], &chopsticks_mutex[i], NULL);
    }
}

void sectionRunPhilosopher(int philosopherID, int numRounds)
{
  int i;
  //left chopstick
  int left = philosopherID;
  //right chopstick
  int right = (philosopherID + 1) % numPhil;
  for (i = 0; i < numRounds; i++)
    {
      //pickup (lock) chopsticks
      mdat_mutex_lock(&chopsticks_mutex[left]); 
      mdat_mutex_lock(&chopsticks_mutex[right]); 
      eat();
      //putdown (unlock) chopsticks
      mdat_mutex_unlock(&chopsticks_mutex[left]);
      mdat_mutex_unlock(&chopsticks_mutex[right]);
      think();
    }
}
