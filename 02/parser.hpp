#pragma once

#include <functional>
#include <iostream>

class TokenParser
{
public:
    TokenParser():
        startCallback(defaultStartCallback),finishCallback(defaultFinishCallback),
        digitTokenCallback(defaultDigitTokenCallback), stringTokenCallback(defaultStringTokenCallback) {}

    void setStartCallback(const std::function<void()>& f) { startCallback = f; }

    void setFinishCallback(const std::function<void()>& f) { finishCallback = f; }

    void setDigitTokenCallback(const std::function<void(u_int64_t)>& f) { digitTokenCallback = f; }
    
    void setStringTokenCallback(const std::function<void(const std::string&)>& f) { stringTokenCallback = f; }

    void parse(const std::string& stringToParse) const
    {
        startCallback();

        std::string token = "";
        bool isDigitToken = true;

        for(const auto& sym : stringToParse)
        {   
            // if not space symbol --> increament token
            if ((sym != ' ') && (sym != '\n') && (sym != '\t'))
            {
                if (isDigitToken) { isDigitToken = isdigit(sym); }

                token += sym;
            }
            else if (token != "") // calling the callback function and reset token
            {
                if (isDigitToken) digitTokenCallback(std::stoull(token));
                else stringTokenCallback(token);

                token = "";
                isDigitToken = true;
            }
        }

        // Last token check if the space symbol wasnot last
        if (token != "")
        {
            if (isDigitToken) digitTokenCallback(std::stoull(token));
            else stringTokenCallback(token);
        }

        finishCallback();
    }

private:
    std::function<void()> startCallback;
    std::function<void()> finishCallback;
    std::function<void(u_int64_t)> digitTokenCallback;
    std::function<void(const std::string&)> stringTokenCallback;

    static void defaultStartCallback()
    {
        std::cout << "Start parsing!" << std::endl;
    }

    static void defaultFinishCallback()
    {
        std::cout << "Fisnish parsing!" << std::endl; 
    }

    static void defaultDigitTokenCallback(u_int64_t token)
    {
        std::cout << "Find a digit token: " << token << '\n'; 
    }

    static void defaultStringTokenCallback(const std::string& token)
    {
        std::cout << "Find a string token: " << token << '\n'; 
    }
};