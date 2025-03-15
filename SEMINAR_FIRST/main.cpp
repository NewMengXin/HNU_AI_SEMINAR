#include<iostream>
#include<vector>
#include<memory.h>
#include<stack>
#include<algorithm>
#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define L 9
#define M 10
#define N 11
#define O 12
#define P 13
#define R 14
#define S 15
#define T 16
#define U 17
#define V 18
#define Z 19

using namespace std;

int h[20] =
{ 366,0,160,242,161,
178,77,151,226,244,
241,234,380,98,193,
253,329,80,199,374 };

struct node
{
    int g;
    int h;
    int f;
    int name;
    node(int name, int g, int h)
    {
        this->name = name;
        this->g = g;
        this->h = h;
        this->f = g + h;
    };
    bool operator <(const node &a)const
    {
        return f < a.f;
    }
};

class Graph
{
public:
    Graph()
    {
        memset(graph, -1, sizeof(graph));
    }
    int getEdge(int from, int to)
    {
        return graph[from][to];
    }
    void addEdge(int from, int to, int cost)
    {
        if (from >= 20 || from < 0 || to >= 20 || to < 0)
            return;
        graph[from][to] = cost;
    }
    
    void init(){
        addEdge(O, Z, 71);
        addEdge(Z, O, 71);

        addEdge(O, S, 151);
        addEdge(S, O, 151);

        addEdge(Z, A, 75);
        addEdge(A, Z, 75);

        addEdge(A, S, 140);
        addEdge(S, A, 140);

        addEdge(A, T, 118);
        addEdge(T, A, 118);

        addEdge(T, L, 111);
        addEdge(L, T, 111);

        addEdge(L, M, 70);
        addEdge(M, L, 70);

        addEdge(M, D, 75);
        addEdge(D, M, 75);

        addEdge(D, C, 120);
        addEdge(C, D, 120);

        addEdge(C, R, 146);
        addEdge(R, C, 146);

        addEdge(S, R, 80);
        addEdge(R, S, 80);

        addEdge(S, F, 99);
        addEdge(F, S, 99);

        addEdge(F, B, 211);
        addEdge(B, F, 211);

        addEdge(P, C, 138);
        addEdge(C, P, 138);

        addEdge(R, P, 97);
        addEdge(P, R, 97);

        addEdge(P, B, 101);
        addEdge(B, P, 101);

        addEdge(B, G, 90);
        addEdge(G, B, 90);

        addEdge(B, U, 85);
        addEdge(U, B, 85);

        addEdge(U, H, 98);
        addEdge(H, U, 98);

        addEdge(H, E, 86);
        addEdge(E, H, 86);

        addEdge(U, V, 142);
        addEdge(V, U, 142);

        addEdge(I, V, 92);
        addEdge(V, I, 92);

        addEdge(I, N, 87);
        addEdge(N, I, 87);
    }

private:
    int graph[20][20];
};

bool list[20];
vector<node> openList;
bool closeList[20];
stack<int> road;
int parent[20];

void A_star(int goal, node &src, Graph &graph)
{
    openList.push_back(src);
    sort(openList.begin(), openList.end());
    memset(closeList, false, sizeof(closeList));
    memset(parent, -1, sizeof(parent));
    
    while (!openList.empty())
    {
        node current = openList[0];
        openList.erase(openList.begin());
        
        if (current.name == goal)
        {
            openList.push_back(current);
            return;
        }
            
        closeList[current.name] = true;
        
        for (int i = 0; i < 20; i++)
        {
            int edge = graph.getEdge(current.name, i);
            if (edge != -1 && !closeList[i])
            {
                int newG = current.g + edge;
                bool inOpen = false;
                
                for (int j = 0; j < openList.size(); j++)
                {
                    if (openList[j].name == i)
                    {
                        inOpen = true;
                        if (newG < openList[j].g)
                        {
                            openList[j].g = newG;
                            openList[j].f = newG + openList[j].h;
                            parent[i] = current.name;
                        }
                        break;
                    }
                }
                
                if (!inOpen)
                {
                    node newNode(i, newG, h[i]);
                    openList.push_back(newNode);
                    parent[i] = current.name;
                }
            }
        }
        
        sort(openList.begin(), openList.end());
    }
}

void print_result(Graph &graph)
{
    if (openList.empty()) {
        cout << "No solution found!" << endl;
        return;
    }
    
    int p = openList[openList.size()-1].name;
    road.push(p);
    while (parent[p] != -1)
    {
        road.push(parent[p]);
        p = parent[p];
    }
    
    int cost = 0;
    cout << "solution: ";
    
    if (!road.empty()) {
        int current = road.top();
        road.pop();
        cout << current;
        
        while (!road.empty()) {
            int next = road.top();
            road.pop();
            cout << "-> " << next;
            cost += graph.getEdge(current, next);
            current = next;
        }
    }
    
    cout << "-> end" << endl;
    cout << "cost:" << cost << endl;
}