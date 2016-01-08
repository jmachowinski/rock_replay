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
    
    // timer
    speedTimer = new QTimer();
    speedTimer->setInterval(350);
    
    // progress bar
    ui.progressBar->setMinimum(0);
    
    // labels
    
    
    // icons
    playIcon.addFile(QString::fromUtf8(":/icons/icons/Icons-master/picol_latest_prerelease_svg/controls_play.svg"), QSize(), QIcon::Normal, QIcon::On);
    pauseIcon.addFile(QString::fromUtf8(":/icons/icons/Icons-master/picol_latest_prerelease_svg/controls_pause.svg"), QSize(), QIcon::Normal, QIcon::On);
    
    // slot connections
    QObject::connect(ui.playButton, SIGNAL(clicked()), this, SLOT(togglePlay()));
    QObject::connect(speedTimer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    
    speedTimer->start();
    
}

ReplayGui::~ReplayGui()
{
    delete replayHandler;
}


void ReplayGui::initReplayHandler(int argc, char* argv[])
{
    replayHandler = new ReplayHandler(argc, argv);
    
    // progress bar
    ui.progressBar->setMaximum(replayHandler->getReplayFactor());
    
    // labels
    ui.label_sample_count->setText(QString(("/ " + std::to_string(replayHandler->getMaxIndex())).c_str()));
    
    // window title
    if(argc > 1) 
        this->setWindowTitle(QString(argv[1]));
}


void ReplayGui::updateTaskNames()
{
    for(const std::pair<std::string, LogTask*>& cur : replayHandler->getAllLogTasks())
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
    if(ui.playButton->isChecked())
    {
        ui.playButton->setIcon(pauseIcon);
        ui.playButton->setChecked(true);
    }
    else
    {
        ui.playButton->setIcon(playIcon);
        ui.playButton->setChecked(false);
    }
}


void ReplayGui::updateProgressBar()
{
    ui.progressBar->setValue(replayHandler->getCurrentSpeed() * ui.progressBar->maximum());
}

