#ifndef GRAPHLNK_H
#define GRAPHLNK_H
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
const int DefaultNumOfPort = 8;
const int DefaultVertices = 8;
const int MaxVertices = 100;	//图中的最大顶点数目
const int maxValue = 10000;	//无穷大的权值
const string invalidN = "0.0.0.0";//无效网络号
const string invalidS = "255.255.255.255";//无效子网掩码
template<class T, class E>
struct Edge
{
	int dest;
	string netNum;          //网络号
	E cost;                 //权值
	Edge<T, E> *link;
	string subNum;          //子网掩码
	Edge(int i_dest, string s_netNum, string subNum_mask, E i_cost)
	{
		dest = i_dest;
		netNum = s_netNum;
		cost = i_cost;
		subNum = subNum_mask;
	}
	Edge() { cost = maxValue; }
	bool operator != (Edge<T, E>& R)const
	{
		return (dest != R.dest) ? true : false;
	}
};
struct port {
	int num;		//端口号
	string netNum;  //端口所属网络号
};
template<class T, class E>
struct Vertex
{
	int numRouter;				 //路由编号
	T nameRouter;				 //路由器名称
	string borderNetNum;		//边界路由器网络号
	string subNumber;			 //子网掩码
	port *por;					 //接口数组
	int numofports;				 //当前接口数
	Edge<T, E> *adj;			 //边链表的头指针
	Vertex() {
		por = new port[DefaultNumOfPort];
		numofports = 0;
	}
};
template<class T, class E>
class Graphlnk
{
public:
	Graphlnk(int sz = MaxVertices);
	~Graphlnk();
	T getValue(int i)					//取位置为i的顶点中的值
	{
		return (i >= 0 && i< numVertices) ? NodeTable[i].nameRouter : 0;
	}
	E getWeight(int v1, int v2);			//返回边(v1,v2)的权值
	bool insertVertex(const Vertex<T, E> vertex);
	bool removeVertex(int v);
	bool insertEdge(int v1, int v2, const Edge<T, E> edge);
	bool removeEdge(int v1, int v2);
	string merge(string net, string subnet);
	void readtext();
	int NumberOfVertices()
	{
		return this->numVertices;
	}
	void getNetMeg(int v1, int v2, string &x, string &y) {						//传出型参数,得到网络号和子网掩码
		if (v1 != -1 && v2 != -1)
		{
			Edge<T, E> *p = NodeTable[v1].adj;
			while (p != NULL && p->dest != v2)
			{
				p = p->link;
			}
			if (p != NULL)
			{
				x = p->netNum;
				y = p->subNum;
			}
			else {
				x = invalidN;
				y = invalidS;
			}
		}

	}
	int getPort(int v1, int v2) {					//得到接口号
		if (v1 != -1 && v2 != -1)
		{
			Edge<T, E> *p = NodeTable[v1].adj;
			while (p != NULL && p->dest != v2)
			{
				p = p->link;
			}
			if (p != NULL)
				for (int i = 0; i < 20; i++)
				{
					if (NodeTable[v1].por[i].netNum == p->netNum)
						return i;
				}
			else return -1;
		}
	}
	int getVertexPos(const int num)
	{
		for (int i = 0; i< numVertices; i++)
			if (NodeTable[i].numRouter == num)
				return i;
		return -1;
	}
	void ShortestPath(int v);
	void printRouTable(int v, E dist[], int path[],bool S[]);
	void traverse() {											
		for (int i =0;i< NumberOfVertices(); i++)
			cout << " " << NodeTable[i].numRouter;
	}
private:
	Vertex<T, E> * NodeTable;
	int maxVertices;
	int numEdges;
	int numVertices;
};

template<class T, class E>
Graphlnk<T, E>::Graphlnk(int sz /* = DeafultVertices */)
{
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	NodeTable = NULL;
	try
	{
		NodeTable = new Vertex<T, E>[maxVertices];
	}
	catch (bad_alloc & memExp)
	{
		cerr << memExp.what() << endl;
	}
	for (int i = 0; i<maxVertices; i++)
	{
		NodeTable[i].adj = NULL;
	}
}

template<class T, class E>
Graphlnk<T, E>::~Graphlnk()
{
	for (int i = 0; i<maxVertices; i++)
	{
		Edge<T, E> * p = NodeTable[i].adj;
		while (p != NULL)
		{
			NodeTable[i].adj = p->link;
			delete p;
			p = NodeTable[i].adj;
		}
	}
	delete[] NodeTable;
}

