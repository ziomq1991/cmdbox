#include "cmdmanager.h"
#include "cmd.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

CmdManager::CmdManager(QObject *parent) :
    QThread(parent),
    NSF("cmdbox.list"), SEPARATOR("##"), CMDBOXLIST("cmdbox-list"),
    DEFAULT_TEXT_EDITOR("gedit"),
    nameSortedFile(new QFile(NSF)),
    cmdList(new QList<Cmd>()), nsList(new QList<Cmd>())
{
    start();        
}

CmdManager::~CmdManager(){
    delete cmdList;
    delete nsList;
    delete nameSortedFile;
}

void CmdManager::run(){
    loadFiles();
}

QList<Cmd> *CmdManager::getCmdList(int count, QString key){
    cmdList->clear();
    int k=findTheBest(key);
    int listSize = nsList->count();
    for(int i=0; i<count; ++i)    {
        if(i < listSize){
            cmdList->append(nsList->at((k+i)%nsList->count()));
        }
    }
    return cmdList;
}

void CmdManager::execCommand(int index, QString cmd){
    if(index == cmdList->count()){
        Cmd::exec(cmd);
    }else{
        (static_cast<Cmd>(cmdList->at(index))).exec();
    }
}

void CmdManager::loadFiles(){
    QString line, cmd;
    if(nameSortedFile->open(QFile::ReadOnly)){
        QTextStream nsfStream(nameSortedFile);
        while(!(line = nsfStream.readLine()).isEmpty()){
            QStringList tempList = line.split(SEPARATOR);
            if(tempList.count() > 1){
                line = tempList.at(0);
                cmd = tempList.at(1);
                nsList->append(Cmd(line, cmd));
            }else{
                nsList->append(Cmd(line, line));
            }
        }
    }else{
        nameSortedFile->open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream nsfStream(nameSortedFile);
        line = CMDBOXLIST;
        cmd = DEFAULT_TEXT_EDITOR + " \"" + NSF + "\"";
        nsfStream<< line <<" "<<SEPARATOR<<" "<< cmd;
        nsList->append(Cmd(line, cmd));
    }
    nameSortedFile->close();
}

int CmdManager::findTheBest(QString key){
    int ip=0, ik=nsList->count(), i;

    while(ik > ip){
        i = (ik+ip)/2;
        if(key.compare(nsList->at(i), Qt::CaseInsensitive) <= 0){            
            ik = i;
        }else{            
            ip = i+1;
        }
    }
    return ik;
}
