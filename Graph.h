#ifndef GRAPH_H
#define GRAPH_H

#include "LocatorHeap.h"

#include <vector>
using std::vector;

class Edge;

class Vertex
{
public:
    int label;
    vector<Edge *> edges;

    // helpers stored inside a vertex to help with path finding
    // you can use different auxilliary structures if desired
    bool visited;
    float distanceTo;
    vector<Vertex *> pathTo;
    Heap<Vertex *>::Locator locator;
    bool operator<(const Vertex &v) const
    {
        // TODO for locator heap implementation
        return distanceTo < v.distanceTo;
    }

    Vertex(int l) : label(l) {}
};

class Edge
{
public:
    Vertex *v1, *v2;
    float weight;

    Edge(Vertex *nv1, Vertex *nv2, float newWeight) : v1(nv1), v2(nv2), weight(newWeight) {}
};

class Graph
{
protected:
    vector<Vertex *> v;
    vector<Edge *> e;

public:
    Graph()
    {
        v = vector<Vertex *>();
        e = vector<Edge *>();
    }

    ~Graph()
    {
        // TODO
        for (int i = 0; i < v.size(); i++)
        {
            delete v.at(i);
        }
        for (int i = 0; i < e.size(); i++)
        {
            delete e.at(i);
        }
    }

    // TODO: complete rule of 3
    // copy constructor
    Graph(const Graph &g)
    {

        for (size_t i = 0; i < v.size(); i++)
        {
            v.push_back(new Vertex(g.v.at(i)->label));
            /* code */
        }
        for (size_t i = 0; i < e.size(); i++)
        {
            Edge *newEdge = new Edge(g.e.at(i)->v1, g.e.at(i)->v2, g.e.at(i)->weight);
            e.push_back(newEdge);
            v.at(g.e.at(i)->v1->label)->edges.push_back(newEdge);
            v.at(g.e.at(i)->v2->label)->edges.push_back(newEdge);
            /* code */
        }
    }

    // assignment operator
    Graph &operator=(const Graph &g)
    {
        if (this != &g)
        {
            for (size_t i = 0; i < v.size(); i++)
            {
                delete v.at(i);
                /* code */
            }
            for (size_t i = 0; i < e.size(); i++)
            {
                delete e.at(i);
                /* code */
            }
            v.clear();
            e.clear();
            for (size_t i = 0; i < g.v.size(); i++)
            {
                v.push_back(new Vertex(g.v.at(i)->label));
                /* code */
            }
            for (size_t i = 0; i < g.e.size(); i++)
            {
                Edge *newEdge = new Edge(g.e.at(i)->v1, g.e.at(i)->v2, g.e.at(i)->weight);
                e.push_back(newEdge);
                v.at(g.e.at(i)->v1->label)->edges.push_back(newEdge);
                v.at(g.e.at(i)->v2->label)->edges.push_back(newEdge);
                /* code */
            }
        }
        return *this;
    }

    void insertVertex(int label)
    {
        // TODO
        v.push_back(new Vertex(label));
    }

    void insertEdge(int l1, int l2, float weight)
    {
        // TODO
        // check if l1 and l2 are in v
        Edge *newEdge = new Edge(v.at(l1), v.at(l2), weight);
        // create new edge
        e.push_back(newEdge);
        v.at(l1)->edges.push_back(newEdge);
        v.at(l2)->edges.push_back(newEdge);
    }

    vector<Vertex *> shortestPath(int start, int end)
    {
        // TODO: Dijkstra's algorithm
        setUnvisited();
        Heap<Vertex *> h;
        for_each(v.begin(), v.end(), [&](Vertex *v)
                 { v->locator = h.insertElement(v); });

        // set the distance of the start vertex to 0
        // cout << "empty: " << h.isEmpty() << endl;
        // cout << "size: " << h.size() << endl;
        // cout << "start: " << start << endl;
        v.at(start)->distanceTo = 0;
        h.update(v.at(start)->locator);
        // cout << v.at(start)->label << " distance: " << v.at(start)->distanceTo << endl;

        while (!h.isEmpty())
        {
            // get the vertex with the smallest distance
            Vertex *current = h.removeMin();
            // mark it as visited
            current->visited = true;
            // cout << "current: " << current->label << endl;
            // cout << "current distance: " << current->distanceTo << endl;
            // cout << "current distance: " << current->distanceTo << endl;
            // for each of its edges
            for (int i = 0; i < current->edges.size(); i++)
            {
                // get the other vertex
                Vertex *other = current->edges[i]->v1 == current ? current->edges[i]->v2 : current->edges[i]->v1;

                // if the other vertex is not visited
                if (!other->visited)
                {
                    // if the distance to the other vertex is greater than the distance to the current vertex + the weight of the edge
                    if (other->distanceTo > current->distanceTo + current->edges[i]->weight)
                    {
                        // set the distance to the other vertex to the distance to the current vertex + the weight of the edge
                        other->distanceTo = current->distanceTo + current->edges[i]->weight;
                        // set the path to the other vertex to the path to the current vertex + the current vertex
                        other->pathTo = current->pathTo;
                        other->pathTo.push_back(current);
                        // cout << other->label << " distance: " << other->distanceTo << endl;
                        h.update(other->locator);
                        // update the locator
                    }
                }
                // cout << "other: " << other->label << endl;
                // cout << "other distance: " << other->distanceTo << endl;
                // cout << "other visited: " << other->visited << endl;
            }

            /* code */
        }
        v.at(end)->pathTo.push_back(v.at(end));
        return v.at(end)->pathTo;
    }

private:
    // TODO: method to set all the vextors to unvisited
    void setUnvisited()
    {
        for (int i = 0; i < v.size(); i++)
        {
            v[i]->visited = false;
            v[i]->pathTo = vector<Vertex *>();
            v[i]->distanceTo = numeric_limits<float>::max();
        }
    }

    // TODO: add all vectores in the proirty queue
    void addAllToQueue(Heap<Vertex *> &h)
    {
        for (int i = 0; i < v.size(); i++)
        {
            // h.insertElement(v[i]);
        }
    }

    int getVectorIndex(int label)
    {
        int i = 0;
        for (i = 0; i < v.size(); i++)
        {
            if (v[i]->label == label)
            {
                return i;
            }
        }
        insertVertex(label);
        return i + 1;
    }
};

#endif
