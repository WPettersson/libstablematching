#include "graph.h"

#include <utility>

DiEdge::DiEdge(int start, int end) : _start(start), _end(end) { }

DiVert::DiVert(int index, int weight) : _index(index), _weight(weight) { }

void DiVert::addOut(std::shared_ptr<DiEdge> edge) {
  int target = edge->end();
  _outEdges[target] = edge;
}

void DiVert::addIn(std::shared_ptr<DiEdge> edge) {
  int source = edge->start();
  _inEdges[source] = edge;
}

void DiVert::removeOut(int end) {
  _outEdges.erase(end);
}

void DiVert::removeIn(int start) {
  _inEdges.erase(start);
}

DiGraph::DiGraph() {}

void DiGraph::addVertex(int vertex, int weight) {
  _vertices.emplace(std::piecewise_construct,
      std::forward_as_tuple(vertex),
      std::forward_as_tuple(vertex, weight));
}

void DiGraph::addEdge(int start, int end) {
  std::shared_ptr<DiEdge> edge(new DiEdge(start, end));
  _vertices.at(start).addOut(edge);
  _vertices.at(end).addIn(edge);
}

void DiGraph::removeEdge(int start, int end) {
  _vertices.at(start).removeOut(end);
  _vertices.at(end).removeIn(start);
}

int DiGraph::getWidth() {
  int width = 0;
  return width;
}
