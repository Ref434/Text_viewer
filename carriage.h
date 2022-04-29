#ifndef CARRIAGE_H
#define CARRIAGE_H

#include "viewer.h"
#include "model.h"

/** \brief - Функции для изменения позиции каретки при нажатии клавиш(стрелочек)
 *
 * \param carriage - каретка
 * \param view_model - модель отображения текста
 * \param text_model - модель изначальных данных
 *
 */

void carriageUP(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);

void carriageDOWN(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);

void carriageLEFT(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);

void carriageRIGHT(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);


#endif // CARRIAGE_H
