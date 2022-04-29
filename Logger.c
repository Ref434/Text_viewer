#include "Logger.h"

void InitLog(logger_t *log)
{
    FILE *f = fopen("Log.txt","w");
    if(f != NULL)
    {
        log->file = f;
        log->status = SUCCESS;
    }
    else
        log->status = FILE_OPENING_ERROR;



}
void Logging(logger_t *log)
{
    static char* log_descriptions[] = {
        "File cannot be opened",
        "Cannot allocate memory",
        "SUCCESS"
    };
    if (log)
    {
        fprintf(log->file, "%s in file: %s\n", log_descriptions[(int)log->status], log->filename);
    }

}

void CloseLog(logger_t *log)
{
    fclose(log->file);

}
