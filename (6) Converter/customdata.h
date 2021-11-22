#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include <QString>
#include <QObject>

class CustomData
{
public:
    CustomData();
    CustomData(int iv, char ch);
    void set(int iv, char ch);


    int m_integer;
    char m_char;
};
Q_DECLARE_METATYPE(CustomData)


#endif // CUSTOMDATA_H
