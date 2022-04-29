#include "model.h"


/** \brief - Подсчёт колличества строк
 *
 * \param text - буффер с текстом
 * \param size - размер буффера
 * \return num - число строк
 *
 */
unsigned long CountStrings(const char *text, size_t size)
{

    size_t num = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (text[i] == '\n')
        {
            num++;
            size--;
        }
    }

    if (text[size - 1] != '\n')
    {
        num++;
    }

    return num;
}

/** \brief - Заполнение массива строк и максимальной длинны строки
 *
 * \param text_model - модель изначальных данных
 * \param text - буффер с текстом
 * \param size - размер буффера
 *
 */
void ParseText(textModel_t *text_model,const char *text,size_t size)
{
    size_t i=0;
    size_t str=0;
    size_t length=0;
    text_model->strings[str]=0;
    text_model->max_length=0;
    for(i=0;i<size;i++)
    {
        if(text[i]=='\n')
        {
            if(text_model->num_strings>1)
            {
                size--;
                str++;
                text_model->strings[str] = i+1;
                length = text_model->strings[str] - text_model->strings[str-1];

                text_model->max_length = (text_model->max_length > length) ? text_model->max_length : length;


            }
        }
    }
    text_model->strings[str+1] = i+1;
    text_model->buff[text_model->strings[str+1]]='\0';
    if(text[size]!='\n')
    {

        if(text_model->num_strings>1)
        {
            length = size - 1 - text_model->strings[text_model->num_strings-1];
            text_model->max_length = (text_model->max_length > length) ? text_model->max_length : length;
        }
    }

}

/** \brief Первая инициализация модели данных
 *
 * \param view_model - модель отображения текста
 * \param text_model - модель изначальных данных
 * \param log - логгер
 *
 */
void InitTextModel(FILE *f,textModel_t *text_model,logger_t *log)
{
    fseek(f,0,SEEK_END);
    text_model->buff_size = ftell(f);


    rewind(f);
    text_model->buff = (char*)calloc(sizeof(char),(text_model->buff_size+1));
    if(text_model->buff == NULL)
    {
        log->status = MEMORY_ERROR;
    }
    else
        fread(text_model->buff,sizeof(char),text_model->buff_size,f);




    text_model->num_strings = CountStrings(text_model->buff,text_model->buff_size);
    text_model->strings = (size_t*)malloc(sizeof(size_t)*(text_model->num_strings+1));
    if(text_model->strings==NULL)
        log->status=MEMORY_ERROR;
    else
        ParseText(text_model,text_model->buff,text_model->buff_size);

}


void freeTextModel(textModel_t *text_model)
{
    free(text_model->buff);
    free(text_model->strings);
}
