#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>

class RPN {
   public:
    static void processInput(const std::string& input);

   private:
    RPN();
    RPN(const RPN&);
    RPN& operator=(const RPN&);
    ~RPN();
};

#endif