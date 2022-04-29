#ifndef LOGER_H
#define LOGER_H
#include <stdlib.h>
#include <stdio.h>

typedef enum status{
    FILE_OPENING_ERROR,
    MEMORY_ERROR,
    SUCCESS,
}status_t;

typedef struct logger{
    status_t status;
    char * filename;
    FILE* file;
}logger_t;

void InitLog(logger_t *log);
void Logging(logger_t *log);
void CloseLog(logger_t *log);

#endif
