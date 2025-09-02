#include <string>

#include "Serializer.hpp"
#include <iostream>

struct Data {
    std::string firstName;
    std::string lastName;
    int         age;
};

int main() {
    Data* data = new Data;
	std::cout << "original Data ptr: " << data << std::endl;
    data->firstName = "John";
    data->lastName = "Doe";
    data->age = 30;

    uintptr_t raw = Serializer::serialize(data);
	std::cout << "Serialized Data ptr: " << raw << std::endl;

    Data* deserializedData = Serializer::deserialize(raw);
	std::cout << "Deserialized Data ptr: " << deserializedData << std::endl;

	
	std::cout << "First Name: " << deserializedData->firstName << std::endl;
	std::cout << "Last Name: " << deserializedData->lastName << std::endl;
	std::cout << "Age: " << deserializedData->age << std::endl;

    delete data;
    return 0;
}