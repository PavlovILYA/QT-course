#include "customdata.h"

CustomData::CustomData():
     m_integer(-1),m_char()
{

}

CustomData::CustomData(int iv, char ch):
    m_integer(iv),m_char(ch)
{

}

void CustomData::set(int iv, char ch)
{
    m_integer = iv;
    m_char = ch;
}

