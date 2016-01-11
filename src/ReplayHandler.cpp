#include "ReplayHandler.hpp"

ReplayHandler::ReplayHandler(int argc, char** argv)
{
    RTT::corba::TaskContextServer::InitOrb(argc, argv);

    char *installDir = getenv("AUTOPROJ_CURRENT_ROOT");
    
    if(!installDir)
    {
//         std::cout << "Error, could not find AUTOPROJ_CURRENT_ROOT env.sh not sourced ?" << std::endl;
        throw std::runtime_error("Error, could not find AUTOPROJ_CURRENT_ROOT env.sh not sourced ?");
    }
    
    std::cout << "Loading all typekits " << std::endl;
    orocos_cpp::PluginHelper::loadAllPluginsInDir(std::string(installDir) + "/install/lib/orocos/gnulinux/types/");
    orocos_cpp::PluginHelper::loadAllPluginsInDir(std::string(installDir) + "/install/lib/orocos/types/");

    
    for(int i = 1; i < argc; i++)
    {
        filenames.push_back(argv[i]);
    }
    
    multiIndex = new pocolog_cpp::MultiFileIndex();
    
    RTT::types::TypeInfoRepository::shared_ptr ti = RTT::types::TypeInfoRepository::Instance();

    multiIndex->registerStreamCheck([&](pocolog_cpp::Stream *st){
        std::cout << "Checking " << st->getName() << std::endl;
        pocolog_cpp::InputDataStream *dataStream = dynamic_cast<pocolog_cpp::InputDataStream *>(st);
        if(!dataStream)
        {
            return false;
        }
        
        std::string typestr = dataStream->getType()->getName();
        RTT::types::TypeInfo* type = ti->type(typestr);
        if (! type)
        {
            std::cerr << "cannot find " << typestr << " in the type info repository" << std::endl;
            return false;
        }
        
        
        return true;
    }
    );
    
    multiIndex->createIndex(filenames);
    
    streamToTask.resize(multiIndex->getAllStreams().size());

    for(pocolog_cpp::Stream *st : multiIndex->getAllStreams())
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

        size_t gIdx = multiIndex->getGlobalStreamIdx(st); 
        if(gIdx > streamToTask.size())
            throw std::runtime_error("Mixup detected");

        streamToTask[gIdx] = task;
    }

    replayFactor = 1.;
    currentSpeed = replayFactor;
    curIndex = 0;
    finished = false;
    play = false;
    replayThread = new boost::thread(boost::bind(&ReplayHandler::replaySamples, boost::ref(*this)));
}


ReplayHandler::~ReplayHandler()
{
    delete multiIndex;
    delete replayThread;
}

void ReplayHandler::replaySample(size_t index) const
{
    try 
    {
        size_t globalStreamIndex = multiIndex->getGlobalStreamIdx(index);
        pocolog_cpp::InputDataStream *inputSt = dynamic_cast<pocolog_cpp::InputDataStream *>(multiIndex->getSampleStream(index));
//         std::cout << "Gidx is " << globalStreamIndex << std::endl;
        streamToTask[globalStreamIndex]->replaySample(*inputSt, multiIndex->getPosInStream(index)); 
    } 
    catch(...)
    {
        std::cout << "Warning: ignoring corrupt sample: " << index << "/" << multiIndex->getSize() << std::endl;
    }
}

void ReplayHandler::replaySamples()
{
    boost::unique_lock<boost::mutex> lock(mut);
    
    std::cout << "Replaying all samples" << std::endl;
    base::Time start(base::Time::now()), lastExecute(base::Time::now());
    
    size_t allSamples = multiIndex->getSize();
    
    while(!finished)
    {
        
        while(!play)
            cond.wait(lock);
        
        if(allSamples > 0 && curIndex <= allSamples)
        {
            
            try {
                curStamp = getTimeStamp(curIndex == 0 ? 0 : curIndex-1);
                nextStamp = getTimeStamp(curIndex);
                duration = nextStamp - curStamp;
            } catch (...) {
                std::cout << "getTimeStamp() failed:  curIndex: " << curIndex << ", allSamples: " << allSamples << std::endl;
                curIndex++;
                continue;
            }
            
            
            if(duration.toMicroseconds() < 0)
            {
                std::cout << "Warning: invalid sample order" << std::endl;
            }
            else
            {
                int64_t timeSinceLastExecute = (base::Time::now() - lastExecute).toMicroseconds();
                int64_t sleepDuration = (duration.toMicroseconds() / replayFactor);
                if(timeSinceLastExecute < sleepDuration)
                {
                    usleep(sleepDuration - timeSinceLastExecute);
                    currentSpeed = replayFactor;
                }
                else if(timeSinceLastExecute == sleepDuration)
                {
                    currentSpeed = replayFactor;
                }
                else if(duration.toMicroseconds() != 0) // in case of non-parallel samples
                {
                    currentSpeed = static_cast<double>((double)duration.toMicroseconds() / (double)timeSinceLastExecute);   
                }           
                
            }
                        
            //TODO check if chronological ordering is right
            replaySample(curIndex);
            
            curIndex++;
            lastExecute = base::Time::now();
        }
        else
            play = false;
        
    }
    
}

const base::Time ReplayHandler::getTimeStamp(size_t globalIndex) const
{    
    size_t globalStreamIndex = multiIndex->getGlobalStreamIdx(globalIndex);
    pocolog_cpp::Index &idx = multiIndex->getSampleStream(globalStreamIndex)->getFileIndex();
    return idx.getSampleTime(multiIndex->getPosInStream(globalIndex));
}



void ReplayHandler::setReplayFactor(double factor)
{
    this->replayFactor = factor;
}


void ReplayHandler::next()
{

}

void ReplayHandler::previous()
{

}


void ReplayHandler::setSampleIndex(uint index)
{

}

void ReplayHandler::toggle()
{
    if(!play) {
        play = true;
        cond.notify_one();
        std::cout << "Starting replay" << std::endl;
    } else {
        play = false;
        std::cout << "Stopping replay" << std::endl;
    }
}







