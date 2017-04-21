#ifndef TINYTEMPLATE_HPP
#define TINYTEMPLATE_HPP

#include <string>
#include <map>
#include <stdexcept>

namespace tinytemplate {
class render_error
        : public std::runtime_error
{
public:
    explicit render_error(const std::string &what_arg);
    explicit render_error(const char *what_arg);
};

std::string render(const std::string &source, const std::map<std::string, std::string> &variables, bool ignoreErrors=false);
}


#endif//TINYTEMPLATE_HPP
