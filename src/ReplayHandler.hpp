#pragma once

#include <iostream>
#include <pocolog_cpp/MultiFileIndex.hpp>
#include <base/Time.hpp>
#include <orocos_cpp/PluginHelper.hpp>
#include <rtt/transports/corba/TaskContextServer.hpp>

#include "LogTask.hpp"

class ReplayHandler
{
  
public:
    ReplayHandler(int argc, char** argv);
    ~ReplayHandler();
    
    void replayAllSamples() const;
    
    void setReplayFactor(double factor);
    
private:  
    double replayFactor;
    std::vector<std::string> filenames;
    std::map<std::string, LogTask *> logTasks;
    std::vector<LogTask *> streamToTask;
    pocolog_cpp::MultiFileIndex *multiIndex;
    
    void replaySample(size_t index) const;
    const base::Time getTimeStamp(size_t globalIndex) const;
};