/*util.cpp*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <queue> 
#include <utility>
#include <set>

#include "util.h"


using namespace std;

class prioritize
{
public:
  bool operator()(const pair<long long, double>& point1, const pair<long long, double>& point2) const{
      
      if(point1.second > point2.second){ 
          return true; 
      }
      else if(point1.second < point2.second ){ 
          return false; 
      }
      else{ 
          return point1.first > point2.first; 
      }
  }
};

//
// Dijkstra:
//
// Performs Dijkstra's shortest weighted path algorithm from
// the given start vertex.  Returns a vector of vertices in
// the order they were visited, along with a map of (string,int)
// pairs where the string is a vertex V and the int is the 
// distance from the start vertex to V; if no such path exists,
// the distance is INF (defined in util.h).
//
map<long long, long long> Dijkstra(graph<long long, double>& G, long long startV, map<long long, double>& distances) {
  set<long long> visitedSet;
	/* Step 0: Create a Priority Queue */
  priority_queue<pair<long long, double>, vector<pair<long long, double>>, prioritize> unvisitedQueue;
  double altPath; 
	
	/* Step 0a: Create Predecessors Map */
	map<long long, long long> prev;
	
	/* Step 0b: Set startV to "" for Predecessors Map */
	prev[startV] = 0;
  
	/* Step 1: Fill up the distances with the number of vertices */
	for(auto curV: G.getVertices()) {
		distances[curV] = INF; 
		unvisitedQueue.push(make_pair(curV, INF));
	} 
	
	/* Step 2: Add Source to Priority Queue, where distance is 0 */
  distances[startV] = 0; 
  unvisitedQueue.push(make_pair(startV, 0));
  
	/* Step 3: While Priority Queue isn't empty */
	while(!unvisitedQueue.empty()) {
		
		/* Step 4: Get min distance vertex (curV) from Priority Queue */
		pair<long long, double> curV; 
		curV = unvisitedQueue.top(); 
		unvisitedQueue.pop();

		if(distances[curV.first] == INF) {
			break;
		} else if(visitedSet.count(curV.first)!=0) {
			continue;
		}

		visitedSet.insert(curV.first);
		
		/* Step 5: Visit all of curV Neighbors */
		for(auto adjV: G.neighbors(curV.first)) {
			double edgeWeight = 0;
			/* Step 5a: Get the weight of the Edge between curV and adjV */
			G.getWeight(curV.first, adjV, edgeWeight);
			altPath = distances[curV.first] + edgeWeight;
			/* Step 5b: If distance to adjV is shorter by going through curV */
			if(altPath < distances[adjV]) {
				/* Step 5c: Update the distance of adjV */
				distances[adjV] = altPath;
				/* Step 5d: Insert adjV into Priority Queue */
				unvisitedQueue.push(make_pair(adjV, altPath));
				/* Step 5e: Insert X into Predecessors Map */
				prev[adjV] = curV.first;
			}
		}
	}
	return prev;
}
