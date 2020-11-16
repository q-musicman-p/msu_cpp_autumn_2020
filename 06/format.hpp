#pragma once

#include <string>
#include <sstream>
#include <exception>


class FormatException : public std::exception
{
public:
    enum class Error
    {
        ParametrsCountError,
        UncorrectFormatingError,
        UncorrectNumberInBracketsError,
    };

    explicit FormatException(Error error): error_(error) {}

    const Error getError() const noexcept
    {
        return error_;
    }

    const char* what() const noexcept override 
    {
        switch (error_)
        {
        case Error::ParametrsCountError:
            return "Need more parametrs!\n";

        case Error::UncorrectFormatingError:
            return "'{' and '}' should be used only for formating!\n";
        
        case Error::UncorrectNumberInBracketsError:
            return "In {...} shoud be index of parametr!\n";

        default:
            return "Undefined error.\n";
        }
    }
private:
    Error error_; 
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
            if (brakets_mode) throw FormatException(FormatException::Error::UncorrectFormatingError);
            brakets_mode = true;
        }break;
        
        case '}': {
            if (!brakets_mode) throw FormatException(FormatException::Error::UncorrectFormatingError);
            brakets_mode = false;
            
            if (brakets_int.empty()) throw FormatException(FormatException::Error::UncorrectNumberInBracketsError);
            

            int b_int = -1;
            try
            {
                b_int = std::atoi(brakets_int.c_str());
            }
            catch(const std::exception& e)
            {
                throw FormatException(FormatException::Error::UncorrectNumberInBracketsError);
            }
            if (b_int < 0) throw FormatException(FormatException::Error::UncorrectNumberInBracketsError);

            if (b_int > parametrs_count) throw FormatException(FormatException::Error::ParametrsCountError);

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
        if ((symbol == '{') || (symbol == '}')) throw FormatException(FormatException::Error::ParametrsCountError);
    }
    
    return string;
}