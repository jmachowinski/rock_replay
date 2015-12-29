#include "replay_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReplayGui gui;
    gui.initReplayHandler(argc, argv);
        
    
    gui.updateTaskNames();
    gui.show();
    return a.exec();
}
