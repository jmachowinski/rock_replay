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
    speedTimer->setInterval(10);
    
    // progress bar
    ui.progressBar->setMinimum(0);
    ui.progressBar->setFormat("paused");
    ui.progressBar->setValue(0);

    // labels
    
    
    // icons
    playIcon.addFile(QString::fromUtf8(":/icons/icons/Icons-master/picol_latest_prerelease_svg/controls_play.svg"), QSize(), QIcon::Normal, QIcon::On);
    pauseIcon.addFile(QString::fromUtf8(":/icons/icons/Icons-master/picol_latest_prerelease_svg/controls_pause.svg"), QSize(), QIcon::Normal, QIcon::On);
    
    // slot connections
    QObject::connect(ui.playButton, SIGNAL(clicked()), this, SLOT(togglePlay()));
    QObject::connect(speedTimer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
    QObject::connect(ui.speedBox, SIGNAL(valueChanged(double)), this, SLOT(setSpeedBox()));
    QObject::connect(ui.speedSlider, SIGNAL(sliderReleased()), this, SLOT(setSpeedSlider()));
    
}

ReplayGui::~ReplayGui()
{
    delete replayHandler;
}


void ReplayGui::initReplayHandler(int argc, char* argv[])
{
    replayHandler = new ReplayHandler(argc, argv);
    
    // progress bar
    ui.progressBar->setMaximum(100);
    
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





int ReplayGui::boxToSlider(double val)
{
    if(val <= 1)
    {
        return val * 50;
    }
    else if(val <= 1000)
    {
        return (val / 40.0) + 50;
    }
    else
    {
        return (val / 400.0) + 75;
    }
}

double ReplayGui::sliderToBox(int val)
{
    if(val <= 50)
    {
        return val / 50.0;
    }
    else if(val <= 75)
    {
        return (val - 50) * 40.0;
    }
    else
    {
        return (val - 75) * 400.0;
    }
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
        ui.progressBar->setFormat("%p%");
        speedTimer->start();
    }
    else
    {
        ui.playButton->setIcon(playIcon);
        ui.playButton->setChecked(false);
        speedTimer->stop();
        ui.progressBar->setValue(0);
        ui.progressBar->setFormat("paused");

    }
}


void ReplayGui::updateProgressBar()
{
    ui.progressBar->setValue(replayHandler->getCurrentSpeed() * ui.progressBar->maximum());
}


void ReplayGui::setSpeedBox()
{
    double speed = ui.speedBox->value();
    replayHandler->setReplayFactor(speed);
    ui.speedSlider->setValue(boxToSlider(speed));
    
}

void ReplayGui::setSpeedSlider()
{
    double speed = sliderToBox(ui.speedSlider->value());
    replayHandler->setReplayFactor(speed);
    ui.speedBox->setValue(speed);
}


