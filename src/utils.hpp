/**
 *      @file  utils.hpp
 *     @brief  deklaracja funkcji pomocniczych
 *
 * Detailed description starts here.
 *
 * Funkcje pomocnicze, służące do obsługi stringów
 *
 *=====================================================================================
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

/**
 * @brief dzieli string na podstringi
 *
 * dzielenie stringa na podstawie znaku dzielącego
 *
 * @param s string wejściowy
 * @param delim znak podziału
 * @param elems wektor z postringami wynikowymi
 *
 * @return referencja na wektor elems
 */
std::vector<std::string>
&split(
        const std::string &s,
        char delim,
        std::vector<std::string> &elems);


/**
 * @brief dźieli string na podstringi
 *
 * dzielenie stringa na podstawie znaku dzielącego
 *
 * @param s string wejściowy
 * @param delim znak podziału
 *
 * @return wektor podstringów
 */
std::vector<std::string>
split(
        const std::string &s,
        char delim);


/**
 * @brief usuwa niewidzialne znaki ze stringa
 *
 * usuwanie znaku spacji, tabulacji, nowej linii ze stringa
 *
 * @param s string wejściowy
 *
 * @return string s bez białych znaków
 */
std::string &strip_space(
        std::string &s);

#endif /* end of include guard: UTILS_HPP */
