#include "myproject/Logger.hpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>

namespace myproject::log
{

namespace
{
std::string_view prefix_tag(Level level)
{
    switch (level)
    {
    case Level::Log:
        return "[log] ";
    case Level::Warn:
        return "[warn] ";
    case Level::Error:
        return "[error] ";
    }
    return "";
}

} // namespace

void write(Level level, std::string_view message)
{
    const std::string_view prefix = prefix_tag(level);
    char buffer[1024];

    if (message.size() <= sizeof(buffer) - prefix.size() - 1)
    {
        std::memcpy(buffer, prefix.data(), prefix.size());
        if (!message.empty())
        {
            std::memcpy(buffer + prefix.size(), message.data(), message.size());
        }
        const auto size = prefix.size() + message.size();
        buffer[size] = '\n';
        std::fwrite(buffer, 1, size + 1, stderr);
    }
    else
    {
        std::string record(prefix);
        record.append(message);
        record.push_back('\n');
        std::fwrite(record.data(), 1, record.size(), stderr);
    }
}

} // namespace myproject::log
