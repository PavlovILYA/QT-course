#ifndef BUFFER_H
#define BUFFER_H

#include <QVector>
#include <QMutex>

class Buffer
{
public:
    Buffer(int size);
    ~Buffer();
    int getSize();

    int getStateOne(int i);
    int getTypeOne(int i);
    char getChar(int i);
    int getNumberOne(int i);
    void setStateOne(int i, int state);
    void setTypeOne(int i, int type);
    void setChar(int i, char ch);
    void setNumberOne(int i, int num);
private:
    int m_size;
    QVector<char> m_buffer; // сам буфер
    QVector<int> m_state_buffer; // пустой(1), обработан(2) или нет(3)
    QVector<int> m_type_buffer; // принадлежность к файлу: 1, 2 или 3
    QVector<int> m_number_buffer; // номера символа в файле
};

#endif // BUFFER_H
