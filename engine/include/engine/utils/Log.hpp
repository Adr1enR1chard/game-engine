#pragma once

#include <iostream>
#include <source_location>
#include <mutex>

namespace engine
{

    enum class LogLevel
    {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class Log
    {
    public:
        static void Print(const std::string &message, LogLevel level, bool logLocation = false,
                          const std::source_location &location = std::source_location::current());

    private:
        static std::mutex m_mutex;
    };

} // namespace engine