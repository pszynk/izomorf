/**
 *      @file  graph.hpp
 *     @brief  plik nagłówkowy klasy Graph
 *
 * Detailed description starts here.
 *
 * Deklaracja klasy Graph i klas wewnętrznych klasy Graph
 *
 *=====================================================================================
 */

#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <iostream>
#include <sstream>
#include <string>

#include <fstream>
#include <map>
#include <vector>
#include <stack>
#include <initializer_list>
#include <stdexcept>
#include <chrono>
#include <random>
#include <cmath>

#include "vertex.hpp"
#include "utils.hpp"



/**
 * @brief znak komentarza w plikach z rep. grafu
 */
#define COM_SIGN "#"

/**
 * @brief znak oddzielający w plikach z rep. grafu
 */
#define DELIMITER_CHAR ','

/**
 * @brief maksymalna liczba losowań przy generowanie grafu losowego
 *
 * Liczba możliwych nieudanych prób przy losowaniu nowej krawędzi
 * do grafu losowego.
 */
#define MAX_RANDOM_FAILS (1000*10)

/**
 * @brief klasa implementuje graf w reprezentacji listowej
 *
 * Klasa Graph to implementacja grafu w reprezentacji list sąsiedztwa.
 * Pozwala na:
 * * ręczne generowanie grafu
 * * wczytywanie i zapisywanie grafu do pliku
 * * generowanie losowego grafu
 *
 * Wierzchołki grafu identyfikowane są typem \link Graph::label_t \endlink, ale
 * wewnątrz grafu są indeksowane typem \link Vertex::idx_t \endlink. Pozwala
 * to na łatwe wygenerowanie grafów izomorficznych. Wystarczy zmienić
 * mapowanie znaczników na indeksy.
 */
class Graph
{
    public:
        /**
         * @brief znacznik wierzchołka w grafie
         *
         * Identyfikuje wierzchołki w grafie. Zewnętrzne
         * oznakowanie wierzchołków.
         */
        typedef unsigned int label_t;

        /**
         * @brief zbiór znaczników wierzchołków
         */
        typedef std::set<label_t> vertex_set_t;

        /**
         * @brief iterator po zbiorze znaczników
         */
        typedef vertex_set_t::const_iterator iterator;

        /**
         * @brief struktura pomocnicza reprezentująca krawędź skierowaną w grafie
         *
         * Prosta struktura do reprezentacji krawędzi skierowanych. Wykorzystywania do
         * porównywania krawędzi np. przy sortowaniu.
         */

        /**
         * @brief ścieżka dfs po grafie
         *
         * Wykorzystywane przy przechodzeniu grafu wgłąb
         */
        typedef std::vector<label_t> dfs_path;

        /**
         * @brief zbiór wcześniej odwiedzonych wierzchołków
         *
         * Wykorzystywane przy przechodzeniu grafu wgłąb
         */
        typedef std::set<label_t> dfs_visited;

        /**
         * @brief struktura reprezentuje krawędź skierowaną w grafie
         */
        struct Edge
        {
            /**
             * @brief konstruktor struktury Edge
             *
             * @param _source znacznik wierzchołka źródłowego
             * @param _target znacznik wierzchołka docelowego
             */
            Edge(Graph::label_t _source, Graph::label_t _target):
                source(_source), target(_target)
            {};

            Graph::label_t source;
            Graph::label_t target;

            /**
             * @brief porównywanie krawędzi
             *
             * krawędzie są równe jeżeli źródło i cel są takie same
             *
             * @param e druga krawędź
             *
             * @return czy są to te same krawędzie
             */
            bool operator == (const Edge &e) const {
                return (this->source == e.source && this->target == e.target);
            };

            /**
             * @brief leksykograficzny porządek na krawędziach
             *
             * @param e druga krawędź
             *
             * @return czy krawędź jest mniejsza od krawędzi e
             */
            bool operator < (const Edge& e) const {
                if (this->source == e.source) {
                    return (this->target < e.target);
                }
                return (this->source < e.source);
            };
        };

        /**
         * @brief zbiór krawędzi
         */
        typedef std::set<Edge> edge_set_t;


