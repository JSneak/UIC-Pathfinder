/*util.h*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>

#include "graph.h"

using namespace std;

const double INF = numeric_limits<double>::max();

map<long long, long long> Dijkstra(graph<long long, double>& G, long long startV, map<long long, double>& distances);