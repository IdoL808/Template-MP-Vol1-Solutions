#include <iostream>

#include "pair.hpp"

#include <string>

using namespace std::string_literals;

int main()
{
    Pair intP(2, 3);
    // Using string literals to make the compiler deduce this type to std::string and not char[2]
    Pair stringP("2"s, "3"s);
    Pair doubleP(2.0, 3.0);

    print_pair(intP);
    print_pair(stringP);
    print_pair(doubleP);
    std::cout << doubleP.get<0b000000000000000001>() << std::endl;
}


/*
 Learned:
  1 - Template specializtion: declartion with 'template<>', defintion without
  2 - std::array as member: init in the MIL only witch brackets(std::array<int,1>{5})
  3 - string literals (using namespace std::string_literals;) - 0b010, "ido"s
  4 - CPP17 has type deduction for templates (as written in the code above)
  5 - Thought: API of template classes so there will be same usage of them, not matter which template they has.
  6 - If constexpr: Tells the compiler to decide in compile time the result, so if possible
      It will discard the if statment. Relevant to templates type dudections.
*/