        /**
         * @brief klasa pomocnicza, iterator po znacznikach wierzchołków
         * sąsiadujących z wierzchołkiem
         *
         * Ponieważ wewnątrz grafu wierzchołki są indeksowane w inny sposób niż
         * z zewnątrz, potrzebny jest iterator mapujący indeks wierzchołka na jego
         * znacznik
         */
        class AdjIter
        {
            private:
                /**
                 * @brief mapowanie za pomocą mapy index -> znacznik
                 */
                typedef std::map<Vertex::idx_t, label_t> Map;

            public:
                /**
                 * @brief konstruktor potrzebuje mapy i wierzchołka
                 *
                 * @param _idx_label_map mapa do mapowania
                 * @param _vit wierzchołek
                 */
                AdjIter(const Map &_idx_label_map, const Vertex::iterator &_vit):
                    idx_label_map(_idx_label_map), vit(_vit)
            {};
                AdjIter &operator++() {++vit; return *this;};
                const AdjIter operator++(int) {AdjIter temp = *this; ++vit; return temp;};
                const label_t &operator*() {return this->idx_label_map.at(*vit);}
                const label_t *operator->() {return &this->idx_label_map.at(*vit);}
                bool operator==(const AdjIter & rhs) { return (this->vit == rhs.vit && this->idx_label_map == rhs.idx_label_map);};
                bool operator!=(const AdjIter & rhs) { return (this->vit != rhs.vit || this->idx_label_map != rhs.idx_label_map);};
            private:
                /**
                 * @brief mapa indeks -> znacznik
                 */
                const Map &idx_label_map;
                /**
                 * @brief iterator po indeksach wierzchołków sąsiadujących
                 */
                Vertex::iterator vit;
        };

    private:
        /**
         * @brief liczba wierzchołków w grafie
         */
        unsigned int vertex_count;

        /**
         * @brief liczba krawędzi w grafie
         */
        unsigned int edge_count;

        /**
         * @brief potęga t używana przy liczeniu 'stopnia' wierzchołka
         *
         * 10^t * wyjściowość + wejściowość
         */
        unsigned int __inv_power;

        /**
         * @brief pierwszy wolny indeks dla kolejnego wierzchołka grafu
         */
        Vertex::idx_t __first_free_idx;

        /**
         * @brief mapa indeks -> wierzchołek
         */
        std::map<Vertex::idx_t, Vertex> vertexes;
        /**
         * @brief wszystkie znaczniki w grafie
         */
        std::set<label_t> labels;
        /**
         * @brief mapa indeks -> znacznik
         */
        std::map<Vertex::idx_t, label_t> idx_label_map;
        /**
         * @brief mapa znacznik -> indeks
         */
        std::map<label_t, Vertex::idx_t> label_idx_map;

        /**
         * @brief zwróć znacznik dla wierzchołka o indeksie idx
         *
         * @param idx indeks wierzchołka
         *
         * @return znacznik wierzchołka idx
         */
        label_t getLabel(Vertex::idx_t idx) const;
        /**
         * @brief zwróć indeks dla wierzchołka o znaczniku label
         *
         * @param label znacznik wierzchołka
         *
         * @return indeks wierzchołka label
         */
        Vertex::idx_t getIndex(label_t label) const;

        /**
         * @brief zwraca referencję na wierzchołek o podanym znaczniku
         *
         * @param label znacznik wierzchołka
         *
         * @return referencja na wierzchołek o znaczniku label
         */
        const Vertex &getVertexAt(label_t label) const;

        /**
         * @brief znajdź kolejny wolny indeks dla następnego wierzchołka
         */
        void findNextFreeIdx();


    public:
        /**
         * @brief stwórz pusty graf
         */
        Graph();

        /* GETTERS */
        /**
         * @brief zwróć liczbę wierzchołków w grafie
         *
         * @return liczba wierzchołków w grafie
         */
        unsigned int getVertexCount() const;

        /**
         * @brief zwróć liczbę krawędzi w grafie
         *
         * @return liczba krawędzi w grafie
         */
        unsigned int getEdgeCount() const;

        /**
         * \copydoc Graph::getVertexCount
         */
        unsigned int getSize() const;

        /**
         * @brief zwróć 'stopień' (invariant) wierzchołka
         *
         * @param label znacznik wierzchołka
         *
         * @return 'stopień' wierzchołka label
         */
        unsigned int getInvariant(label_t label) const;

