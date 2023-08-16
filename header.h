#ifndef __Heder_H__
#define __Heder_H__

#include "constants.h"
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
    float x, y;
    float direction;
    int speed;
    int index;
    float pheromone;
    bool isLeavingPheromone;
    bool isSearching;
    int food_index;
} Ant;

Ant movingAnt;
char keyPressed;
bool openNewWindow = true;

typedef struct
{
    float x, y;
    float percentage;
    int on; // on or off
} Food;

pthread_mutex_t *foodMutex;
pthread_mutex_t workMutex = PTHREAD_MUTEX_INITIALIZER;

Food *foods;
Ant *ants;

time_t foodGenerationTimer;
time_t startTime;
float simulationEndTime;

pthread_t *antThreads; // Array to store thread IDs
pthread_t foodThread;

void readConstants(char *filename)
{
    char line[200];
    char label[50];

    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("The argument file doesn't exist\n");
        exit(-2);
    }

    char separator[] = "=";

    while (fgets(line, sizeof(line), file) != NULL)
    {

        char *str = strtok(line, separator);
        strncpy(label, str, sizeof(label));
        str = strtok(NULL, separator);

        if (strcmp(label, "ant_count") == 0)
        {
            ANT_COUNT = atoi(str);
        }
        else if (strcmp(label, "food_generation_interval") == 0)
        {
            FOOD_GENERATION_INTERVAL = atoi(str);
        }
        else if (strcmp(label, "ant_smell_food") == 0)
        {
            FOOD_SMELL_DISTANCE = atof(str);
        }
        else if (strcmp(label, "ant_smell_pheromone") == 0)
        {
            PHEROMONE_THRESHOLD = atoi(str);
        }
        else if (strcmp(label, "food_eating_per") == 0)
        {
            FOOD_EATING_PERCENTAGE = atof(str);
        }
        else if (strcmp(label, "simulation_time") == 0)
        {
            SIMULATION_DURATION = atof(str);
        }
        else if (strcmp(label, "smell_intensety") == 0)
        {
            smell_intensety = atof(str);
        }
    }

    fclose(file);
}

#endif