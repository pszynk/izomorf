/**
 *      @file  utils.cpp
 *     @brief  implementacja metod klasy pomocniczych
 *
 * Detailed description starts here.
 *=====================================================================================
 */


#include "utils.hpp"

std::vector<std::string>
&split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string>
split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::string &strip_space(
        std::string &s)
{
    s.erase(
            std::remove_if(
                s.begin(), s.end(),
                std::bind(std::isspace<char>, std::placeholders::_1, std::locale::classic())),
            s.end());
    return s;
}
