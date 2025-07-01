#include "utils/logger.hpp"

namespace utils
{
std::shared_ptr<spdlog::logger> Logger::_instance = nullptr;

std::shared_ptr<spdlog::logger> Logger::logger()
{
    if (!_instance)
    {
        init();
    }
    return _instance;
}

void Logger::init()
{
    _instance = spdlog::stdout_color_mt("console");
    _instance->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
    _instance->set_level(spdlog::level::trace);
}

} // namespace utils
