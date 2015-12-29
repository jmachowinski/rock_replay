#include "replay_gui.h"

ReplayGui::ReplayGui(QMainWindow *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    // initing  lists
    taskNameList = new QStringList();
    
    // initing models
    taskNameListModel = new QStringListModel(*taskNameList);
    ui.taskNameList->setModel(taskNameListModel);    
    
    QObject::connect(ui.playButton, SIGNAL(clicked()), this, SLOT(togglePlay()));
    
}

ReplayGui::~ReplayGui()
{
    delete replayHandler;
}


void ReplayGui::initReplayHandler(int argc, char* argv[])
{
    replayHandler = new ReplayHandler(argc, argv);
}


void ReplayGui::updateTaskNames()
{
    for(std::pair<std::string, LogTask*> cur : replayHandler->getAllLogTasks())
    {
        std::string taskName = cur.first;
        for(const std::string& portName : cur.second->getTaskContext()->ports()->getPortNames())
        {
            taskNameList->append(QString((taskName + "::" + portName).c_str()));
        }
       
    }

    taskNameListModel->setStringList(*taskNameList);
    
}


// #######################################
// ######### SLOT IMPLEMENTATION #########
// #######################################

void ReplayGui::togglePlay()
{
    replayHandler->toggle();
}
