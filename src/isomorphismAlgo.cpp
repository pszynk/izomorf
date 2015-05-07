/**
 *      @file  isomorphismAlgo.cpp
 *     @brief  implementacja metod klasy IsomorphismAlgo
 *
 * Detailed description starts here.
 *
 *=====================================================================================
 */

#include "isomorphismAlgo.hpp"

IsomorphismAlgo::IsomorphismAlgo(
        const Graph &_graphX,
        const Graph &_graphY):
    graphX(_graphX), graphY(_graphY), edges_count_k(0)
{

}
bool IsomorphismAlgo::isIsomorphism()
{
    if (this->graphX.isEmpty() && this->graphY.isEmpty()) {
        return true;
    }

    if (!this->meetsRequirements()) {
        return false;
    }

    this->numberVertexes();
    this->orderEdges();

    int dfs_num_k = -1;
    return this->match(ordered_edges.begin(), dfs_num_k);
}


bool IsomorphismAlgo::meetsRequirements()
{
    if (this->graphX.isEmpty() && this->graphY.isEmpty()) {
        return true;
    }

    if (this->graphX.getSize() != this->graphY.getSize()) {
        return false;
    }

    if (this->graphX.getEdgeCount() != this->graphY.getEdgeCount()) {
        return false;
    }

    this->resetData();

    this->countInvBuckets();

    if (this->invX_buckets != this->invY_buckets) {
        return false;
    }

    return true;
}

