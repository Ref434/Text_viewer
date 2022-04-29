#ifndef MODEL_H
#define MODEL_H

#include "model.h"
#include <stdlib.h>
#include <stdio.h>
#include "Logger.h"
#define MAX_NUM_LINES 0x2710
/** \brief  - ������ ����������� ������
 *
 * \param buff - ����� � ��������
 * \param strings - ������ ������ �����
 * \param buff_size - ������ ������
 * \param num_strings - ������ ����� ��� ��������
 * \param num_strings_wrap - ����� ����� �� ���������
 * \param max_length - ������������ ������ ������
 *
 */
typedef struct textModel
{
    char *buff;
    size_t *strings;
    size_t buff_size;
    size_t num_strings;
    size_t num_strings_wrap;
    size_t max_length;

}textModel_t;

void InitTextModel(FILE *f,textModel_t*text_model,logger_t *log);
void freeTextModel(textModel_t *text_model);


#endif
