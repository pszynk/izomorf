/**
 *      @file  isomorphismAlgo.hpp
 *     @brief  plik nagłówkowy klasy IsomorphismAlgo
 *
 * Detailed description starts here.
 *
 * Deklaracja klasy IsomorphismAlgo i klas wewnętrznych klasy IsomorphismAlgo
 *
 *=====================================================================================
 */

#ifndef ISOMORPHISMALGO_HPP
#define ISOMORPHISMALGO_HPP

#include <map>
#include <set>
#include <sstream>
#include <iostream>

#include "graph.hpp"

/**
 * @brief klasa reprezentuje algorytm do weryfikacji izomorfizmu grafów
 *
 * Klasa zawiera struktury danych i funkcje pomocnicze realizujące rozszerzoną
 * wersję algorytmu powrotów.
 *
 * Udostępnia także funkcję weryfikującą izomorfizm dwóch grafów.
 */
class IsomorphismAlgo
{
    public:
        /**
         * @brief mapa do reprezentacji izomorfizmu dwóch grafów
         *
         * mapa znacznik (grafu X) -> znacznik (grafu Y)
         */
        typedef std::map<Graph::label_t, Graph::label_t> iso_map;

        /**
         * @brief konstruktor biorący referencje na dwa grafy
         *
         * @param _graphX graf X
         * @param _graphY graf Y
         */
        IsomorphismAlgo(const Graph &_graphX, const Graph &_graphY);

        /**
         * @brief funkcja weryfikująca izomorfizm grafów podanych w
         * konstruktorze
         *
         * * W pierwszym kroku sprawdzane jest czy grafy spełniają wymagania
         * podstawowe \ref IsomorphismAlgo::meetsRequirements.
         * * Jeżeli grafy spełniają te wymagania, przygotowywane są struktury danych.
         * * Następnie wykonywany jest właściwy algorytm (metoda powrotów).
         * * Jeżeli grafy są izomorficzne, po wykonaniu się funkcji obiekt klasy
         * zawiera przekształcenie izomorficzne z grafu X na graf Y.
         *
         *
         * @return czy grafy \ref IsomorphismAlgo::graphX i IsomorphismAlgo::graphY są izomorficzne.
         */
        bool isIsomorphism();

        /**
         * @brief sprawdź warunki podstawowe izomorfizmu grafów podanych w
         * konstruktorze.
         *
         * Sprawdzane warunki to:
         * * Taka sama liczba wierzchołków.
         * * Taka sama liczba krawędzi.
         * * Taka sama liczba wierzchołków o jednakowych 'stopniach' (invariant)
         *
         * @return czy grafy \ref IsomorphismAlgo::graphX i IsomorphismAlgo::graphY
         * mają szansę być izomorficzne.
         */
        bool meetsRequirements();

        /**
         * @brief zwraca referencję na przekształcenie izomorficzne, otrzymane
         * przy weryfikacji izomorfizmu.
         *
         * @return referencja na mapę znaczników grafu X na graf Y
         */
        const iso_map &getIsoMap() const {return this->f_map;};

        /**
         * @brief Weryfikuje przekształcenie izomorficzne dwóch grafów.
         *
         * Weryfikuje, czy przekształcenie f: X -> Y jest izomorfizmem grafów.
         *
         * @param _graphX referencja na graf X
         * @param _graphY referencja na graf Y
         * @param f referencja na przekształcenie
         *
         * @return czy przekształcenie jest izomorfizmem
         */
        static bool verifyIsomorphism(const Graph &_graphX, const Graph &_graphY, const iso_map &f);

        /**
         * @brief informacje pomocnicze na temat obiektu klasy \ref IsomorphismAlgo
         *
         * @return string z informacjami pomocniczymi
         */
        std::string getInfo() const;

    private:
        /**
         * @brief indeks wierzchołka DFS lasu
         */
        typedef int dfs_idx_t;

        /**
         * @brief iterator po posortowanych krawędziach
         */
        typedef std::vector<Graph::Edge>::const_iterator edge_iter;

        /**
         * @brief mapowanie \ref Graph::label_t -> \ref IsomorphismAlgo::dfs_idx_t
         */
        typedef std::map<Graph::label_t, dfs_idx_t> dfs_num_t;

        /**
         * @brief mapowanie \ref IsomorphismAlgo::dfs_idx_t -> \ref Graph::label_t
         */
        typedef std::vector<Graph::label_t> dfs_vec_t;

        /**
         * @brief komparator do sortowania krawędzi w DFS lesie
         *
         * Krawędzie z grafu \ref IsomorphismAlgo::graphX są sortowane leksykograficznie
         * względem indeksów wierzchołków \ref IsomorphismAlgo::dfs_idx_t DFS lasu.
         */
        class EdgeComparator {
            public:
                /**
                 * @brief konstruktor pobiera referencje na mapowanie typu
                 * \ref IsomorphismAlgo::dfs_num_t
                 *
                 * @param _dfs_num mapowanie typu \ref IsomorphismAlgo::dfs_num_t
                 */
                EdgeComparator(const dfs_num_t& _dfs_num):dfs_num(_dfs_num)
            {};

