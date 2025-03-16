# A*算法分析报告

## 1. 问题描述

根据说明.txt中的要求，本次任务是应用A*搜索算法找到从Fagaras(F)到Craiova(C)的路径。使用的评估函数为：

```
f(n) = g(n) + h(n)
```

其中h(n) = |hSLD(Craiova) - hSLD(n)|，hSLD(n)是从任意城市n到Bucharest的直线距离。

## 2. 代码结构分析

### 2.1 城市编码

代码使用宏定义将城市名称映射为整数编码：

```cpp
#define A 0  // Arad
#define B 1  // Bucharest
#define C 2  // Craiova
#define D 3  // Dobreta
#define E 4  // Eforie
#define F 5  // Fagaras
#define G 6  // Giurgiu
#define H 7  // Hirsova
#define I 8  // Iasi
#define L 9  // Lugoj
#define M 10 // Mehadia
#define N 11 // Neamt
#define O 12 // Oradea
#define P 13 // Pitesti
#define R 14 // Rimnicu Vilcea
#define S 15 // Sibiu
#define T 16 // Timisoara
#define U 17 // Urziceni
#define V 18 // Vaslui
#define Z 19 // Zerind
```

### 2.2 启发式函数

代码中定义了一个数组h[20]，存储了各个城市到Bucharest的直线距离：

```cpp
int h[20] =
{ 366,0,160,242,161,
178,77,151,226,244,
241,234,380,98,193,
253,329,80,199,374 };
```

这里h[F] = 178表示Fagaras到Bucharest的直线距离为178，h[C] = 160表示Craiova到Bucharest的直线距离为160。

### 2.3 图的表示

代码使用邻接矩阵表示城市之间的连接关系：

```cpp
class Graph
{
private:
    int graph[20][20];
public:
    // 构造函数初始化邻接矩阵
    Graph()
    {
        memset(graph, -1, sizeof(graph));
    }
    // 获取边的权重
    int getEdge(int from, int to)
    {
        return graph[from][to];
    }
    // 添加边
    void addEdge(int from, int to, int cost)
    {
        if (from >= 20 || from < 0 || to >= 20 || to < 0)
            return;
        graph[from][to] = cost;
    }
    // 初始化图
    void init(){
        // 添加所有城市之间的连接
        // ...
    }
};
```

### 2.4 节点结构

```cpp
struct node
{
    int g;  // 从起点到当前节点的实际代价
    int h;  // 启发式函数值
    int f;  // 评估函数值 f = g + h
    int name;  // 节点名称（城市编号）
    
    node(int name, int g, int h)
    {
        this->name = name;
        this->g = g;
        this->h = h;
        this->f = g + h;
    };
    
    // 重载小于运算符，用于优先队列排序
    bool operator <(const node &a)const
    {
        return f < a.f;
    }
};
```

### 2.5 A*算法实现

```cpp
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
        
        // 如果找到目标节点，则返回
        if (current.name == goal)
        {
            openList.push_back(current);
            return;
        }
            
        closeList[current.name] = true;
        
        // 扩展当前节点的所有邻居
        for (int i = 0; i < 20; i++)
        {
            int edge = graph.getEdge(current.name, i);
            if (edge != -1 && !closeList[i])
            {
                int newG = current.g + edge;
                bool inOpen = false;
                
                // 检查节点是否已在开放列表中
                for (int j = 0; j < openList.size(); j++)
                {
                    if (openList[j].name == i)
                    {
                        inOpen = true;
                        // 如果找到更短的路径，则更新
                        if (newG < openList[j].g)
                        {
                            openList[j].g = newG;
                            openList[j].f = newG + openList[j].h;
                            parent[i] = current.name;
                        }
                        break;
                    }
                }
                
                // 如果节点不在开放列表中，则添加
                if (!inOpen)
                {
                    node newNode(i, newG, h[i]);
                    openList.push_back(newNode);
                    parent[i] = current.name;
                }
            }
        }
        
        // 按f值排序开放列表
        sort(openList.begin(), openList.end());
    }
}
```

### 2.6 结果输出

```cpp
void print_result(Graph &graph)
{
    if (openList.empty()) {
        cout << "No solution found!" << endl;
        return;
    }
    
    // 从目标节点回溯到起点
    int p = openList[openList.size()-1].name;
    road.push(p);
    while (parent[p] != -1)
    {
        road.push(parent[p]);
        p = parent[p];
    }
    
    // 输出路径和总代价
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
```

## 3. 算法分析

### 3.1 启发式函数分析

根据题目要求，启发式函数h(n) = |hSLD(Craiova) - hSLD(n)| = |160 - hSLD(n)|，其中hSLD(n)是从城市n到Bucharest的直线距离。

对于目标节点Craiova(C)，h(C) = |160 - 160| = 0，符合启发式函数的要求。

### 3.2 启发式函数的可采纳性（Admissibility）

启发式函数h(n)是可采纳的，如果对于每个节点n，h(n) ≤ h*(n)，其中h*(n)是从n到目标的实际最小代价。

在本例中，我们需要证明|hSLD(Craiova) - hSLD(n)| ≤ 从n到Craiova的实际最短路径长度。

根据三角不等式，对于任意两个城市n和m，从n到m的实际路径长度必然大于等于它们之间的直线距离。因此，从n到Craiova的实际最短路径长度必然大于等于它们之间的直线距离。

