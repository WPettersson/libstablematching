#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <memory>
#include <vector>


// Forward declaration for friendships
class DiGraph;

class DiEdge {
  public:
    DiEdge(int start, int end);
    int start() const { return _start; }
    int end() const { return _end; }

  private:
    int _start;
    int _end;
};

class DiVert {
  public:
    DiVert(int index, int weight);
    int inDeg() const { return _inEdges.size(); }
    int outDeg() const { return _outEdges.size(); }

  private:
    void addOut(std::shared_ptr<DiEdge> edge);
    void addIn(std::shared_ptr<DiEdge> edge);
    void removeOut(int end);
    void removeIn(int start);
    std::map<int, std::shared_ptr<DiEdge>> _outEdges;
    std::map<int, std::shared_ptr<DiEdge>> _inEdges;

    int _index;
    int _weight;

  friend DiGraph;
};

class DiGraph {
  public:
    DiGraph();
    void addVertex(int vertex, int weight);
    void addEdge(int start, int end);

    /**
     * Remove an edge. Leaves both end vertices in place.
     */
    void removeEdge(int start, int end);

    /**
     * Remove a vertex, and also any incident edges.
     */
    void removeVertex(int index);

    /**
     * Get the poset width-measure that I made up. Note that this DESTROYS the
     * graph to calculate it.
     */
    int getWidth();

  private:
    std::map<int,DiVert> _vertices;

    void reduce();

};

#endif /* GRAPH_H */
