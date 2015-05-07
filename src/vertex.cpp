/**
 *      @file  vertex.cpp
 *     @brief  implementacja metod klasy Vertex
 *
 * Detailed description starts here.
 *
 *=====================================================================================
 */


#include "vertex.hpp"

typedef Vertex::idx_t idx_t;

/* PUBLIC */

Vertex::Vertex(idx_t _index):index(_index),
    degree(std::make_pair(0, 0))
{}

idx_t Vertex::getIndex() const
{
    return this->index;
}

bool Vertex::addAdjacent(Vertex &adj)
{
    using std::set;
    if (this->adjacent.insert(adj.getIndex()).second) {
        this->degree.first++;
        adj.addNeighbour();
        return true;
    }
    return false;
}
bool Vertex::isAdjacent(idx_t v) const
{
    return (adjacent.end() != adjacent.find(v));
}

Vertex::iterator Vertex::begin() const
{
    return this->adjacent.begin();
}

Vertex::iterator Vertex::end() const
{
    return this->adjacent.end();
}

unsigned int Vertex::getIn() const
{
    return this->degree.first;
}

unsigned int Vertex::getOut() const
{
    return this->degree.second;
}

std::string Vertex::getInfo() const
{
    using std::string;
    using std::stringstream;

    stringstream ss;
    ss << "[" << this->getIndex() << "]";
    ss << "{" << this->getIn() << "/" << this->getOut() << "}";
    ss << "|";
    for(iterator it = this->begin(); it != this->end(); ++it) {
        ss << (*it) << ", ";
    }
    ss << "|";
    return ss.str();
}


/* PRIVATE */

void Vertex::addNeighbour()
{
    this->degree.second++;
}