        /**
         * @brief zwróć wejściowość wierzchołka
         *
         * @param label znacznik wierzchołka
         *
         * @return wejściowość wierzchołka label
         */
        unsigned int getIn(label_t label) const;

        /**
         * @brief zwróć wejściowość wierzchołka
         *
         * wejściowość wierzchołka w podgrafie indukowanym zbiorem
         * wierzchołków
         *
         * @param label znacznik wierzchołka
         * @param vset podzbiór wierzchołków grafu
         *
         * @return wejściowość wierzchołka label w grafie indkowanym zbiorem vset
         */
        unsigned int getIn(label_t label, const vertex_set_t &vset) const;

        /**
         * @brief zwróć wyjściowość wierzchołka
         *
         * @param label znacznik wierzchołka
         *
         * @return wyjściowość wierzchołka label
         */
        unsigned int getOut(label_t label) const;

        /**
         * @brief zwróć wyjściowość wierzchołka
         *
         * wyjściowość wierzchołka w podgrafie indukowanym zbiorem
         * wierzchołków
         *
         * @param label znacznik wierzchołka
         * @param vset podzbiór wierzchołków grafu
         *
         * @return wyjściowość wierzchołka label w grafie indkowanym zbiorem vset
         */
        unsigned int getOut(label_t label, const vertex_set_t &vset) const;


        /**
         * @brief zapełnij zbiór krawędziami grafu
         *
         * @param edges referencja na zbiór krawędzi
         *
         * @return liczba umieszczonych krawędzi
         */
        unsigned int getEdges(edge_set_t &edges) const;

        /**
         * \copybrief Graph::getEdges
         *
         * krawędzie grafu incydentne z wierzchołkiem label
         *
         * @param edges referencja na zbiór krawędzi
         * @param label znacznik wierzchołka z incydentnego
         *
         * @return liczba umieszczonych krawędzi
         */
        unsigned int getEdges(edge_set_t &edges, label_t label) const;

        /**
         * \copybrief Graph::getEdges
         *
         * krawędzie grafu indukowanego wierzchołkami z verts
         *
         * @param edges referencja na zbiór krawędzi
         * @param verts referencja na zbiór znaczników wierzchołków
         *
         * @return liczba umieszczonych krawędzi
         */
        unsigned int getEdges(edge_set_t &edges, const vertex_set_t &verts) const;

        /**
         * @brief zapisz ścieżkę po przejściu grafu wgłąb (DFS)
         *
         * @param path referencja na ścieżkę
         * @param start znacznik wierzchołka startowego
         * @param visited zbiór wierzchołków które należy uznać za wcześniej odwiedzone
         */
        void getDFSPath(dfs_path &path, label_t start, const dfs_visited &visited = dfs_visited()) const;

        /**
         * @brief informacje na temat grafu
         *
         * Metoda pomocnicza.
         *
         * @return string z informacjami o grafie
         */
        std::string getInfo() const;

        /* SETTERS */
        /**
         * @brief dodaj wierzchołek do grafu
         *
         * @param label znacznik dodawanego wierzchołka
         *
         * @return czy rzeczywiście dodano wierzchołek (jeżeli
         * znacznik jest już w użyciu to nie dodano)
         */
        bool addVertex(label_t label);

        /**
         * @brief ustaw wierzchołek
         *
         * ustawia listę sąsiedztwa wierzchołka. Jeżeli w grafie
         * nie istnieją jeszcze rozpatrywane wierzchołki to są one
         * dodawane.
         *
         * @param label znacznik wierzchołka
         * @param adj lista znaczników wierzchołków (lista sąsiedztwa)
         */
        void setVertex(label_t label, std::initializer_list<label_t> adj);

        /**
         * \copydoc Graph::setVertex
         */
        void setVertex(label_t label, std::vector<label_t> adj);

        /**
         * @brief dodaje krawędź między dwoma wierzchołkami
         *
         * @param v znacznik wierzchołka źródłowego
         * @param w znacznik wierzchołka docelowego
         *
         * @return czy dodano krawędź (nie dodano gdy już ustnieje lub
         * nie w grafie nie ma któregoś z wierzchołków)
         */
        bool addEdge(label_t v, label_t w); // Dodaje krawedz pomiedzy wierzcholkami

