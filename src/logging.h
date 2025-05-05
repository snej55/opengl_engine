#ifndef LOGGING_H
#define LOGGING_H

#include <fstream>
#include <iostream>
#include <string>

class Logger
{
public:
    Logger(std::string_view outPath)
     : m_path{outPath}
    {
        // open log file
        m_file = std::ofstream{m_path.c_str()};
        if (m_file.is_open())
        {
            std::cout << "Started log file at `" << m_path << "`" << std::endl;
            // redirect cout to log file
            std::cout.rdbuf(m_file.rdbuf());
        } else {
            std::cout << "LOGGER::LOGGER::ERROR: Error starting log file at `" << m_path << "`" << std::endl;
        }
    }

    ~Logger()
    {
        close();
    }

    void close()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

private:
    std::string m_path;
    std::ofstream m_file;
};

#endif 