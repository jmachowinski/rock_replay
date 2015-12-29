#ifndef REPLAY_GUI_H
#define REPLAY_GUI_H

#include "rock_replay/ReplayHandler.hpp"

#include <QMainWindow>
#include <QStringListModel>

#include "ui_main.h"

class ReplayGui : public QMainWindow
{
    Q_OBJECT

public:
    ReplayGui(QMainWindow *parent = 0);
    ~ReplayGui();
    
    void initReplayHandler(int argc, char* argv[]);
    void updateTaskNames();

private:
    Ui::MainWindow ui;
    ReplayHandler *replayHandler;
    
    // models
    QStringListModel *taskNameListModel;
    
    // lists
    QStringList *taskNameList;
    
    // icons
    QIcon playIcon, pauseIcon;
    
    
public slots:
    void togglePlay();

};

#endif // REPLAY_GUI_H
