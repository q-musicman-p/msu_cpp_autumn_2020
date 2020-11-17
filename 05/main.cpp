#include <iostream>
#include <assert.h>
#include <sstream>
#include <ostream>
#include "serializer.hpp"

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;
    uint64_t d;
    uint64_t e;
    bool f;
    bool g;
    uint64_t h;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c, d, e, f, g, h);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c, d, e, f, g, h);
    }
};

void testSerializer()
{
    Data x { 1, true, 2, 5, 912, false, true, 0};

    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    
    if (stream.str() != "1 true 2 5 912 false true 0") std::cout << "testSerializer has aborted" << std::endl;
}

void testDeserializer()
{
    Data x { 1, true, 2, 5, 912, false, true, 0};

    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);

    Data y {0, false, 5, 72, 12, true, true, 4};

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);

    if (!((err == Error::NoError) && (x.a == y.a) && (x.b == y.b) && (x.c == y.c) && (x.d == y.d) && (x.e == y.e) &&
        (x.f == y.f) && (x.g == y.g) && (x.h == y.h)))
        {
            std::cout << "testDeserializer has aborted" << std::endl;
        }
}

void testBadInputStream()
{
    Data y {0, false, 5, 72, 12, true, true, 4};

    std::stringstream ss;
    ss << "123 false false ERROR -543";

    Deserializer deserializer(ss);
    const Error err = deserializer.load(y);

    if (err != Error::CorruptedArchive) std::cout << "testBadInputStream has aborted" << std::endl;
}

int main()
{
    testSerializer();
    testDeserializer();
    testBadInputStream();
   
    std::cout << "Complete!" << std::endl;
    
    return 0;
}
