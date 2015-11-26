#pragma once
#include <string>
#include <pocolog_cpp/InputDataStream.hpp>

class PortHandle;

namespace Typelib {
    class Registry;
}

namespace RTT {
    class TaskContext;
    namespace base {
        class OutputPortInterface;
    }
}

class LogTask
{
    std::vector<PortHandle *> handles;
    RTT::TaskContext *task;
public:
    LogTask(const std::string &name);
    
    bool addStream(const pocolog_cpp::InputDataStream &stream);
    void replaySample(pocolog_cpp::InputDataStream& stream, size_t sampleNr);
    bool createReplayPort(const std::string& portname, const std::string& typestr, PortHandle& handled);
    bool addReplayPort(RTT::base::OutputPortInterface* writer, std::string const& stream_name);
};


