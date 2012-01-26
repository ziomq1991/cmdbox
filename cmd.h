#ifndef CMD_H
#define CMD_H

#include <QString>

class Cmd : public QString
{    
private:
    QString base;
    QString cmd;
public:
    Cmd(const QString& str): QString(str){}
    Cmd(const QString& str, const QString& cmd): QString(str), base(str), cmd(cmd){}

    static int exec(const QString& cmd);    
    int exec();
    
};

#endif // CMD_H