bool IsomorphismAlgo::match(edge_iter iter, int dfs_num_k)
{
    using std::max;

    if (iter != this->ordered_edges.end()) {
        // zostały jeszcze krawędzie do przyporządkowania

        // rozpatrujemy wierzchołki krawędzi (graf G_X)
        Graph::label_t i = iter->source;
        Graph::label_t j = iter->target;

        if (dfs_num[i] > dfs_num_k) {
            // Szukanie 'MATCH' dla korzenia lasu DFS o nr. k+1
            //
            // nie przyporządkowaliśmy jeszcze wierzchołka na którym
            // obecnie stoimy. Ma to miejsce dla korzeni DFS lasu

            Graph::label_t next = dfs_vec[dfs_num_k + 1];
            // sprawdzamy jaki znacznik w grafie ma korzeń k+1

            for(Graph::iterator it = this->graphY.begin();
                    it != this->graphY.end(); ++it) {
                // dla wszystkich wierzchołków w grafie G_Y
                if (this->graphX.getInvariant(next) ==
                        this->graphY.getInvariant(*it) &&
                        (!this->in_S.count(*it))) {
                    // które mają taki sam 'stopień' (invariant)
                    // i nie są jeszcze przypisane do żadnego z k
                    // wierzchołków drzewa
                    this->f_map[next] = (*it);
                    this->in_S.insert(*it);
                    // próbujemy przypisać korzeń k+1
                    this->edges_count_k = 0;
                    // do nowego korzenia nie dochodzą i nie wychodzą żadne
                    // wcześniej napotkane krawędzie

                    if (this->match(iter, dfs_num_k + 1)) {
                        // próbujemy dopasować kolejny wierzchołek
                        return true;
                    }
                    // nie udało się go dopasować do tego wierzchołka z G_Y
                    this->in_S.erase(*it);
                }
            }
        }
        else if (dfs_num[j] > dfs_num_k) {
            // Sprawdz czy G_X[k] ~= G_Y[S] i znajdz 'MATCH' dla j
            //
            // Obecna krawędź wychodzi z dopasowanego wierzchołka, ale
            // dochodzi do jeszcze niedopasowanego. Oznacza to, że jest
            // to ostatnia krawędź incydentna z tym wierzchołkiem jaką będziemy
            // rozpatrywać.
            Graph::label_t k = dfs_vec[dfs_num_k];
            // sprawdzamy jaki znacznik w G_X ma k-ty dopasowany wierzchołek
            this->edges_count_k -= this->graphY.getOut(f_map.at(k), in_S);
            this->edges_count_k -= this->graphY.getIn(f_map.at(k), in_S);
            // sprawdzamy czy grafie G_Y[S] f(k) jest incydentny z taką samą
            // liczbą krawędzi co wierzchołek k w grafie G[k]

            if (this->edges_count_k != 0) {
                // jak nie jest to k i f(k) zostały źle dopasowane
                return false;
            }

            Graph::label_t fi = this->f_map.at(i);
            // będziemy próbowali przyporządkować wierzchołek z grafu G_Y[V-S]
            // do wierzchołka j z G_X. Wierzchołek j jest incydentny z i, a więc
            // f(j) musi być incydentny z f(i). Dlatego przeszukujemy listę sąsiedztwa
            // wierzchołka f(i).
            for(Graph::AdjIter it = this->graphY.adjBegin(fi);
                    it != this->graphY.adjEnd(fi); ++it) {
                if (this->graphX.getInvariant(j) ==
                        this->graphY.getInvariant(*it) &&
                        (!this->in_S.count(*it))) {
                    // odrzucamy wierzchołki o złym 'stopniu' (invariant)
                    // i już przyporządkowane
                    this->f_map[j] = (*it);
                    this->in_S.insert(*it);
                    this->edges_count_k = 1;
                    // liczba krawędzi łączących nowo przyporządkowany wierzchołek
                    // z G[k] to 1, (właśnie nią dotarliśmy do wierzchołka)

                    int next_k = max(dfs_num_k, max(dfs_num[i], dfs_num[j]));
                    // jeżeli numerowanie drzewa jest poprawne to next_k = dfs_num_K + 1
                    // graf musi być spójny

                    if (this->match(iter + 1, next_k)) {
                        // rozpatruj następną krawędź dla grafu G_X[k+1]
                        return true;
                    }
                    this->in_S.erase(*it);
                    // jednak złe przyporządkowanie
                }
            }
        }
        else {
            // sprawdź czy (f(i), f(j)) in E_Y[S] i idź dalej
            //
            // Oba wierzchołki mają już przyporządkowanie, trzeba sprawdzić
            // czy nowo napotkana krawędź występuje też w G[S]
            Graph::label_t fi = this->f_map.at(i);
            Graph::label_t fj = this->f_map.at(j);
            if(this->graphY.isConnection(fi, fj, this->in_S)) {
                ++this->edges_count_k;
                // jeśli występuje to zwiększamy licznik
            }
            if (this->match(iter + 1, dfs_num_k)) {
                // rozpatrujemy kolejną krawędź, próbując dopasować k+1 wierzchołek
                return true;
            }
        }
    } else {
        return true;
        // przeszliśmy wszystkie krawędzie, znaleziono dopasowania
    }
    return false;
}


void IsomorphismAlgo::resetData()
{
    this->f_map.clear();
    this->dfs_num.clear();
    this->dfs_vec.clear();
    this->in_S.clear();
    this->ordered_edges.clear();
    this->invX_buckets.clear();
    this->invY_buckets.clear();
    this->edges_count_k = 0;

}

void IsomorphismAlgo::countInvBuckets()
{
    using std::map;

    for(Graph::iterator it = this->graphX.begin();
            it != this->graphX.end(); ++it) {
        ++this->invX_buckets[graphX.getInvariant(*it)];
    }

    for(map<unsigned, unsigned>::const_iterator it = this->invX_buckets.begin();
            it != invX_buckets.end(); ++it) {
    }

    for(Graph::iterator it = this->graphY.begin();
            it != this->graphY.end(); ++it) {
        ++this->invY_buckets[graphY.getInvariant(*it)];
    }
}


