#include <ctime>
#include <vector>
#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct route{
  int dest, cost;
};

struct node {
  vector<route> neighbours;
};

vector<node> readPlaces(){
  ifstream text("agraph");
  int numNodes; text >> numNodes;
  vector<node> nodes(numNodes);
  int node, neighbour, cost;
  while (text >> node >> neighbour >> cost){
    nodes[node].neighbours.push_back(route{neighbour, cost});
  }
  for (auto &n : nodes) {
    sort(n.neighbours.begin(), n.neighbours.end(),
        [](const route &a, const route &b) {
          return a.cost > b.cost;
        });
  }
  return nodes;
}

static int getMaxCost(const vector<node> &nodes,
                      const vector<char> &visited) {
  int result = 0;
  int lowestMaxNodeCost = 0;
  for (int i = 0; i < (int) nodes.size(); ++i) {
    if (visited[i]) {
      continue;
    }
    int maxNodeCost = 0;
    for (const route &neighbour : nodes[i].neighbours) {
      if (visited[neighbour.dest]) {
        continue;
      }
      if (neighbour.cost > maxNodeCost) {
        maxNodeCost = neighbour.cost;
      }
    }
    if (maxNodeCost < lowestMaxNodeCost) {
      lowestMaxNodeCost = maxNodeCost;
    }
    result += maxNodeCost;
  }
  return result - lowestMaxNodeCost;
}

static void getLongestPath(int remainingNodes,
                           int nodeID,
                           int curPathLen,
                           const vector<node> &nodes,
                           vector<char> &visited,
                           int &maxPathLen) {
  if (!remainingNodes) {
    if (curPathLen > maxPathLen) {
      maxPathLen = curPathLen;
    }
    return;
  }
  visited[nodeID] = true;
  int nextMaxCost = getMaxCost(nodes, visited);
  for (const route &neighbour : nodes[nodeID].neighbours) {
    if (!visited[neighbour.dest]) {
      int nextPathLen = curPathLen + neighbour.cost;
      if (nextPathLen + nextMaxCost > maxPathLen) {
        getLongestPath(remainingNodes - 1, neighbour.dest, nextPathLen,
                       nodes, visited, maxPathLen);
      }
    }
  }
  visited[nodeID] = false;
}

static int getLongestPath(const vector<node> &nodes)
{
  vector<char> visited(nodes.size());
  int maxPathLen = 0;
  getLongestPath(nodes.size() - 1, 0, 0, nodes, visited, maxPathLen);
  return maxPathLen;
}

int main(int argc, char** argv){
  auto nodes = readPlaces();
  auto start = high_resolution_clock::now();
  int len = getLongestPath(nodes);
  auto end = high_resolution_clock::now();
  auto duration = (int)(0.001 * duration_cast<microseconds>(end - start).count());
  cout << len << " LANGUAGE C++ " << duration << std::endl;
}
