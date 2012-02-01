#ifndef CMDBOX_H
#define CMDBOX_H

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QPushButton;
class CmdManager;

namespace Ui {
class CmdBox;
}

class CmdBox : public QDialog
{
    Q_OBJECT
    
private:
    Ui::CmdBox *ui;
    int itemCount;
    int selectedItem;
    int iconSize;
    CmdManager *cmdManager;    

public:
    explicit CmdBox(QWidget *parent = 0);
    ~CmdBox();

protected:    
    void initSlotsAndSignals();
    void initList();    
    void keyPressEvent(QKeyEvent * event);
    QIcon getIconFromCmd(QString cmd);


    inline void setItemCount(int count){ itemCount = count; }
    inline void changeSelectedItem(int change){ selectedItem += change; }
    inline void setSelectedItem(int selected){ selectedItem = selected; }

    inline int getItemCount()const { return itemCount; }
    inline int getSelectedItem()const { return selectedItem; }
    inline CmdManager *getCmdManager()const{ return cmdManager; }
    inline QListWidget *getCmdList();
    inline QListWidgetItem *getItem(int row);
    inline QLineEdit *getCmdEdit();
    inline QPushButton *getRunButton();

public slots:
    void updateLastItem(QString str);
    void execCommand();
    void updateList(QString txt);
};

#endif // CMDBOX_H
