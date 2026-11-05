#pragma once

#include <cstdint>
#include <format>
#include <string_view>
#include <utility>

namespace myproject::log
{

enum class Level : std::uint8_t
{
    Log,
    Warn,
    Error
};

/**
 * @brief Main log call.
 *
 * @param level Level enum for deciding prefix tag in log.
 * @param message string_view to log.
 */
void write(Level level, std::string_view message);

inline void log(std::string_view message)
{
    write(Level::Log, message);
}

template <typename... Args>
    requires(sizeof...(Args) > 0)
void log(std::format_string<Args...> fmt, Args&&... args)
{
    write(Level::Log, std::format(fmt, std::forward<Args>(args)...));
}

inline void warn(std::string_view message)
{
    write(Level::Warn, message);
}

template <typename... Args>
    requires(sizeof...(Args) > 0)
void warn(std::format_string<Args...> fmt, Args&&... args)
{
    write(Level::Warn, std::format(fmt, std::forward<Args>(args)...));
}

inline void error(std::string_view message)
{
    write(Level::Error, message);
}

template <typename... Args>
    requires(sizeof...(Args) > 0)
void error(std::format_string<Args...> fmt, Args&&... args)
{
    write(Level::Error, std::format(fmt, std::forward<Args>(args)...));
}

} // namespace myproject::log

// Diagnostic log macro that should generate no instructions when disabled.
#if !defined(NDEBUG) || defined(MYPROJECT_ENABLE_LOGGING)
#define DEBUG_LOG(...) ::myproject::log::log(__VA_ARGS__)
#else
#define DEBUG_LOG(...) ((void)0)
#endif
