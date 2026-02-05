#include <engine/utils/Log.hpp>

namespace engine
{

    inline const char *LogLevelToString(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Critical:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }

    inline const char *LogLevelToColorCode(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return "\033[36m"; // Cyan
        case LogLevel::Info:
            return "\033[32m"; // Green
        case LogLevel::Warning:
            return "\033[33m"; // Yellow
        case LogLevel::Error:
            return "\033[31m"; // Red
        case LogLevel::Critical:
            return "\033[41m"; // Red background
        default:
            return "\033[0m"; // Reset
        }
    }

    constexpr const char *RESET_COLOR = "\033[0m";
    constexpr const char *LOG_LOCATION_COLOR = "\033[37m";
    constexpr const char *BOLD_TEXT = "\033[1m";
    constexpr const char *ITALIC_TEXT = "\033[3m";

    void Log::Print(const std::string &message, LogLevel level, bool logLocation,
                    const std::source_location &location)
    {
        std::cout << "[" << BOLD_TEXT << LogLevelToColorCode(level) << LogLevelToString(level) << RESET_COLOR << "] ";
        if (logLocation)
        {
            std::cout << ITALIC_TEXT << LOG_LOCATION_COLOR << location.function_name() << " (l." << location.line()
                      << "): " << RESET_COLOR;
        }
        std::cout << message << std::endl;
        fflush(stdout);
    }

    std::mutex Log::m_mutex;

} // namespace engine
