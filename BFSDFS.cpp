#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    // 🔹 Parallel BFS
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS Traversal:\n";

        while (!q.empty()) {
            int size = q.size();

            #pragma omp parallel for
            for (int i = 0; i < size; i++) {
                int node;

                #pragma omp critical
                {
                    node = q.front();
                    q.pop();
                    cout << node << " ";
                }

                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }

    // 🔹 DFS Helper
    void dfsUtil(int v, vector<bool> &visited) {
        visited[v] = true;

        #pragma omp critical
        cout << v << " ";

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); i++) {
            int neighbor = adj[v][i];

            if (!visited[neighbor]) {
                #pragma omp task
                dfsUtil(neighbor, visited);
            }
        }
    }

    // 🔹 Parallel DFS
    void parallelDFS(int start) {
        vector<bool> visited(V, false);

        cout << "DFS Traversal:\n";

        #pragma omp parallel
        {
            #pragma omp single
            {
                dfsUtil(start, visited);
            }
        }
        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "Enter number of edges: ";
    cin >> E;

    Graph g(V);

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;
    cout << "Enter starting vertex: ";
    cin >> start;

    int choice;
    cout << "\nChoose Traversal:\n";
    cout << "1. BFS\n2. DFS\n3. Both\n";
    cin >> choice;

    if (choice == 1) {
        g.parallelBFS(start);
    } else if (choice == 2) {
        g.parallelDFS(start);
    } else if (choice == 3) {
        g.parallelBFS(start);
        g.parallelDFS(start);
    } else {
        cout << "Invalid choice\n";
    }

    return 0;
}