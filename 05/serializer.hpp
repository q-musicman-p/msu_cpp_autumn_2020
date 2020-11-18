#pragma once

#include <string>
#include <istream>
#include <ostream>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
public:
    explicit Serializer(std::ostream& out): out_(out) {}

    Serializer(const Serializer&) = default;
    Serializer(Serializer&&) = default;

    Serializer& operator=(const Serializer&) = default;
    Serializer& operator=(Serializer&&) = default;

    ~Serializer() = default;

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
private:
    static constexpr char separator = ' ';
    std::ostream& out_;

    template <class T, class... ArgsT>
    Error process(T& head, ArgsT... agrs)
    {
        out_ << std::boolalpha << head << separator;   

        process(agrs...);
    
        return Error::NoError;
    }

    template <class T>
    Error process(T& head)
    {
        out_ << std::boolalpha << head;
        
        return Error::NoError;  
    }
};


// =======================================================================
// =======================================================================
// =======================================================================


class Deserializer
{
public:
    explicit Deserializer(std::istream& in): in_(in) {}

    Deserializer(const Deserializer&) = default;
    Deserializer(Deserializer&&) = default;

    Deserializer& operator=(const Deserializer&) = default;
    Deserializer& operator=(Deserializer&&) = default;

    ~Deserializer() = default;

    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }
private:
    template <class T, class... ArgsT>
    Error process(T& head, ArgsT&... args)
    {
        std::string text = "";
        in_ >> text;

        const Error error = unparse(text, head);

        if (error == Error::NoError) return process(args...);
        else return error;
    }

    template <class T>
    Error process(T& head)
    {
        std::string text = "";
        in_ >> text;

        return unparse(text, head);
    }

    Error unparse(const std::string& text, bool& head)
    {
        if (text == "true")
        {
            head = true;
        }
        else if (text == "false")
            head = false;
        else
            return Error::CorruptedArchive;
        
        return Error::NoError;
    }

    Error unparse(const std::string& text, u_int64_t& head)
    {
        try
        {
            if ((text == "true") || (text == "false")) return Error::CorruptedArchive;
            head = std::stoull(text);
        }
        catch(const std::exception& e)
        {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }
    
    static constexpr char separator = ' ';

    std::istream& in_;
};