#include <iostream>
#include <sstream>

namespace GTC
{

class invalid_format : std::exception
{
private:
    int line;
    std::string message;
    std::string line_str;

public:
    invalid_format() : line(0), message("none specified"), line_str("none specified")
    {
    }

    invalid_format(int line, std::string message, std::string line_str) : line(line), message(message), line_str(line_str)
    {
    }

    virtual const char *what()
    {
        std::stringstream s;
        s << "At line " << this->line << ": [" << this->line_str << "]\n" << this->message;
        return s.str().c_str();
    }

    int get_line() const
    {
        return this->line;
    }

    void change_line(int line)
    {
        this->line = line;
    }
};

};
