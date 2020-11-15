#pragma once

#include <string>
#include <sstream>

enum class Error
{
    NoError
};

template <class T, class... Args>
std::string parse(const std::string& string, const T& head, const Args&... args);

std::string parse(const std::string& string);


int parametrs_count;


template <class... Args>
static std::string format(const std::string& string, const Args&... args)
{
    parametrs_count = sizeof...(args);
    return parse(string, args...);
}

template <class T, class... Args>
std::string parse(const std::string& string, const T& head, const Args&... args)
{
    std::stringstream result;

    bool brakets_mode = false;
    std::string brakets_int;

    for (const auto& symbol : string)
    {
        switch (symbol)
        {
        case '{': {
            if (brakets_mode) throw;
            brakets_mode = true;
        }break;
        
        case '}': {
            if (!brakets_mode) throw;
            brakets_mode = false;
            
            if (brakets_int.empty()) throw;
            int b_int = std::atoi(brakets_int.c_str());
            if (b_int > parametrs_count) throw;

            if (b_int == parametrs_count - (sizeof...(args) + 1)) result << head;
            else result << '{' << brakets_int << '}';

            brakets_int = "";
        }break;
        
        default: {
            if (!brakets_mode) result << symbol;
            else brakets_int += symbol;
            }
        }
    }

    return parse(result.str(), args...);
}

std::string parse(const std::string& string)
{
    for (const auto& symbol : string)
    {
        if ((symbol == '{') || (symbol == '}')) throw;
    }
    
    return string;
}