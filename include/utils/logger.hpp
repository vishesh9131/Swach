/*
 * @file logger.hpp
 * @brief Declaration of the Logger class for logging messages.
 *
 * This header defines the Logger class which provides static methods
 * to log messages at different severity levels using the spdlog library.
 *
 * @author Mohit Sharma (Coderero)
 * @email mohit@coderero.dev
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace utils
{

/**
 * @class Logger
 * @brief A simple logger class for logging
 * messages to the console.
 *
 * This class provides static methods
 * to log messages at different
 * severity levels.
 */

class Logger
{
  public:
    /**
     * @brief Initializes the logger with a console sink.
     */
    static std::shared_ptr<spdlog::logger> logger();

  private:
    /**
     * @brief Private constructor to prevent instantiation.
     */
    static std::shared_ptr<spdlog::logger> _instance;

    /**
     * @brief Private constructor to prevent instantiation.
     */
    static void init();
};
} // namespace utils

#endif // LOGGER_HPP
