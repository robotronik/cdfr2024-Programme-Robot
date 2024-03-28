#ifndef UTILS_H
#define UTILS_H
#include <time.h>

typedef enum {
    BLUE,
    YELLOW
} colorTeam_t;

inline unsigned long millis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Utilisation de CLOCK_MONOTONIC pour éviter les changements d'horloge
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000); // Conversion en millisecondes
}

#endif