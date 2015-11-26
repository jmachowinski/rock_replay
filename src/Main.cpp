#include <iostream>
#include <pocolog_cpp/MultiFileIndex.hpp>
#include <base/Time.hpp>
#include "LogTask.hpp"
#include <orocos_cpp/PluginHelper.hpp>
#include <rtt/transports/corba/TaskContextServer.hpp>

int main(int argc, char** argv)
{
    RTT::corba::TaskContextServer::InitOrb(argc, argv);

    char *installDir = getenv("AUTOPROJ_CURRENT_ROOT");
    
    if(!installDir)
    {
        std::cout << "Error, could not find AUTOPROJ_CURRENT_ROOT env.sh not sourced ?" << std::endl;
        return 1;
    }
    
    std::cout << "Loading all typekits " << std::endl;
    orocos_cpp::PluginHelper::loadAllPluginsInDir(std::string(installDir) + "/lib/orocos/gnulinux/types/");
    orocos_cpp::PluginHelper::loadAllPluginsInDir(std::string(installDir) + "/install/lib/orocos/types/");
    
    std::vector<std::string> filenames;
    
    for(int i = 1; i < argc; i++)
    {
        filenames.push_back(argv[i]);
    }
    
    pocolog_cpp::MultiFileIndex multiIndex(filenames);
    
    std::map<std::string, LogTask *> logTasks;
    std::vector<LogTask *> streamToTask;
    streamToTask.resize(multiIndex.getAllStreams().size());

    for(pocolog_cpp::Stream *st : multiIndex.getAllStreams())
    {
        pocolog_cpp::InputDataStream *inputSt = dynamic_cast<pocolog_cpp::InputDataStream *>(st);
        if(!inputSt)
            continue;
        
        std::string taskName = st->getName();
        taskName = taskName.substr(0, taskName.find_last_of('.'));
        
        LogTask *task = nullptr;
        
        auto it = logTasks.find(taskName);
        if(it == logTasks.end())
        {
            task = new LogTask(taskName);
            logTasks.insert(std::make_pair(taskName, task));
        }
        else
        {
            task = it->second;
        }
        task->addStream(*inputSt);

        size_t gIdx = multiIndex.getGlobalStreamIdx(st); 
        if(gIdx > streamToTask.size())
            throw std::runtime_error("Mixup detected");

        streamToTask[gIdx] = task;
    }
    
    std::cout << "Replaying all samples" << std::endl;
    base::Time start(base::Time::now());

    size_t allSamples = multiIndex.getSize();
    int lastPercentage = 0;
    
    for(size_t i = 0; i < allSamples; i++)
    {
        size_t globalStreamIndex = multiIndex.getGlobalStreamIdx(i);
        pocolog_cpp::InputDataStream *inputSt = dynamic_cast<pocolog_cpp::InputDataStream *>(multiIndex.getSampleStream(i));

        std::cout << "Gidx is " << globalStreamIndex << std::endl;
        streamToTask[globalStreamIndex]->replaySample(*inputSt, multiIndex.getPosInStream(i));
        
        int curPercentag = (i * 100 / allSamples);
        if(lastPercentage != curPercentag)
        {
            lastPercentage = curPercentag;
            std::cout << "\r" << lastPercentage << "% Done" << std::flush;
        }
        
    }

    
    return 0;
}
