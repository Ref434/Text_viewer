#ifndef CARRIAGE_H
#define CARRIAGE_H

#include "viewer.h"
#include "model.h"

/** \brief - ������� ��� ��������� ������� ������� ��� ������� ������(���������)
 *
 * \param carriage - �������
 * \param view_model - ������ ����������� ������
 * \param text_model - ������ ����������� ������
 *
 */

void carriageUP(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);

void carriageDOWN(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);

void carriageLEFT(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);

void carriageRIGHT(HWND hwnd,carriage_t* carriage, viewModel_t* view_model,textModel_t *text_model);


#endif // CARRIAGE_H
