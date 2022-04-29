#include "viewer.h"

/** \brief Первая инициализация модели отображения текста
 *
 * \param view_model - модель отображения текста
 * \param text_model - модель изначальных данных
 *
 */
void InitViewModel(viewModel_t *view_model,textModel_t *text_model,TEXTMETRIC tm)
{
    if (!view_model && !text_model)
    {
        return;
    }
    view_model->text_model=text_model;
    view_model->fontParam.charWidth = tm.tmAveCharWidth;
    view_model->fontParam.charHeight = tm.tmAveCharWidth = tm.tmHeight + tm.tmExternalLeading;
    view_model->mode=USUAL;
    view_model->scrollParam.maxPos.horizontal=0;
    view_model->scrollParam.maxPos.vertical=0;
    view_model->scrollParam.position.horizontal=0;
    view_model->scrollParam.position.vertical=0;
    view_model->scrollParam.difference.horizontal=0;
    view_model->scrollParam.difference.vertical=0;
    view_model->numStrs=text_model->num_strings;
    view_model->strings=(size_t*)malloc(sizeof(size_t)*(view_model->numStrs+1));
    for(size_t i=0;i<view_model->numStrs;i++)
        view_model->strings[i]=text_model->strings[i];

}
void freeViewModel(viewModel_t *view_model)
{
    if(view_model->strings!=NULL)
        free(view_model->strings);
}

/** \brief - Пересчёт начала строк и изменение некоторых параметров для режима со смещением
 *
 * \param view_model - модель отображения текста
 * \param text_model - модель изначальных данных
 * \param carriage - каретка
 * \param size - количество строк для новой модели
 *
 */
void FillStringsWrap(viewModel_t *view_model,textModel_t *text_model,carriage_t* carriage,size_t size)
{
    carriage->position.horizontal=0;
    carriage->position.vertical=0;
    size_t wndSize,str=1,koeff=1,lenght;
    freeViewModel(view_model);

    view_model->strings = (size_t*)malloc(sizeof(size_t)*(size+1));

    wndSize=view_model->wndParam.size.horizontal/ view_model->fontParam.charWidth;
    view_model->strings[str]=0;

    for (size_t i = 0; i < text_model->num_strings; i++)
    {
        lenght =text_model->strings[i + 1] - text_model->strings[i];
        if(lenght<=wndSize)
        {
            view_model->strings[str]=text_model->strings[i+1];
            str++;
        }
        else
        {
            while(lenght > wndSize)
            {
                lenght -= wndSize;
                view_model->strings[str]=text_model->strings[i]+wndSize*koeff;
                str++;
                koeff++;
            }
            if(lenght<=wndSize)
            {
                view_model->strings[str]=text_model->strings[i+1];
                str++;
                koeff=1;
            }
        }
    }
    view_model->mode=WRAP;

    view_model->scrollParam.position.horizontal=0;
    view_model->scrollParam.maxPos.horizontal=0;
}

/** \brief - Пересчёт начала строк и изменение некоторых параметров для режима без смещением
 *
 * \param view_model - модель отображения текста
 * \param text_model - модель изначальных данных
 * \param carriage - каретка
 *
 */
void FillUsualMode(viewModel_t *view_model,textModel_t *text_model,carriage_t* carriage)
{
    carriage->position.horizontal=0;
    carriage->position.vertical=0;
    view_model->mode=USUAL;
    view_model->numStrs=text_model->num_strings;
    freeViewModel(view_model);
    view_model->strings=(size_t*)malloc(sizeof(size_t)*(view_model->numStrs+1));
    for(size_t i=0;i<view_model->numStrs;i++)
        view_model->strings[i]=text_model->strings[i];

}

/** \brief  - Пересчёт колличества строк при режиме со смещением
 *
 * \param view_model - модель отображения текста
 * \param text_model - модель изначальных данных
 *
 */

void CountStringsWrap(textModel_t *text_model,viewModel_t *view_model)
{

    size_t numWrap=0,wndSize=0,lenght=0;
    wndSize=view_model->wndParam.size.horizontal/ view_model->fontParam.charWidth;
    for (size_t i = 0; i < text_model->num_strings; i++)
    {

        lenght =text_model->strings[i + 1] - text_model->strings[i];
        if(lenght<=wndSize)
        {
            numWrap++;
        }
        else
        {
            while(lenght > wndSize)
            {
                lenght -= wndSize;
                numWrap++;
            }
            if(lenght<=wndSize)
                numWrap++;
        }
    }
    text_model->num_strings_wrap=numWrap;

}

/** \brief - Корректировка позиции скрола при изменении размера окна
 *
 * \param wndSize - размер окна
 * \param size - размер буфера до предыдущей позиции скрола(вычисляется в findStartLine)
 * \param text_model - модель изначальных данных
 * \return result - новая позиция скрола
 *
 */
size_t findNewScrollVertPos(size_t wndSize, size_t size , textModel_t *text_model)
{
    size_t result=0,lenght,resultStr=0,tmp;
    for(size_t i=0;i<size;i++)
    {
        if(text_model->buff[i]=='\n')
            result++;

    }

    for(size_t i=0;i<result;i++)
    {
        lenght =text_model->strings[i + 1] - text_model->strings[i];
        if(lenght>wndSize)
        {
            tmp=lenght/wndSize;
            resultStr+=tmp;
        }


    }
    result+=resultStr;

    return result;
}


/** \brief - Нахождение размера буфера до позиции скрола
 *
 * \param wndSize - размер окна
 * \param scrollVertPos - позиция крола
 * \param text_model - модель изначальных данных
 * \return размер буфера
 *
 */
size_t findBufSize(size_t wndSize, size_t scrollVertPos , textModel_t *text_model)
{
    size_t bufSize = 0, i, stringLenght = 0;

    for(i = 0; i < scrollVertPos; i++)
    {
        stringLenght = 0;
        while(text_model->buff[bufSize] != '\n')
        {
            if(stringLenght == wndSize)
                break;

            bufSize++;
            stringLenght++;
        }

        bufSize++;
    }

    return bufSize;
}
