#pragma once

#include <string>
#include <sstream>


class FormatException : public std::exception
{
public:
    enum class Error
    {
        ParametrsCountError = 0,
        UncorrectFormatingError = 1,
        UncorrectNumberInBracketsError = 2,
        UndefinedError
    };

    explicit FormatException(int error_code) 
    {
        if ((error_code < 0) || (error_code > 2))
        {
            error_ = Error::UndefinedError;
        }
        else
        {
            error_ = Error(error_code);
        }
    }

    const Error& getError() const
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

int parametrs_count;

template <class T, class... Args>
std::string parse(const std::string& string, const T& head, const Args&... args);

std::string parse(const std::string& string);


template <class... Args>
std::string format(const std::string& string, const Args&... args)
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
            if (brakets_mode) throw FormatException(1);
            brakets_mode = true;
        }break;
        
        case '}': {
            if (!brakets_mode) throw FormatException(1);
            brakets_mode = false;
            
            if (brakets_int.empty()) throw FormatException(2);
            

            int b_int = -1;
            try
            {
                b_int = std::stoi(brakets_int.c_str());
            }
            catch(const std::exception& e)
            {
                throw FormatException(2);
            }
            if (b_int < 0) throw FormatException(2);

            if (b_int > parametrs_count) throw FormatException(0);

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
        if ((symbol == '{') || (symbol == '}')) throw FormatException(0);
    }
    
    return string;
}
