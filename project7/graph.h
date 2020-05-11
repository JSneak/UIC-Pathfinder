/*graph.h*/

//
// Sean Kim
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// original author: Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <set>
#include <map>
#include <vector>

using namespace std;


template<typename VertexT, typename WeightT>
class graph
{
private:

  map<VertexT, map<VertexT, WeightT>> adjacencyList;

  /* Clears the Vertex Map and the Edge Map */
  void _clearMap() {
    for (auto x : this->adjacencyList) {
      (x.second).clear();
    }
    this->adjacencyList.clear();
  }

  /* Inserts the Vertexs first and then the Edges */
  void _copyHelper(graph& other) {
    
    for (auto x : other.adjacencyList) {
      addVertex(x.first);
    }

    for (auto x : other.adjacencyList) {
      for(auto y: x.second) {
        addEdge(x.first, y.first, y.second);
      }
    }

  }

public:
  //
  // constructor:
  //
  graph() {}

  /* Deep Copy Constructor */
  graph(graph& other) {

    /* First need to clear the map */
    _clearMap();

    /* Copy over the vertexs first */
    _copyHelper(other);

  }

  graph& operator=(graph& other) {
    /* First need to clear the map */
    _clearMap();

    // delete adjacencyList;

    /* Copy over the vertexs first */
    _copyHelper(other);

  }

  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const
  {
    return this->adjacencyList.size();
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const
  {
    int count = 0;

    for (auto x : this->adjacencyList) {
      for(auto y: x.second) {
        count++;
      }
    }

    return count;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if already doesn't exist. If it does exist, return false
  //
  bool addVertex(VertexT v)
  {
    /* Check if vertex already exists in the map */
    if(this->adjacencyList.count(v) > 0)
      return false;

    //
    // if we get here, vertex does not exist so insert.
    //
    map<VertexT, WeightT> Edge;
    this->adjacencyList.emplace(v, Edge);

    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist, return false.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight)
  {
    /* Check to see if the verticies exist */
    if(this->adjacencyList.count(from) <= 0)
      return false;
    if(this->adjacencyList.count(to) <= 0)
      return false;

    // map<VertexT, WeightT> Edge;
    // Edge.emplace(to, weight);

    /* If edge already exists, erase and then insert again */
    if((this->adjacencyList.at(from)).count(to) > 0)
      (this->adjacencyList.at(from)).erase(to);
      

    (this->adjacencyList.at(from)).emplace(to, weight);

    return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If 
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not 
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const
  {
    /* Check to see if the verticies exist */
    if(this->adjacencyList.count(from) <= 0)
      return false;

    if(this->adjacencyList.count(to) <= 0)
      return false;

    //
    // the vertices exist, but does the edge exist?
    //    
    if ((this->adjacencyList.at(from)).find(to) == (this->adjacencyList.at(from)).end())  // no:
      return false;

    weight = (this->adjacencyList.at(from)).at(to);

    return true;
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const
  {
    set<VertexT>  S;

    /* Check if vertex exists map */
    if(this->adjacencyList.count(v) <= 0)
      return S;

    //
    // Loop through the adjancency list populating the set
    //
    
    for(auto y: this->adjacencyList.at(v)) {
      S.insert(y.first);
    }

    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const
  {
    vector<VertexT> V;
    for (auto x : this->adjacencyList) {
      V.emplace_back(x.first);
    }
    return V;
  }

  //
  // dump
  // 
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G(26);
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const
  {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    output << "**Vertices:" << endl;

    int count = 1;
    
    for (auto x : this->adjacencyList) {
      output << " " << count << "." << x.first << endl;
      count++;
    }

    output << endl;
    output << "**Edges:" << endl;

    for (auto x : this->adjacencyList) {
      output << " row " << x.first << ": ";
      for(auto y: x.second) {
        output << "(" << x.first << ", " << y.first << ", " << y.second << "), ";
      }
      output << endl;
    }

    output << "**************************************************" << endl;
  }

};