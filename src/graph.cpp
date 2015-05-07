/**
 *      @file  graph.cpp
 *     @brief  implementacja metod klasy Graph
 *
 * Coś didatkowegoh
 *
 *=====================================================================================
 */

#include "graph.hpp"

typedef Vertex::idx_t idx_t;
typedef Graph::label_t label_t;

/* PRIVATE */

label_t Graph::getLabel(idx_t idx) const
{
    return this->idx_label_map.at(idx);
}

idx_t Graph::getIndex(label_t label) const
{
    return this->label_idx_map.at(label);
}

void Graph::findNextFreeIdx()
{
    using std::pair;
    using std::map;
    typedef map<idx_t, Vertex> vertexMap;

    vertexMap::const_iterator it;
    it = this->vertexes.find(this->__first_free_idx);

    if (it == this->vertexes.end()) {
        return;
    }

    idx_t next = this->__first_free_idx;
    for (; it != this->vertexes.end() && next == it->first; ++it, ++next) {}

    this->__first_free_idx = next;
}

/* PUBLIC */

/* CONSTRUCTOR */
Graph::Graph(): vertex_count(0), edge_count(0), __first_free_idx(0)
{}

/* GETTERS */
unsigned int Graph::getVertexCount() const
{
    return this->vertex_count;
}

unsigned int Graph::getEdgeCount() const
{
    return this->edge_count;
}


unsigned int Graph::getSize() const
{
    return this->getVertexCount();
}

unsigned int Graph::getInvariant(label_t label) const
{
    unsigned int out = this->vertexes.at(this->getIndex(label)).getOut();
    unsigned int in = this->vertexes.at(this->getIndex(label)).getIn();

    return this->__inv_power * out + in;

}

const Vertex &Graph::getVertexAt(label_t label) const
{
    return this->vertexes.at(this->getIndex(label));
}

unsigned int Graph::getIn(label_t label) const
{
    return this->getVertexAt(label).getIn();
}

unsigned int Graph::getIn(label_t label, const vertex_set_t &vset) const
{
    unsigned int counter = 0;

    for(vertex_set_t::const_iterator it = vset.begin(); it != vset.end(); ++it) {
        if(this->isConnection((*it), label)) {
            ++counter;
        }
    }
    return counter;
}

unsigned int Graph::getOut(label_t label) const
{
    return this->getVertexAt(label).getOut();
}

unsigned int Graph::getOut(label_t label, const vertex_set_t &vset) const
{
    const Vertex &ver = this->getVertexAt(label);

    unsigned int counter = 0;
    for(Vertex::iterator it = ver.begin(); it != ver.end(); ++it) {
        if (vset.count(this->idx_label_map.at((*it)))) {
            ++counter;
        }
    }
    return counter;
}


unsigned int Graph::getEdges(edge_set_t &edges) const
{
    return this->getEdges(edges, this->labels);
}
unsigned int Graph::getEdges(edge_set_t &edges, label_t label) const
{
    const Vertex &ver = this->getVertexAt(label);
    for(Vertex::iterator it = ver.begin();
            it != ver.end(); ++it) {
        edges.insert(Edge(label, this->getLabel(*it)));
    }
    return ver.getOut();

}
unsigned int Graph::getEdges(edge_set_t &edges, const vertex_set_t &verts) const
{
    unsigned int rtrn = 0;
    for(vertex_set_t::iterator it = verts.begin();
            it != verts.end(); ++it) {
        rtrn += this->getEdges(edges, *it);
    }
    return rtrn;
}

void Graph::getDFSPath(
        dfs_path &path, label_t start,
        const dfs_visited &visited) const
{
    using std::stack;

    stack<label_t> s;
    dfs_visited v;

    path.clear();

    if(visited.count(start)) {
        return;
    }

    s.push(start);
    v.insert(start);

    while(!s.empty()) {
        label_t parent = s.top(); s.pop();
        path.push_back(parent);
        const Vertex &ver = this->getVertexAt(parent);

        for(Vertex::iterator it = ver.begin();
                it != ver.end(); ++it) {
            label_t child = this->getLabel((*it));

            if(!visited.count(child) && !v.count(child)) {
                s.push(child);
                v.insert(child);
            }
        }
    }
}


std::string Graph::getInfo() const
{

    using std::string;
    using std::endl;
    using std::stringstream;

    stringstream ss;

    ss << "Vertexes:" << this->getVertexCount() << endl;
    ss << "Edges   :" << this->getEdgeCount() << endl;
    ss << string(80, '-') << endl;

    typedef std::map<Vertex::idx_t, Vertex>::const_iterator itVer;
    for(itVer it = this->vertexes.begin(); it != this->vertexes.end(); ++ it) {
        ss << "(" << this->getLabel(it->first) << ") -> ";
        ss << it->second.getInfo() << endl;
    }
    ss << string(80, '-') << endl;
    return ss.str();
}



