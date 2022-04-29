#include "carriage.h"
/** \brief - Функции для изменения позиции каретки при нажатии клавиш(стрелочек)
 *
 * \param carriage - каретка
 * \param view_model - модель отображения текста
 * \param text_model - модель изначальных данных
 *
 */

void carriageUP(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model)
{
    if(carriage->position.vertical+view_model->scrollParam.position.vertical!=0 )
    {
        if(carriage->position.vertical==0)
            SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);


        if(view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical-1+view_model->scrollParam.position.vertical]-2<carriage->position.horizontal+view_model->scrollParam.position.horizontal)
        {
            if(view_model->scrollParam.position.horizontal>=view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical-1+view_model->scrollParam.position.vertical]-2)
            {
                view_model->scrollParam.position.horizontal=view_model->strings[carriage->position.vertical+2+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-2;
                carriage->position.horizontal=0;
            }
            else
                carriage->position.horizontal=view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical-1+view_model->scrollParam.position.vertical]-2;
        }
        if(view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical-1+view_model->scrollParam.position.vertical]-2==-1)
        {
            view_model->scrollParam.position.horizontal=0;
            carriage->position.horizontal=0;

        }


        carriage->position.vertical--;


    }
}

void carriageDOWN(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model)
{
        if(carriage->position.vertical+view_model->scrollParam.position.vertical!=view_model->numStrs)
        {
            if(carriage->position.vertical==(view_model->wndParam.size.vertical/view_model->fontParam.charHeight)-1)
                SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);

            if(view_model->strings[carriage->position.vertical+2+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-2<carriage->position.horizontal+view_model->scrollParam.position.horizontal)
            {
                if(view_model->scrollParam.position.horizontal>=view_model->strings[carriage->position.vertical+2+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-2)
                {
                    view_model->scrollParam.position.horizontal=view_model->strings[carriage->position.vertical+2+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-2;
                    carriage->position.horizontal=0;

                }
                else
                    carriage->position.horizontal=view_model->strings[carriage->position.vertical+2+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-2;

            }

            if(view_model->strings[carriage->position.vertical+2+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-2==-1)
            {
                view_model->scrollParam.position.horizontal=0;
                carriage->position.horizontal=0;

            }

            carriage->position.vertical++;

        }
}

void carriageLEFT(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model)
{
    if(carriage->position.horizontal==0)
    {
        if(view_model->scrollParam.position.horizontal==0)
        {
            if(carriage->position.vertical==0)
                SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);

            if(carriage->position.vertical+view_model->scrollParam.position.vertical!=0 )
            {
                carriage->position.vertical--;
                    if(view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-2==-1)
                        carriage->position.horizontal=0;

                    else
                        carriage->position.horizontal=view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-2;

                if(view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-2>view_model->wndParam.size.horizontal/view_model->fontParam.charWidth-1)
                {
                    view_model->scrollParam.position.horizontal=carriage->position.horizontal;
                    view_model->scrollParam.position.horizontal = min(view_model->scrollParam.position.horizontal, view_model->scrollParam.maxPos.horizontal);
                    carriage->position.horizontal-=view_model->scrollParam.position.horizontal;
                }
            }
        }
       else
       {
            carriage->position.horizontal--;
            SendMessage(hwnd, WM_HSCROLL, SB_LINEUP, 0L);
       }
    }
    else
    {
        carriage->position.horizontal--;
    }

}

void carriageRIGHT(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model)
{

    if(view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-2==-1)
    {
        carriage->position.vertical++;
        carriage->position.horizontal=0;
        view_model->scrollParam.position.horizontal=0;

    }

    else if(carriage->position.horizontal+view_model->scrollParam.position.horizontal==view_model->strings[carriage->position.vertical+1+view_model->scrollParam.position.vertical]-view_model->strings[carriage->position.vertical+view_model->scrollParam.position.vertical]-2 )
    {
        if(carriage->position.vertical+view_model->scrollParam.position.vertical==(view_model->wndParam.size.vertical/view_model->fontParam.charHeight))
            SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
        else
        {

            carriage->position.vertical++;
            carriage->position.horizontal=0;
            view_model->scrollParam.position.horizontal=0;
        }
    }
    else
        carriage->position.horizontal++;


    if(carriage->position.horizontal==view_model->wndParam.size.horizontal/view_model->fontParam.charWidth+1)
        SendMessage(hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
}
