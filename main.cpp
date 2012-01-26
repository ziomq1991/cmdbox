#include <QApplication>
#include "cmdbox.h"

int main(int argc, char **argv){
    QApplication app(argc, argv);

    CmdBox cmdBox;
    cmdBox.show();

    return app.exec();
}