                /**
                 * @brief sortowanie leksykograficzne krawędzi
                 *
                 * Sortowanie krawędzi DFS lasu wymagane w algorytmie powrotów.
                 *
                 * @param left pierwsza krawędź
                 * @param right druga krawędź
                 *
                 * @return czy pierwsza krawędź jest mniejsza od drugiej
                 */
                bool operator() (const Graph::Edge &left, const Graph::Edge &right);

            private:
                /**
                 * @brief mapa \ref IsomorphismAlgo::dfs_num_t
                 */
                const dfs_num_t &dfs_num;
        };
    private:
        /**
         * @brief Graf X na podstawie którego budowany będzie DFS las
         */
        const Graph &graphX;

        /**
         * @brief Graf Y w którym szukane będą wierzchołki izomorficzne
         */
        const Graph &graphY;

        /**
         * @brief przekształcenie izomorficzne wierzchołków grafu \ref IsomorphismAlgo::graphX na
         * \ref IsomorphismAlgo::graphY
         */
        iso_map f_map;


        /**
         * @brief mapowanie \ref Graph::label_t -> \ref IsomorphismAlgo::dfs_idx_t
         * dla wierzchołków grafu \ref IsomorphismAlgo::graphX
         */
        dfs_num_t dfs_num;

        /**
         * @brief mapowanie \ref IsomorphismAlgo::dfs_idx_t -> \ref Graph::label_t
         * dla wierzchołków grafu \ref IsomorphismAlgo::graphX
         */
        dfs_vec_t dfs_vec;

        /**
         * @brief wierzchołki grafu \ref IsomorphismAlgo::graphY już dopasowane
         */
        std::set<Graph::label_t> in_S;

        /**
         * @brief posortowane krawędzie DFS lasu, odpowiadające krawędziom grafu
         * \ref IsomorphismAlgo::graphX
         */
        std::vector<Graph::Edge> ordered_edges;

        /**
         * @brief struktura pomocnicza, do wyznaczenia liczby
         * wierzchołków o takim samym 'stopniu' (invariant) w grafie
         * \ref IsomorphismAlgo::graphX
         */
        std::map<unsigned int, unsigned int> invX_buckets;

        /**
         * @brief struktura pomocnicza, do wyznaczenia liczby
         * wierzchołków o takim samym 'stopniu' (invariant) w grafie
         * \ref IsomorphismAlgo::graphY
         */
        std::map<unsigned int, unsigned int> invY_buckets;

        /**
         * @brief licznik wykorzystywany w metodzie IsomorphismAlgo::match
         *
         * Licznik wskazuje ile jest krawędzi incydentnych z rozpatrywanym
         * wierzchołkiem k grafu G_X[k].
         */
        int edges_count_k;

        /**
         * @brief główna procedura sprawdzająca izomorfizm grafów
         *
         * Metoda rekurencyjna, która próbuje przyporządkować k-temu
         * wierzchołkowi DFS lasu, wierzchołek z grafu G_Y, tak żeby
         * G_X[k] ~= G_Y[S].
         *
         * Procedura przechodzi przez wierzchołki DFS lesu. Porusza się po odpowiednio
         * posortowanych krawędziach grafu \ref IsomorphismAlgo::graphX. Gdy uda się przejść
         * wszystkie krawędzie grafu \ref IsomorphismAlgo::graphX, procedura kończy się sukcesem.
         *
         * @param iter iterator po posortowanych krawędziach grafu \ref IsomorphismAlgo::graphX, czyli po
         * \ref IsomorphismAlgo::dfs_num.
         * @param dfs_num_k indeks rozpatrywanego wierzchołka DFS lasu.
         *
         * @return czy krawędź wskazywana przez \p iter może zostać przyporządkowana
         * jakieś niewykorzystanej krawędzi grafu \ref IsomorphismAlgo::graphY.
         */
        bool match(edge_iter iter, int dfs_num_k);

        /**
         * @brief czyści struktury danych
         */
        void resetData();

        /**
         * @brief wyznacza liczność wierzchołków o tych samych 'stopniach'
         * (invariant) w obu grafach.
         *
         * Generuje struktury danych \ref IsomorphismAlgo::invX_buckets i
         * \ref IsomorphismAlgo::invY_buckets.
         */
        void countInvBuckets();

        /**
         * @brief numeruje wierzchołki grafu \ref IsomorphismAlgo::graphX zgodnie
         * z czasem przechodzenia DFS
         *
         * Generuje struktury danych \ref IsomorphismAlgo::dfs_num i
         * \ref IsomorphismAlgo::dfs_vec.
         */
        void numberVertexes();

        /**
         * @brief pobiera i sortuje krawędzie grafu \ref IsomorphismAlgo::graphX
         *
         * Do sortowania wykorzystywany jest komaprator \ref IsomorphismAlgo::EdgeComparator
         *
         * Generuje strukturę danych \ref IsomorphismAlgo::ordered_edges
         */
        void orderEdges();
};


#endif /* end of include guard: ISOMORPHISMALGO_HPP */