/* CHECKERS */
bool Graph::isEmpty() const
{
    return (this->getSize() == 0);
}

bool Graph::isNode(label_t label) const
{
    return (this->label_idx_map.find(label) !=
            this->label_idx_map.end());
}

bool Graph::isConnection(label_t v, label_t w) const
{
    if (!this->isNode(v)) {
        return false;
    } else if (!this->isNode(w)) {
        return false;
    }

    return this->getVertexAt(v).isAdjacent(this->getIndex(w));
}

bool Graph::isConnection(label_t v, label_t w, const vertex_set_t &vset) const
{
    if (!this->isNode(v)) {
        return false;
    } else if (!this->isNode(w)) {
        return false;
    } else if (!vset.count(v)) {
        return false;
    } else if (!vset.count(w)) {
        return false;
    }
    return this->getVertexAt(v).isAdjacent(this->getIndex(w));
}




/* SETTERS */
bool Graph::addVertex(label_t label)
{

    using std::pair;
    using std::map;
    typedef pair<label_t, idx_t> label2idx;
    typedef pair<idx_t, Vertex> idx2Vertex;

    if (this->label_idx_map.insert(
                label2idx(label, this->__first_free_idx)).second)
    {
        this->labels.insert(label);
        ++this->vertex_count;
        this->__inv_power = pow(10, ceil(log10(this->vertex_count + 1)));
        this->idx_label_map[this->__first_free_idx] = label;
        this->vertexes.insert(
                idx2Vertex(this->__first_free_idx, Vertex(this->__first_free_idx)));

        this->findNextFreeIdx();
    }

    return false;

}

void Graph::setVertex(label_t label, std::initializer_list<label_t> adj)
{
    std::vector<label_t> vec(adj.begin(), adj.end());
    this->setVertex(label, vec);
}

void Graph::setVertex(label_t label, std::vector<label_t> adj)
{
    typedef std::vector<label_t> labVec;
    std::vector<label_t> vec;

    this->addVertex(label);
    for(labVec::iterator it = adj.begin(); it != adj.end(); ++it) {
        this->addVertex((*it));
        this->addEdge(label, (*it));
    }
}

bool Graph::addEdge(label_t v, label_t w)
{
    if (
            (v != w)        &&
            this->isNode(v) &&
            this->isNode(w) &&
            !this->isConnection(v, w)
       ) {
        ++this->edge_count;
        this->vertexes.at(this->getIndex(v)).
            addAdjacent(this->vertexes.at(this->getIndex(w)));
        return true;
    }
    return false;
}

void Graph::clear()
{
    this->vertex_count = 0;
    this->edge_count = 0;
    this->__first_free_idx = 0;

    this->vertexes.clear();
    this->idx_label_map.clear();
    this->label_idx_map.clear();
}


/* ITERATORS */
Graph::AdjIter Graph::adjBegin(label_t label) const
{
    return AdjIter(this->idx_label_map, this->getVertexAt(label).begin());
}

Graph::AdjIter Graph::adjEnd(label_t label) const
{
    return AdjIter(this->idx_label_map, this->getVertexAt(label).end());
}




/* INPUT/OUTPUT */
std::string Graph::dumpVertex(const Vertex &v) const
{
    using std::stringstream;
    using std::set;

    set<label_t> labels;
    for(Vertex::iterator itl = v.begin();
            itl != v.end(); ++itl) {
        labels.insert(this->idx_label_map.at((*itl)));
    }

    stringstream ss;
    ss << this->idx_label_map.at(v.getIndex()) << ": ";
    set<label_t>::iterator it = labels.begin();
    if (it != labels.end()) {
        ss << (*it);
        ++it;
    }
    for(; it != labels.end(); ++it) {
        ss  << ", " << (*it);
    }
    return ss.str();
}

void Graph::loadVertex(std::string &str)
{
    using std::string;
    using std::vector;
    using std::stringstream;
    using std::stoi;
    using std::invalid_argument;
    using std::out_of_range;

    size_t colIdx = str.find(":");
    if (colIdx == string::npos) {
        throw invalid_argument("Brak znaku `:'");
        return;
    }

    label_t label;
    try {
        label = stoi(str.substr(0, colIdx));
    } catch (invalid_argument &e) {
        throw invalid_argument("Zła etykieta wierzchołka głównego");
    } catch (out_of_range &e) {
        throw std::invalid_argument("Etykieta wierzchołka głównego przekracza wymiar");
    }

    string adjStr;
    vector<label_t> labVec;
    try {
        adjStr = str.substr(colIdx+1, str.length());
    } catch (out_of_range &e) {
        labVec.clear();
    }

    vector<string> labstrVec = split(strip_space(adjStr), DELIMITER_CHAR);


    try {
        for (vector<string>::iterator it = labstrVec.begin();
                it != labstrVec.end(); ++it) {
            labVec.push_back(stoi((*it)));
        }
    } catch (invalid_argument &e) {
        stringstream ss;
        ss << "Krawędź " << labVec.size() + 1
            << ": zła etykieta wierzchołka docelowego";
        throw invalid_argument(ss.str());
    } catch (out_of_range &e) {
        stringstream ss;
        ss << "Krawędź " << labVec.size() + 1
            << ": etykieta wierzchołka docelowego przekracza wymiar";
        throw std::invalid_argument(ss.str());
    }

    this->setVertex(label, labVec);
}

