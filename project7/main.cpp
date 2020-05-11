/*main.cpp*/

//
// Sean Kim
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #07: open street maps, graphs, and Dijkstra's alg
// 
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:  
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <limits>
#include <stack>

#include "graph.h"
#include "tinyxml2.h"
#include "dist.h"
#include "osm.h"
#include "util.h"

using namespace std;
using namespace tinyxml2;

/* Checks to see if a building exists */
bool DoesBuildingExist(string building, vector<BuildingInfo> Buildings, int& pos) {
	for(unsigned int i = 0; i < Buildings.size(); i++){
		if(Buildings[i].Abbrev == building) {
			pos = i;
			return true;
		}

		size_t found = (Buildings[i].Fullname).find(building);
		if(found!=std::string::npos) {
			pos = i;
			return true;
		}
	}
	return false;
}

/* Gets nearest footway to building */
long long GetNearestFootway(map<long long, Coordinates> Nodes, vector<FootwayInfo> Footways, double lat, double lon) {
	double lowest = 999999.0;
	long long ID = 0;
	for(unsigned int i = 0; i < Footways.size(); i++){
		for(unsigned int x = 0; x < (Footways[i].Nodes).size(); x++) {
			double distance = distBetween2Points(lat, lon, (Nodes.at(Footways[i].Nodes[x])).Lat, (Nodes.at(Footways[i].Nodes[x])).Lon);
			if(lowest > distance) {
				lowest = distance;
				ID = Footways[i].Nodes[x];
			}
		}
	}
	return ID;
}



//////////////////////////////////////////////////////////////////
//
// main
//
int main()
{
  map<long long, Coordinates>  Nodes;     // maps a Node ID to it's coordinates (lat, lon)
  vector<FootwayInfo>          Footways;  // info about each footway, in no particular order
  vector<BuildingInfo>         Buildings; // info about each building, in no particular order
  XMLDocument                  xmldoc;
	
  graph<long long, double> G; // vertices are nodes, weights are distances
  
  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string def_filename = "map.osm";
  string filename;

  cout << "Enter map filename> ";
  getline(cin, filename);

  if (filename == "")
  {
    filename = def_filename;
  }

  //
  // Load XML-based map file 
  //
  if (!LoadOpenStreetMap(filename, xmldoc))
  {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }
  
  //
  // Read the nodes, which are the various known positions on the map:
  //
  unsigned nodeCount = ReadMapNodes(xmldoc, Nodes);

  //
  // Read the footways, which are the walking paths:
  //
  unsigned footwayCount = ReadFootways(xmldoc, Footways);

  //
  // Read the university buildings:
  //
  unsigned buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

  //
  // Stats
  //
  assert(nodeCount == Nodes.size());
  assert(footwayCount == Footways.size());
  assert(buildingCount == Buildings.size());

  cout << endl;
  cout << "# of nodes: " << Nodes.size() << endl;
  cout << "# of footways: " << Footways.size() << endl;
  cout << "# of buildings: " << Buildings.size() << endl;


  //
  // TODO: build the graph, output stats:
  //

	/* Inserting Nodes as Vertices */
  for(const auto &node: Nodes) {
		G.addVertex(node.first);
	}
	
	for(unsigned int i = 0; i < Footways.size(); i++) {
		for(unsigned int x = 0; x < Footways[i].Nodes.size() - 1; x++) {
			double distance = distBetween2Points(Nodes[Footways[i].Nodes[x]].Lat, Nodes[Footways[i].Nodes[x]].Lon, Nodes[Footways[i].Nodes[x+1]].Lat, Nodes[Footways[i].Nodes[x+1]].Lon);
			G.addEdge(Footways[i].Nodes[x], Footways[i].Nodes[x+1], distance);
			G.addEdge(Footways[i].Nodes[x+1], Footways[i].Nodes[x], distance);
		}
	}

  cout << "# of vertices: " << G.NumVertices() << endl;
  cout << "# of edges: " << G.NumEdges() << endl;
  cout << endl;

  //
  // Navigation from building to building
  //
  string startBuilding, destBuilding;

  cout << "Enter start (partial name or abbreviation), or #> ";
  getline(cin, startBuilding);

  while (startBuilding != "#")
  {
    cout << "Enter destination (partial name or abbreviation)> ";
    getline(cin, destBuilding);

		int posStart;
		int posDest;
		
		if(startBuilding == "BH"){
        startBuilding = "Burnham Hall";
    }
		
		/* Check to see if startBuilding exists */
		if(DoesBuildingExist(startBuilding, Buildings, posStart)) {
			/* Check to see if destBuilding exists */
			if(DoesBuildingExist(destBuilding, Buildings, posDest)) {
				
				cout << "Starting point:" << endl;
				cout << " " << Buildings[posStart].Fullname << endl;
				cout << " (" << Buildings[posStart].Coords.Lat << ", " << Buildings[posStart].Coords.Lon << ")" << endl;
				
				cout << "Destination point:" << endl;
				cout << " " << Buildings[posDest].Fullname << endl;
				cout << " (" << Buildings[posDest].Coords.Lat << ", " << Buildings[posDest].Coords.Lon << ")" << endl;
				cout << endl;
				
				/* Find nearest start and dest footways */
				long long StartID = GetNearestFootway(Nodes, Footways, Buildings[posStart].Coords.Lat, Buildings[posStart].Coords.Lon);
				long long DestID = GetNearestFootway(Nodes, Footways, Buildings[posDest].Coords.Lat, Buildings[posDest].Coords.Lon);
				
				cout << "Nearest start node:" << endl;
				cout << " " << StartID << endl;
				cout << " (" << (Nodes.at(StartID)).Lat << ", " << (Nodes.at(StartID)).Lon << ")" << endl;
				
				cout << "Nearest destination node:" << endl;
				cout << " " << DestID << endl;
				cout << " (" << (Nodes.at(DestID)).Lat << ", " << (Nodes.at(DestID)).Lon << ")" << endl;
				cout << endl;
				
				/* Run Dijkstra's Alg */
				map<long long, long long> shortestPath;
		    map<long long, double> distances;

		    shortestPath = Dijkstra(G, StartID, distances);
				cout << "Navigating with Dijkstra..." << endl;
				if(distances[DestID] == INF) {
					cout << "Sorry, destination unreachable" << endl;
				} else {
					cout << "Distance to dest: " << distances[DestID] << " miles" << endl;
					
					/* Create new Stack because we're able to print it out backwards right now */
					stack<long long> pathStack;
					
					/* x is the predecessor to DestID */
					long long x = shortestPath[DestID];
					/* Push DestID onto the stack so it'll print last */
					pathStack.push(DestID);
					/* Loop through the map and push the predecessors into the stack */
					while(shortestPath[x] != 0) {
						pathStack.push(x);
						x = shortestPath[x];
					}
					/* Push the StartID onto the top of the stack */
					pathStack.push(StartID);
					
					/* Print out the top of the stack */
					cout << "Path: "  << pathStack.top();
					pathStack.pop();
					
					/* Loop through the rest of the stack */
					while (!pathStack.empty()) {
					cout << "->";
					cout << pathStack.top();
					pathStack.pop();
					}
					cout << endl;
				}

			} else {
				/* Building Does Not Exist */
				cout << "Destination building not found" << endl;
			}
		} else {
			/* Building Does Not Exist */
			cout << "Start building not found" << endl;
		}
    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);
  }

  //
  // done:
  //
  cout << "** Done **" << endl;

  return 0;
}
