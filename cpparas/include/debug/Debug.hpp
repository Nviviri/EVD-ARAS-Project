#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <deque>
#include <memory>
#include <string>

namespace cpparas {

namespace Debug {
    const uint32_t MAX_MEMORY_LOG_LINES = 200;

    void println(const std::string& line);
    extern std::shared_ptr<std::deque<std::string>> lines;
}

} // namespace cpparas

#endif /* DEBUG_HPP */
