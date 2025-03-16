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

// 用于存储不同搜索策略的路径和代价
struct SearchResult {
    vector<int> path;
    int cost;
};

// 清理全局变量，为下一次搜索做准备
void clearGlobalVars() {
    openList.clear();
    memset(closeList, false, sizeof(closeList));
    memset(parent, -1, sizeof(parent));
    while(!road.empty()) road.pop();
}

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

// 获取搜索结果，返回路径和代价
SearchResult get_search_result(Graph &graph)
{
    SearchResult result;
    result.cost = 0;
    
    if (openList.empty()) {
        return result;
    }
    
    int p = openList[openList.size()-1].name;
    road.push(p);
    while (parent[p] != -1)
    {
        road.push(parent[p]);
        p = parent[p];
    }
    
    vector<int> path;
    if (!road.empty()) {
        int current = road.top();
        road.pop();
        path.push_back(current);
        
        while (!road.empty()) {
            int next = road.top();
            road.pop();
            path.push_back(next);
            result.cost += graph.getEdge(current, next);
            current = next;
        }
    }
    
    result.path = path;
    return result;
}

// 广度优先搜索(BFS)
void BFS(int start, int goal, Graph &graph)
{
    clearGlobalVars();
    
    // 使用队列进行BFS
    vector<int> queue;
    bool visited[20] = {false};
    
    queue.push_back(start);
    visited[start] = true;
    memset(parent, -1, sizeof(parent));
    
    while (!queue.empty())
    {
        int current = queue[0];
        queue.erase(queue.begin());
        
        if (current == goal)
        {
            // 创建一个节点放入openList，以便使用现有的结果输出函数
            node goalNode(goal, 0, 0);
            openList.push_back(goalNode);
            return;
        }
        
        for (int i = 0; i < 20; i++)
        {
            int edge = graph.getEdge(current, i);
            if (edge != -1 && !visited[i])
            {
                queue.push_back(i);
                visited[i] = true;
                parent[i] = current;
            }
        }
    }
}


// 深度优先搜索(DFS)
bool DFS_recursive(int current, int goal, Graph &graph, bool visited[])
{
    if (current == goal)
    {
        // 创建一个节点放入openList，以便使用现有的结果输出函数
        node goalNode(goal, 0, 0);
        openList.push_back(goalNode);
        return true;
    }
    
    visited[current] = true;
    
    for (int i = 0; i < 20; i++)
    {
        int edge = graph.getEdge(current, i);
        if (edge != -1 && !visited[i])
        {
            parent[i] = current;
            if (DFS_recursive(i, goal, graph, visited))
                return true;
        }
    }
    
    return false;
}

void DFS(int start, int goal, Graph &graph)
{
    clearGlobalVars();
    
    bool visited[20] = {false};
    DFS_recursive(start, goal, graph, visited);
}

// 一致代价搜索(UCS)
struct UCSNode {
    int name;
    int cost;
    
    UCSNode(int name, int cost) : name(name), cost(cost) {}
    
    bool operator<(const UCSNode &other) const {
        return cost > other.cost; // 优先队列是最大堆，所以这里用>来实现最小堆
    }
};

void UCS(int start, int goal, Graph &graph)
{
    clearGlobalVars();
    
    vector<UCSNode> pq; // 优先队列
    bool visited[20] = {false};
    int cost[20]; // 存储从起点到每个节点的代价
    
    for (int i = 0; i < 20; i++)
        cost[i] = INT_MAX;
    
    pq.push_back(UCSNode(start, 0));
    cost[start] = 0;
    memset(parent, -1, sizeof(parent));
    
    while (!pq.empty())
    {
        // 找到代价最小的节点
        int minIdx = 0;
        for (int i = 1; i < pq.size(); i++)
            if (pq[i].cost < pq[minIdx].cost)
                minIdx = i;
        
        UCSNode current = pq[minIdx];
        pq.erase(pq.begin() + minIdx);
        
        int currentName = current.name;
        
        if (visited[currentName])
            continue;
        
        visited[currentName] = true;
        
        if (currentName == goal)
        {
            // 创建一个节点放入openList，以便使用现有的结果输出函数
            node goalNode(goal, 0, 0);
            openList.push_back(goalNode);
            return;
        }
        
        for (int i = 0; i < 20; i++)
        {
            int edge = graph.getEdge(currentName, i);
            if (edge != -1)
            {
                int newCost = cost[currentName] + edge;
                
                if (newCost < cost[i])
                {
                    cost[i] = newCost;
                    parent[i] = currentName;
                    pq.push_back(UCSNode(i, newCost));
                }
            }
        }
    }
}

int main()
{
    Graph graph;
    graph.init();
    
    cout << "===== A* Algorithm (Arad to Bucharest) =====" << endl;
    // 创建起始节点（Arad）
    node src(A, 0, abs(h[B] - h[A]));
    // 执行A*算法，目标是Bucharest
    A_star(B, src, graph);
    // 输出结果
    print_result(graph);
    
    cout << "\n===== BFS (Arad to Bucharest) =====" << endl;
    BFS(A, B, graph);
    print_result(graph);
    
    cout << "\n===== DFS (Arad to Bucharest) =====" << endl;
    DFS(A, B, graph);
    print_result(graph);
    
    cout << "\n===== UCS (Arad to Bucharest) =====" << endl;
    UCS(A, B, graph);
    print_result(graph);
    
    return 0;
}