void IsomorphismAlgo::numberVertexes()
{

    using std::multimap;
    using std::make_pair;
    typedef multimap<unsigned int, Graph::label_t> XinvLabelMap;

    XinvLabelMap xinvLabel;
    // multimapa liczba wierzchołków o takim samym 'stopniu' (invariant) jak ja
    //   -->
    // mój znacznik

    for(Graph::iterator it = this->graphX.begin();
            it != this->graphX.end(); ++it) {
        unsigned int inv = graphX.getInvariant(*it);
        xinvLabel.insert(
                make_pair(this->invX_buckets[inv], (*it)));
    }


    Graph::dfs_visited visited;

    dfs_idx_t idx = 0;
    this->dfs_vec.resize(xinvLabel.size());

    for(XinvLabelMap::const_iterator it = xinvLabel.begin();
            it != xinvLabel.end(); ++it) {
        Graph::dfs_path path;
        this->graphX.getDFSPath(path, it->second, visited);

        for(Graph::dfs_path::const_iterator dfsIt = path.begin();
                dfsIt != path.end(); ++dfsIt) {
            this->dfs_vec[idx] = (*dfsIt);
            this->dfs_num[(*dfsIt)] = (idx++);
        }
        visited.insert(path.begin(), path.end());

        if (visited.size() == this->graphX.getSize()) {
            break;
        }
    }
}

bool IsomorphismAlgo::EdgeComparator::operator() (
        const Graph::Edge &left, const Graph::Edge &right)
{
    using std::min;
    using std::max;
    using std::make_pair;

    typedef IsomorphismAlgo::dfs_idx_t dfs_idx_t;

    dfs_idx_t ls = this->dfs_num.at(left.source);
    dfs_idx_t lt = this->dfs_num.at(left.target);

    dfs_idx_t rs = this->dfs_num.at(right.source);
    dfs_idx_t rt = this->dfs_num.at(right.target);

    dfs_idx_t maxl = max(ls, lt);
    dfs_idx_t maxr = max(rs, rt);

    return make_pair(maxl, make_pair(ls, lt)) <
        make_pair(maxr, make_pair(rs, rt));
}

void IsomorphismAlgo::orderEdges()
{
    using std::sort;

    Graph::edge_set_t edges;

    this->graphX.getEdges(edges);

    this->ordered_edges.insert(this->ordered_edges.begin(),
            edges.begin(), edges.end());

    EdgeComparator comp(this->dfs_num);

    sort(this->ordered_edges.begin(), this->ordered_edges.end(),
            comp);
}


std::string IsomorphismAlgo::getInfo() const
{
    using std::endl;
    using std::stringstream;
    typedef std::vector<Graph::Edge> EdgeVec;

    stringstream ss;
    for(dfs_num_t::const_iterator it = this->dfs_num.begin();
            it != this->dfs_num.end(); ++it) {
        ss << it->first << " :: " << it->second << endl;
    }

    for(EdgeVec::const_iterator it = this->ordered_edges.begin();
            it != this->ordered_edges.end(); ++it) {
        ss << it->source << " -> " << it->target << " | " <<
            this->dfs_num.at(it->source) << " -> " <<
            this->dfs_num.at(it->target) << endl;
    }

    ss << endl;

    for(iso_map::const_iterator it = this->f_map.begin();
            it != this->f_map.end(); ++it) {
        ss << it->first << " -->> " << it->second << endl;
    }

    ss << endl;

    return ss.str();
}
bool IsomorphismAlgo::verifyIsomorphism(const Graph &graphX, const Graph &graphY, const iso_map &f)
{
    Graph::edge_set_t edgesX;
    Graph::edge_set_t edgesY;

    graphX.getEdges(edgesX);
    graphY.getEdges(edgesY);

    for(Graph::edge_set_t::const_iterator itx = edgesX.begin();
            itx != edgesX.end(); ++itx) {
        bool matched = false;
        for(Graph::edge_set_t::const_iterator ity = edgesY.begin();
                ity != edgesY.end(); ++ity) {
            if ((ity->source) == f.at(itx->source) &&
                    (ity->target) == f.at(itx->target)) {
                matched = true;
            }
        }
        if (!matched) {
            return false;
        }
    }
    return true;
}
