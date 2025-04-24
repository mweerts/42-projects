#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>
#include <string>

class Brain {
   public:
    Brain();
    Brain(const Brain &other);
    ~Brain();
    Brain            &operator=(const Brain &other);
    const std::string getIdea(size_t index) const;
    void              setIdea(size_t index, std::string idea);

   private:
    std::string _ideas[100];
};

#endif