而|hSLD(Craiova) - hSLD(n)|表示的是从n到Bucharest的直线距离与从Craiova到Bucharest的直线距离之差的绝对值，这个值小于等于n到Craiova的直线距离，更小于实际路径长度。

因此，该启发式函数是可采纳的。

### 3.3 一致性（Consistency）分析

启发式函数h(n)是一致的，如果对于每个节点n和它的后继m，h(n) ≤ c(n,m) + h(m)，其中c(n,m)是从n到m的代价。

在本例中，我们需要验证|hSLD(Craiova) - hSLD(n)| ≤ c(n,m) + |hSLD(Craiova) - hSLD(m)|。

由于直线距离满足三角不等式，因此|hSLD(n) - hSLD(m)| ≤ 从n到m的直线距离 ≤ c(n,m)。

所以，|hSLD(Craiova) - hSLD(n)| ≤ |hSLD(Craiova) - hSLD(m)| + |hSLD(m) - hSLD(n)| ≤ |hSLD(Craiova) - hSLD(m)| + c(n,m)。

因此，该启发式函数也是一致的。

## 4. 问题解答

### 4.1 问题(a)：证明一致的启发式函数必然是可采纳的

给定一个启发式函数h，如果h(G) = 0（G是任何目标状态），证明如果h是一致的，那么它必然是可采纳的。

**证明**：

假设h是一致的，那么对于任意节点n和它的后继m，有：
h(n) ≤ c(n,m) + h(m)

考虑从节点n到目标G的最优路径n→n1→n2→...→nk→G，根据一致性，我们有：

h(n) ≤ c(n,n1) + h(n1)
h(n1) ≤ c(n1,n2) + h(n2)
...
h(nk) ≤ c(nk,G) + h(G)

由于h(G) = 0，将这些不等式相加，我们得到：

h(n) ≤ c(n,n1) + c(n1,n2) + ... + c(nk,G) = h*(n)

其中h*(n)是从n到G的实际最小代价。因此，h(n) ≤ h*(n)，即h是可采纳的。

### 4.2 问题(b)：给出一个可采纳但不一致的启发式函数的例子

考虑以下启发式函数：

```
h(n) = {
    0, 如果n是目标节点
    min(实际代价, 一个常数K), 否则
}
```

其中K是一个足够小的常数，使得对于所有非目标节点n，h(n) ≤ h*(n)。

这个启发式函数是可采纳的，因为对于所有节点，h(n) ≤ h*(n)。

但它不一定是一致的。例如，如果有两个相邻节点n和m，它们到目标的实际代价分别是10和5，而K=3，那么h(n)=3，h(m)=3，c(n,m)=1。此时，h(n) = 3 > 1 + 3 = c(n,m) + h(m)，违反了一致性条件。

### 4.3 问题(c)：一致的启发式函数是否可能不可采纳

不可能。如问题(a)中所证明的，如果一个启发式函数是一致的且h(G)=0，那么它必然是可采纳的。

## 5. 总结

本报告分析了使用A*算法解决从Fagaras到Craiova的路径搜索问题。代码实现了完整的A*算法，包括图的表示、节点的定义、开放列表和关闭列表的管理、路径的回溯等。

使用的启发式函数h(n) = |hSLD(Craiova) - hSLD(n)|是可采纳的，保证了A*算法能找到最优解。同时，该启发式函数也是一致的，这意味着A*算法在搜索过程中不会重复扩展已经扩展过的节点，提高了算法效率。

通过A*算法的实现和分析，我们可以得出以下结论：

1. A*算法结合了Dijkstra算法和最佳优先搜索的优点，使用评估函数f(n) = g(n) + h(n)来指导搜索方向。

2. 当启发式函数h(n)是可采纳的时，A*算法保证能找到最优解。

3. 当启发式函数h(n)是一致的时，A*算法在搜索过程中不会重复扩展已经扩展过的节点，提高了算法效率。

4. 在本例中，从Fagaras到Craiova的最优路径应该是：Fagaras -> Sibiu -> Rimnicu Vilcea -> Pitesti -> Craiova，总代价为：99 + 80 + 97 + 138 = 414。

## 6. 代码运行

要完成这个程序，我们还需要添加main函数来执行A*算法并输出结果：

```cpp
int main()
{
    Graph graph;
    graph.init();
    
    // 创建起始节点（Fagaras）
    node src(F, 0, abs(h[C] - h[F]));
    
    // 执行A*算法，目标是Craiova
    A_star(C, src, graph);
    
    // 输出结果
    print_result(graph);
    
    return 0;
}
```

运行程序后，预期输出应该是从Fagaras到Craiova的最优路径和总代价。

## 7. 改进建议

1. 代码中使用了全局变量（openList, closeList, road, parent等），这不是一个好的编程实践。建议将这些变量封装在A*算法的类中。

2. 当前的实现使用vector作为开放列表，并在每次迭代后进行排序。为了提高效率，可以使用优先队列（priority_queue）来自动维护开放列表的顺序。

3. 城市名称使用宏定义映射为整数，这使得代码不够直观。可以考虑使用枚举类型或者字符串来表示城市名称，提高代码的可读性。

4. 可以添加更多的注释和文档，解释算法的每个步骤和关键决策。

5. 可以添加可视化功能，将搜索过程和最终路径显示在地图上，使结果更加直观。

通过这些改进，可以使A*算法的实现更加高效、可读和可维护。