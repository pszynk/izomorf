/**
 *      @file  vertex.hpp
 *     @brief  plik nagłówkowy klasy Vertex
 *
 * Detailed description starts here.
 *
 * Deklaracja klasy Vertex
 * Wierzchołek w grafie w reprezentacji list sąsiedztwa
 *=====================================================================================
 */

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <set>
#include <string>
#include <iostream>
#include <sstream>

/**
 * @brief klasa reprezentuje wierzchołek grafie w reprezentacji
 * list sąsiedztwa
 *
 * Klasa odpowiada liście sąsiedztwa jednego wierzchołka.
 *
 */
class Vertex
{
    public:
        /**
         * @brief typ po którym indeksowane są wierzchołki w grafie
         */
        typedef unsigned int idx_t;

        /**
         * @brief typ reprezentujący parę wejściowość - wyjściowość
         */
        typedef std::pair<unsigned int, unsigned int> deg_t;

        /**
         * @brief iterator wierzchołka
         *
         * Iterator po indeksach wierzchołków sąsiadujących z danym
         * wierzchołkiem
         */
        typedef std::set<idx_t>::const_iterator iterator;

    public:
        /**
         * @brief konstruktor wierzchołka o podanym indeksie
         *
         * @param _index indeks dla tworzonego wierzchołka
         */
        Vertex(idx_t _index);

        /**
         * @brief dodaj wierzchołek sąsiadujący
         *
         * @param adj referencja na dodawany wierzchołek
         *
         * @return czy dodano wierzchołek (nie dodano jeżeli
         * wierzchołek jest już sąsiadem)
         */
        bool addAdjacent(Vertex &adj);

        /**
         * @brief sprawdź czy wierzchołek o danym indeksie
         * jest sąsiadem
         *
         * @param v indeks potencjalnego sąsiada
         *
         * @return czy v to indeks wierzchołka sąsiadującego z danym
         * wierzchołkiem
         */
        bool isAdjacent(idx_t v) const;

        /**
         * @brief zwraca indeks danego wierzchołka
         *
         * @return indeks
         */
        idx_t getIndex() const;

        /**
         * @brief zwraca wyjściowość wierzchołka
         *
         * @return wyjściowość
         */
        unsigned int getOut() const;

        /**
         * @brief zwraca wejściowość wierzchołka
         *
         * @return wejściowość
         */
        unsigned int getIn() const;


        /**
         * @brief iterator po indeksach wierzchołków sąsiadujących
         *
         * @return iterator na początek listy wierzchołków
         */
        iterator begin() const;

        /**
         * @brief iterator po indeksach wierzchołków sąsiadujących
         *
         * @return iterator na koniec listy wierzchołków
         */
        iterator end() const;

        /**
         * @brief informacje na temat wierzchołka
         *
         * procedura pomocnicza
         *
         * @return string z info. wierzchołka
         */
        std::string getInfo() const;
    private:
        /**
         * @brief indeks wierzchołka
         */
        idx_t index;

        /**
         * @brief wejściowość i wyjściowość wierzchołka
         */
        deg_t degree;

        /**
         * @brief lista sąsiedztwa wierzchołka
         */
        std::set<idx_t> adjacent;

        /**
         * @brief procedura wywoływana gdy wierzchołek jest dodawany
         * do listy sąsiedztwa innego wierzchołka
         */
        void addNeighbour();
};

#endif /* end of include guard: VERTEX_HPP */
