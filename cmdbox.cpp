#include "cmdbox.h"
#include "ui_cmdbox.h"
#include "cmdmanager.h"
#include "cmd.h"

#include <QListWidget>
#include <QKeyEvent>

CmdBox::CmdBox(QWidget *parent) :
    QDialog(parent), ui(new Ui::CmdBox), itemCount(4), selectedItem(0),iconSize(50),
    FOLDER_ICON("folder"), INTERNET_ICON("applications-internet"),
    PLAY_ICON("media-playback-start"), RUN_ICON("system-run"),
    WARNING_ICON("dialog-warning"),
    DEFAULT_WEB_BROWSER("google-chrome"), DEFAULT_FOLDER_EXPLORER("nautilus"),
    cmdManager(new CmdManager())
{
    ui->setupUi(this);
    initList();
    initSlotsAndSignals();

    getCmdList()->setMinimumHeight(iconSize*(itemCount+1) + iconSize/2);
    setMinimumHeight(getCmdList()->height() + 80);
    setMaximumHeight(getCmdList()->height() + 80);
}

CmdBox::~CmdBox(){
    delete ui;
    delete cmdManager;
}

void CmdBox::initSlotsAndSignals(){
    QObject::connect(getCmdEdit(), SIGNAL(textChanged(QString)),
                     this, SLOT(updateLastItem(QString)));
    QObject::connect(getRunButton(), SIGNAL(clicked()),
                     this, SLOT(execCommand()));
    QObject::connect(getCmdEdit(), SIGNAL(textChanged(QString)),
                     this, SLOT(updateList(QString)));
}

void CmdBox::initList(){    
    getCmdList()->setIconSize(QSize(iconSize, iconSize));

    for(int i=0; i<getItemCount(); ++i){
        QListWidgetItem *item = new QListWidgetItem("", getCmdList());
        item->setIcon(QIcon::fromTheme(WARNING_ICON));
        item->setData(Qt::UserRole, i);
    }
    QListWidgetItem *item = new QListWidgetItem("", getCmdList());
    item->setIcon(QIcon::fromTheme(RUN_ICON));
    item->setData(Qt::UserRole, getItemCount());

    getCmdList()->item(0)->setSelected(true);    
    updateList("");
}

QListWidget *CmdBox::getCmdList(){
    return ui->cmdList;
}

QListWidgetItem *CmdBox::getItem(int row){
    return getCmdList()->item(row);
}

QLineEdit *CmdBox::getCmdEdit(){
    return ui->cmdEdit;
}

QPushButton *CmdBox::getRunButton(){
    return ui->runButton;
}

void CmdBox::keyPressEvent(QKeyEvent *event){
    bool arrowPressed = false;

    switch(event->key()){
    case Qt::Key_Down:
        if(event->modifiers() & Qt::ALT){

        }else{
            changeSelectedItem(1);
            arrowPressed = true;
        }
        break;
    case Qt::Key_Up:
        changeSelectedItem(-1);
        arrowPressed = true;
        break;
    default:
        QDialog::keyPressEvent(event);
        break;
    }

    if(arrowPressed){
        if(getSelectedItem() < 0){
            setSelectedItem(getItemCount());
        }else{
            setSelectedItem(getSelectedItem() % (getItemCount()+1));
        }
        getItem(getSelectedItem())->setSelected(true);
    }
    return QDialog::keyPressEvent(event);
}

QIcon CmdBox::getIconFromCmd(QString cmd){
    QString s = cmd.split(" ", QString::SkipEmptyParts).at(0);

    if(s.compare(DEFAULT_WEB_BROWSER) == 0){
        return QIcon::fromTheme(INTERNET_ICON);
    }
    if(s.compare(DEFAULT_FOLDER_EXPLORER) == 0){
        return QIcon::fromTheme(FOLDER_ICON);
    }
    return QIcon::fromTheme(PLAY_ICON);
}

void CmdBox::updateLastItem(QString str){
    getItem(getItemCount())->setText(str);
}

void CmdBox::execCommand(){
    cmdManager->execCommand(getSelectedItem(), getItem(getSelectedItem())->text());
}

void CmdBox::updateList(QString txt){
    QList<Cmd> *list = cmdManager->getCmdList(getItemCount(), txt);
    for(int i=0; i<getItemCount() && i<list->count(); ++i){
        getItem(i)->setText(list->at(i));
        getItem(i)->setIcon(getIconFromCmd(list->at(i).getCmd()));
    }
}
