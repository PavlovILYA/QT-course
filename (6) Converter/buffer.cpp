#include "buffer.h"

Buffer::Buffer(int size)
{
    m_size = size;
    m_buffer.resize(m_size);
    m_state_buffer.resize(m_size);
    for (int i=0; i<m_size; i++)
        m_state_buffer[i] = 1;
    m_type_buffer.resize(m_size);
    m_number_buffer.resize(m_size);
}

Buffer::~Buffer()
{

}

int Buffer::getSize()
{
    return m_size;
}

char Buffer::getChar(int i)
{
    return m_buffer[i];
}

int Buffer::getNumberOne(int i)
{
    return m_number_buffer[i];
}

int Buffer::getTypeOne(int i)
{
    return m_type_buffer[i];
}

int Buffer::getStateOne(int i)
{
    return m_state_buffer[i];
}

void Buffer::setChar(int i, char ch)
{
    m_buffer[i] = ch;
}

void Buffer::setNumberOne(int i, int num)
{
    m_number_buffer[i] = num;
}

void Buffer::setTypeOne(int i, int type)
{
    m_type_buffer[i] = type;
}

void Buffer::setStateOne(int i, int state)
{
    m_state_buffer[i] = state;
}
