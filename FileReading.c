#include "FileReading.h"


void FileOpen(PCHAR fileName,HWND hwnd,textModel_t *text_model,logger_t *log)
{
    if (!hwnd || !fileName)
    {
        return;
    }
    FILE *f = fopen(fileName,"r");
    log->filename = fileName;
    FileReader(hwnd,f,text_model,log);

    fclose(f);
}
void FileReader(HWND hwnd,FILE *f,textModel_t *text_model,logger_t *log)
{
    if(f!=NULL)
    {
        log->status = SUCCESS;
        InitTextModel(f,text_model,log);
    }
    else
    {
        log->status = FILE_OPENING_ERROR;
        Logging(log);
        MessageBox(hwnd, "File open error", NULL, MB_OK);
    }

    if (log->status == MEMORY_ERROR)
    {
        Logging(log);
        MessageBox(hwnd, "Can not allocate memory", NULL, MB_OK);
    }
}

void FileOpenDlg(HWND hwnd,textModel_t *text_model,viewModel_t *view_model,logger_t *log)
{
    char szFile[256] = "";
    char szDirName[256] = "";
    char szFileTitle[256] = "";
    char szFilter[256] =
    "Text Files\0*.txt\0"
    "Any Files\0*.*\0";
    OPENFILENAME ofn;
    FILE *f;
    memset(&ofn, 0, sizeof(OPENFILENAME));
    GetCurrentDirectory(sizeof(szDirName), szDirName);
    szFile[0] = '\0';
    ofn.lStructSize     = sizeof(OPENFILENAME);
    ofn.hwndOwner       = hwnd;
    ofn.lpstrFilter     = szFilter;
    ofn.lpstrInitialDir = szDirName;
    ofn.nFilterIndex    = 1;
    ofn.lpstrFile       = szFile;
    ofn.nMaxFile        = sizeof(szFile);
    ofn.lpstrFileTitle  = szFileTitle;
    ofn.nMaxFileTitle   = sizeof(szFileTitle);
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


    if(GetOpenFileName(&ofn) == TRUE)
    {
        if (*ofn.lpstrFile)
        {
            f = fopen(ofn.lpstrFile, "r");
            log->filename=ofn.lpstrFile;
            FileReader(hwnd,f,text_model,log);
            fclose(f);
            view_model->numStrs=text_model->num_strings;
            InvalidateRect(hwnd, NULL, TRUE);
            SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
            SendMessage(hwnd, WM_SIZE, 0, 0);
            SetWindowPos(hwnd, HWND_TOP, 200, 200, 900, 700, 0);

        }
  }
}