void Graph::saveToFile(const std::string &filename)
{
    using std::ofstream;
    using std::runtime_error;

    ofstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Nie udało się otworzyć pliku do zapisu");
    }
    file << (*this);
    file.close();
}

void Graph::loadFromFile(const std::string &filename)
{
    using std::ifstream;
    using std::runtime_error;

    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Nie udało się otworzyć pliku do odczytu");
    }

    this->clear();

    file >> (*this);
    file.close();
}



/* GENERATORS */
Graph Graph::generateRandom(
        unsigned int vertex_count, double density)
{
    using std::invalid_argument;

    typedef std::chrono::high_resolution_clock hclock;
    typedef std::default_random_engine Generator;
    typedef std::uniform_int_distribution<> Distribution;

    typedef std::set<label_t> Set;
    typedef std::map<label_t, label_t> Map;

    if (vertex_count < 1) {
        throw invalid_argument("Liczba wierzchołków musi być większa od 1");
    }

    if (density <= 0 || density > 1) {
        throw invalid_argument("Gęstość musi należeć do przedziału (0, 1]");
    }

    label_t minLabel = 1;
    label_t maxLabel = vertex_count;

    Generator gen(hclock::now().time_since_epoch().count());
    Distribution vertexDist(minLabel, maxLabel);
    Distribution coin(0, 1);

    Set inTree;
    Map successor;

    label_t root = vertexDist(gen);
    inTree.insert(root);

    Graph graph;
    graph.addVertex(root);
    for (label_t i = minLabel; i <= maxLabel; ++i) {
        label_t ver = i;
        graph.addVertex(ver);
        while (!inTree.count(ver)) {
            label_t next = vertexDist(gen);
            Map::iterator it = successor.find(ver);
            if (it != successor.end()) {
                it->second = next;
            } else {
                successor[ver] = next;
            }
            ver = next;
        }
        ver = i;
        while (!inTree.count(ver)) {
            inTree.insert(ver);
            ver = successor[ver];
        }
    }


    for(Map::iterator it = successor.begin();
            it != successor.end(); ++it) {
        if (coin(gen)) {
            graph.addEdge(it->first, it->second);
        } else {
            graph.addEdge(it->second, it->first);
        }
    }

    long long counter = 0;
    unsigned long edge_count = vertex_count * (vertex_count - 1) * density + 0.5;

    while(graph.getEdgeCount() < edge_count) {
        if ((counter++) > MAX_RANDOM_FAILS) {
            break;
        }

        if (graph.addEdge(vertexDist(gen), vertexDist(gen))) {
            counter = 0;
        }
    }

    return graph;
}

Graph &Graph::randomIsomorphic(const Graph &other)
{
    using std::vector;
    typedef std::chrono::high_resolution_clock hclock;
    typedef std::default_random_engine Generator;

    typedef std::map<idx_t, label_t> Map;

    if (this != &other) {
        *this = other;
    }

    vector<label_t> labels;
    for(Map::iterator it = this->idx_label_map.begin();
            it != this->idx_label_map.end(); ++it) {
        labels.push_back(it->second);
    }

    unsigned seed = hclock::now().time_since_epoch().count();

    std::shuffle(labels.begin(), labels.end(), Generator(seed));

    this->label_idx_map.clear();


    size_t i = 0;
    for(Map::iterator it = this->idx_label_map.begin();
            it != this->idx_label_map.end(); ++it) {
        it->second = labels[i];
        this->label_idx_map[labels[i++]] = it->first;
    }
    return *this;
}





/* STREAMS */
std::ostream& operator<<(std::ostream &strm, const Graph &g)
{
    using std::map;
    using std::endl;

    typedef map<label_t, idx_t> labelMap;

    for(labelMap::const_iterator it = g.label_idx_map.begin();
            it != g.label_idx_map.end(); ++it) {
        strm << g.dumpVertex(g.vertexes.at(it->second));
        strm << endl;
    }
    return strm;
}

std::istream& operator>>(std::istream &strm, Graph &g)
{
    using std::string;
    using std::stringstream;
    using std::getline;
    using std::invalid_argument;

    string line;

    unsigned counter = 1;
    while(getline(strm, line)) {
        string pure_line = line.substr(0, line.find(COM_SIGN));
        if (pure_line == "") continue;
        try {
            g.loadVertex(pure_line);
        } catch (invalid_argument &iae) {
            stringstream ss;
            ss << "Linia [" << counter << "]> " << iae.what();
            throw invalid_argument(ss.str());
        }
        ++counter;
    }
    return strm;
}
