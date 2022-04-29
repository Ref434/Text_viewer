#ifndef FileReading_H
#define FileReading_H
#include "model.h"
#include "viewer.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "Logger.h"
#include <memory.h>
#include <commdlg.h>


void FileOpen(PCHAR fileName,HWND hwnd, textModel_t *text_model,logger_t *log);
void FileReader(HWND hwnd,FILE *f,textModel_t *text_model,logger_t *log);
void FileOpenDlg(HWND hwnd,textModel_t *text_model,viewModel_t *view_model,logger_t *log);


#endif
