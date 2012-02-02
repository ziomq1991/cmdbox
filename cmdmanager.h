#ifndef CMDMANAGER_H
#define CMDMANAGER_H

#include <QThread>
#include <QList>

class QFile;
class Cmd;

class CmdManager : public QThread
{
    Q_OBJECT
private:            
    const QString NSF;
    const QString SEPARATOR;
    const QString CMDBOXLIST;
    const QString DEFAULT_TEXT_EDITOR;
    QFile *nameSortedFile;
    QList<Cmd> *cmdList;
    QList<Cmd> *nsList;
public:        
    explicit CmdManager(QObject *parent=0);
    ~CmdManager();

    void run();
    QList<Cmd> *getCmdList(int count, QString key="");
    void execCommand(int index, QString cmd);
    void loadFiles();    
    int findTheBest(QString key);
    
};

#endif // CMDMANAGER_H
