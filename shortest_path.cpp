#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream> 

using namespace std;

class Edge;

class Node {
public:
    int id;
    vector<Edge*> edges;

    Node(int id) : id(id) {}
};

class Edge {
public:
    Node* node1;
    Node* node2;
    int weight;

    Edge(Node* n1, Node* n2, int w) : node1(n1), node2(n2), weight(w) {}
};

void calculate_shortest_path(vector<Node*>& nodes, Node* start, Node* target, vector<Node*>& path) {
    vector<int> dist(nodes.size(), numeric_limits<int>::max());
    vector<Node*> parent(nodes.size(), nullptr);
    vector<bool> visited(nodes.size(), false);
    dist[start->id] = 0;

    priority_queue<pair<int, Node*>, vector<pair<int, Node*>>, greater<pair<int, Node*>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        Node* current = pq.top().second;
        pq.pop();

        if (visited[current->id]) continue;
        visited[current->id] = true;

        for (Edge* edge : current->edges) {
            Node* neighbor = (edge->node1 == current) ? edge->node2 : edge->node1;

            if (visited[neighbor->id]) continue;

            int new_dist = dist[current->id] + edge->weight;
            if (new_dist < dist[neighbor->id]) {
                dist[neighbor->id] = new_dist;
                parent[neighbor->id] = current;
                pq.push({new_dist, neighbor});
            }
        }
    }

    Node* current = target;
    while (current != nullptr) {
        path.push_back(current);
        current = parent[current->id];
    }

    reverse(path.begin(), path.end());
}

Node* find_node_by_id(vector<Node*>& nodes, int id) {
    for (Node* node : nodes) {
        if (node->id == id) {
            return node;
        }
    }
    return nullptr;
}

int main() {
    Node* node1 = new Node(0);
    Node* node2 = new Node(1);
    Node* node3 = new Node(2);
    Node* node4 = new Node(3);

    vector<Node*> nodes = {node1, node2, node3, node4};

    Edge* edge1 = new Edge(node1, node2, 10);
    Edge* edge2 = new Edge(node1, node3, 5);
    Edge* edge3 = new Edge(node2, node4, 1);
    Edge* edge4 = new Edge(node3, node2, 3);
    Edge* edge5 = new Edge(node3, node4, 2);

    vector<Edge*> edges = {edge1, edge2, edge3, edge4, edge5};

    node1->edges = {edge1, edge2};
    node2->edges = {edge1, edge3};
    node3->edges = {edge2, edge4, edge5};
    node4->edges = {edge3, edge5};

    vector<Node*> path;
    calculate_shortest_path(nodes, node1, node4, path);  

    ofstream output("output.txt"); 

    if (output.is_open()) {
        output << "Shortest path from node " << node1->id << " to node " << node4->id << ":\n";
        for (Node* node : path) {
            output << node->id << " ";
        }
        output << endl;
        output.close(); 
    } else {
        cerr << "Unable to open output.txt for writing." << endl;
    }
    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete edge1;
    delete edge2;
    delete edge3;
    delete edge4;
    delete edge5;

    return 0;
}
