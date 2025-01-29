#ifndef GAMEMATH

#define GAMEMATH

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846

unsigned int seed;

//Initialize random seed based on the current time
void RandomInitialization()
{
    seed = time(NULL);
    srand(seed);
}

// Returns a pseudo-random int with "min" being the lowest and "max" being the highest
int RandomInt(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

float RandomFloat(float min, float max) 
{
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

float Lerp(int a, int b, float t) // Made by Cade Dowdy©
{
    return (float)a + t * ((float)b - (float)a);
}



// all of this is for sleeping with milliseconds across platforms

#ifdef _WIN32
    #include <windows.h>  // Windows Sleep(ms)
#else
    #include <unistd.h>   // Unix usleep(microseconds)
#endif

void sleep_ms(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);  // Windows Sleep takes milliseconds
    #else
        usleep(milliseconds * 1000);  // Unix usleep takes microseconds
    #endif
}

#endif