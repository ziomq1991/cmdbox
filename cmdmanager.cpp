#include "cmdmanager.h"
#include "cmd.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

CmdManager::CmdManager(QObject *parent) :
    QThread(parent),
    NSF("/home/ziomq1991/Code/cpp/cmdbox/cmdbox-build-desktop-Desktop_Qt_4_7_4_for_GCC__Qt_SDK__Release/nsf.cmd"),
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
        }else{
            cmdList->append(Cmd(""));
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
    if(nameSortedFile->open(QFile::ReadOnly)){
        QTextStream nsfStream(nameSortedFile);
        QString line, cmd;

        while(!(line = nsfStream.readLine()).isEmpty()){
            QStringList tempList = line.split("##");
            if(tempList.count() > 1){
                line = tempList.at(0);
                cmd = tempList.at(1);
                nsList->append(Cmd(line, cmd));
            }else{
                nsList->append(Cmd(line, line));
            }
        }
    }else{
        nameSortedFile->open(QFile::WriteOnly);
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