template<class T, class E>
void Graphlnk<T, E>::ShortestPath(int v1)
{//Graph是一个带权有向图，本算法建立一个数组，dist[j],0<=j<n;是当前求到的从顶点v到顶点j的最短路径长度，同时用数组path存放求到的最短路径,S[]标志位存放是否有最短路径
	int	v = getVertexPos(v1);				//得到编号v1的顶点位置
	int n = numVertices;
	E *dist = new E[n];
	int *path = new int[n];
	bool *S = new bool[n];
	int i, j, k;
	E w, min;
	for (i = 0; i<n; i++)
	{
		dist[i] = getWeight(v, i);
		S[i] = false;
		if (i != v && dist[i] <maxValue)
			path[i] = v;
		else
		{
			path[i] = -1;
		}
	}
	S[v] = true;
	dist[v] = 0;
	for (i = 0; i<n - 1; i++)
	{
		min = maxValue;
		int u = v;
		for (j = 0; j<n; j++)
		{
			if (S[j] == false && dist[j] < min)
			{
				u = j;
				min = dist[j];
			}
		}
		S[u] = true;
		for (k = 0; k<n; k++)
		{
			w = getWeight(u, k);
			if (S[k] == false && w <maxValue && dist[u] + w<dist[k])
			{
				dist[k] = dist[u] + w;
				path[k] = u;
			}
		}
	}
	printRouTable(v, dist, path,S);
}

template<class T, class E>
void Graphlnk<T, E>::printRouTable(int v, E dist[], int path[],bool S[])		//v为nodeta位置
{//输出path数组中存储的图G从顶点v到其余各顶点的路径和距离
	cout << "路由表" << getValue(v) << "的路由表为:" << endl;
	int i, j, k, n = numVertices;
	int m = 0;
	string netN, subN;
	cout << "目 的 网 络  子 网 掩 码      下一跳               | 距 离" << endl;
	int * d = new int[n];						//保存最短路径
	for (i = 0; i<n; i++)
	{
		if (i != v)
		{
			netN = NodeTable[i].borderNetNum;
			subN = NodeTable[i].subNumber;
			if (S[i] == false) {
				cout << netN << " |" << subN << " |" << "  不可达    " << "    |      " << maxValue << endl;
				continue;
			}
			j = i;
			k = 0;
			while (j != v)
			{
				d[k++] = j;					//d[k]存的是编号
				j = path[j];
			}
			k--;
			if (k == 0)
			{
				m = getPort(v, d[k]);				//得到的接口号
				cout << netN << " |" << subN << " |" << "  直接交付接口" << NodeTable[v].por[m].num << "    |    " << dist[i] << endl;
			}
			else if (k >= 1)
			{
				cout << netN << " |" << subN << " |" << "     " << getValue(d[k]) << "      |    " << dist[i] << endl;

			}
		}
	}
	delete[] d;
}

template<class T, class E>
bool Graphlnk<T, E>::removeVertex(int v)		//同时删除和这个点有关的边
{
	int n = 0;
	v = getVertexPos(v);				//得到编号v1的顶点位置
	if (numVertices == 1 || v<0 || v >= numVertices)return false;
	Edge<T, E> *p, *s, *t;
	int i, k;
	while (NodeTable[v].adj != NULL)			//逐个删除与其邻接的点对应的v
	{
		p = NodeTable[v].adj;
		k = p->dest;
		s = NodeTable[k].adj;					//找到和这个点邻接的点并删除相应边
		t = NULL;
		while (s != NULL && s->dest != v)
		{
			t = s;                           //t为s的上一个点
			s = s->link;					 //找到那个点
		}									 //s指向下一个点为v
		if (s != NULL)
		{
			if (t == NULL) NodeTable[k].adj = s->link;  //意味着s->dest==v第一个点
			else t->link = s->link;						//前一个点和后一点连接起来
			delete s;
		}
		NodeTable[v].adj = p->link;				//遍历到下一个位置
		delete p;
		numEdges--;								//每次删除边数一条
	}
	numVertices--;
	NodeTable[v].numRouter = NodeTable[numVertices].numRouter;				//将删除点的信息复制为最后一个点的信息，点从0开始
	NodeTable[v].nameRouter = NodeTable[numVertices].nameRouter;
	NodeTable[v].numofports = NodeTable[numVertices].numofports;
	NodeTable[v].borderNetNum = NodeTable[numVertices].borderNetNum;
	NodeTable[v].subNumber = NodeTable[numVertices].subNumber;
	n = NodeTable[numVertices].numofports;											//接口数为最后一个顶点接口数
	for (int i = 0; i < n; i++)
	{
		NodeTable[v].por[i].num = NodeTable[numVertices].por[i].num;							//依次接入接口
		NodeTable[v].por[i].netNum = NodeTable[numVertices].por[i].netNum;				//将网络号传入到接口的网络号
	}
	p = NodeTable[v].adj = NodeTable[numVertices].adj;						//俩个重复信息
	while (p != NULL)
	{
		k = p->dest;
		s = NodeTable[k].adj;
		while (s != NULL) {
			if (s->dest == numVertices) {								 //删除重复最后一个点顶点位置对应的边使其指向下下个邻接点，类似上面的删除
				s->dest = v;
				break;
			}
			else s = s->link;
		}
		p = p->link;									//遍历到下一个邻接点
	}
	cout << "删除成功" << endl;
	cout << "(现可供使用的路由器有: ";
	traverse();
	cout << endl;
	return true;
}
