#ifndef VIEWER_H
#define VIEWER_H
#include "model.h"
#include <windows.h>


/** \brief Тип отображения
 *
 * \param USUAL - без смещения
 * \param WRAP - со смещением
 *
 */
typedef enum view_mode{
    USUAL,
    WRAP,
}view_mode_t;


typedef struct side {
    long long int horizontal;
    long long int vertical;
}side_t;


/** \brief - Параметры шрифта
 *
 * \param charWidth - ширина символа
 * \param charHeight - высота символа
 *
 */
typedef struct fontParam{
    int charWidth;
    int charHeight;
}fontParam_t;



typedef struct wndParam{
    side_t size;
}wndParam_t;

/** \brief - Параметры скрола
 *
 * \param position - текущая позиция
 * \param maxPos - максимальное значение
 * \param difference - параметр для изменения позиции при получении сообщения
 *
 */
typedef struct scrollParam{
    side_t position;
    side_t maxPos;
    side_t difference;
}scrollParam_t;

typedef struct Carriage{
    side_t position;
}carriage_t;


/** \brief  - Модель отображения
 *
 * \param strings - Указатели на начало строк(изменяется при переключении режимов и изменении размеров окна при wrap)
 * \param numStrs - Максимальное число строк(изменяется при переключении режимов и изменении размеров окна при wrap)
 *
 */
typedef struct viewModel{
    view_mode_t mode;
    textModel_t *text_model;
    wndParam_t wndParam;
    scrollParam_t scrollParam;
    fontParam_t fontParam;
    size_t *strings;
    size_t numStrs;
}viewModel_t;

void InitViewModel(viewModel_t *view_model,textModel_t *text_model,TEXTMETRIC tm);
void FillStringsWrap(viewModel_t *view_model,textModel_t *text_model,carriage_t* carriage,size_t size);
void freeViewModel(viewModel_t *view_model);
void FillUsualMode(viewModel_t *view_model,textModel_t *text_model,carriage_t* carriage);
void CountStringsWrap(textModel_t *text_model,viewModel_t *view_model);
size_t findBufSize(size_t wndSize, size_t scrollVertPos , textModel_t *text_model);
size_t findNewScrollVertPos(size_t wndSize, size_t size , textModel_t *text_model);






#endif