        /**
         * @brief usuwa wszystkie krawędzie i wierzchołki z grafu
         */
        void clear();


        /* CHECKERS */
        /**
         * @brief sprawdź czy graf nie zawiera wierzchołków
         *
         * @return czy graf jest pusty
         */
        bool isEmpty() const;

        /**
         * @brief czy wierzchołek o danym znaczniku należy
         * do grafu
         *
         * @param label znacznik wierzchołka
         *
         * @return czy graf zawiera wierzchołek label
         */
        bool isNode(label_t label) const;

        /**
         * @brief czy w grafie występuje połączenie między
         * wierzchołkami
         *
         * @param v znacznik wierzchołka źródłowego
         * @param w znacznik wierzchołka docelowego
         *
         * @return czy wstępuje połączenie
         */
        bool isConnection(label_t v, label_t w)  const;// Zwraca true jesli istnieje bezposrednie polaczenie miedzy wierzcholkami

        /**
         * @brief czy w grafie indukowanym występuje połączenie między
         * wierzchołkami
         *
         * @param v znacznik wierzchołka źródłowego
         * @param w znacznik wierzchołka docelowego
         * @param vset zbiór wierzchołków rozpinających
         *
         * @return czy wstępuje połączenie
         */
        bool isConnection(label_t v, label_t w, const vertex_set_t &vset) const;

        /* ITERATE */
        /**
         * @brief iterator wierzchołków grafu
         *
         * @return iterator wskazujący na początek znaczników
         * wierzchołków grafu
         */
        inline iterator begin() const {return labels.begin();};

        /**
         * @brief iterator wierzchołków grafu
         *
         * @return iterator wskazujący na koniec znaczników
         * wierzchołków grafu
         */
        inline iterator end() const {return labels.end();};

        /**
         * @brief iterator wierzchołków incydentnych
         *
         * @param label znacznik wierzchołka źródłowego
         *
         * @return iterator wskazujący na początek znaczników
         * wierzchołków incydentnych z wierzchołkiem label
         */
        AdjIter adjBegin(label_t label) const;


        /**
         * @brief iterator wierzchołków incydentnych
         *
         * @param label znacznik wierzchołka źródłowego
         *
         * @return iterator wskazujący na koniec znaczników
         * wierzchołków incydentnych z wierzchołkiem label
         */
        AdjIter adjEnd(label_t label) const;

        /* INPUT/OUTPUT */
        /**
         * @brief zapisz wierzchołek do postaci stringa
         *
         * @param v referencja na wierzchołek
         *
         * @return string z zapisanym wierzchołkiem
         */
        std::string dumpVertex(const Vertex &v) const;

        /**
         * @brief wczyta wierzchołek ze stringa
         *
         * @param str string z wierzchołkiem
         */
        void loadVertex(std::string &str);

        /**
         * @brief zapisz graf do pliku
         *
         * @param filename nazwa pliku
         */
        void saveToFile(const std::string &filename);

        /**
         * @brief wczytaj graf z pliku
         *
         * @param filename nazwa pliku
         */
        void loadFromFile(const std::string &filename);

        /* GENERATORS */
        /**
         * @brief wygeneruj losowy, skierowany graf spójny
         *
         * @param vertex_count liczba wierzchołków w grafie losowym
         * @param density gęstość grafu
         *
         * @return losowy, skierowany graf spójny o vertex_count wierzchołkach
         * i gęstości density krawędzi
         */
        static Graph generateRandom(unsigned int vertex_count, double density);

        /**
         * @brief twórz losowy graf izomorficzny
         *
         * @param other graf który będzie izomorficzny z
         * nowym grafem
         *
         * @return referencja na graf izomorficzny do grafu other
         */
        Graph &randomIsomorphic(const Graph &other);

        /* FRIENDS */
        friend std::ostream& operator<<(std::ostream &strm, const Graph &g);
        friend std::istream& operator>>(std::istream &strm, Graph &g);
};

std::ostream& operator<<(std::ostream &strm, const Graph &g);
std::istream& operator>>(std::istream &strm, Graph &g);

#endif /* end of include guard: GRAPH_HPP */
