#ifndef _SERIALIZER_HPP
#define _SERIALIZER_HPP

#include <stdint.h>

struct Data;

class Serializer {
   public:
    static uintptr_t serialize(Data* ptr);
    static Data*     deserialize(uintptr_t raw);

   private:
    Serializer();
    Serializer(const Serializer& other);
    Serializer& operator=(const Serializer& other);
    ~Serializer();
};

#endif