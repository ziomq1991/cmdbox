#include "cmd.h"
#include "cstdlib"

int Cmd::exec(const QString &cmd){
    if(cmd.isEmpty()){
        return -1;
    }
    return system((cmd + " &").toAscii());

}

int Cmd::exec(){
    return system((cmd + " &").toAscii());
}
