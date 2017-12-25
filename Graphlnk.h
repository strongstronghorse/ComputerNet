#ifndef GRAPHLNK_H
#define GRAPHLNK_H
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
const int DefaultNumOfPort = 8;
const int DefaultVertices = 8;
const int MaxVertices = 100;	//图中的最大顶点数�?
const int maxValue = 10000;	//无穷大的权�?
const string invalidN = "0.0.0.0";//无效网络�?
const string invalidS = "255.255.255.255";//无效子网掩码
template<class T, class E>
struct Edge
{
	int dest;
	string netNum;          //网络�?
	E cost;                 //权�?
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
	int num;		//端口�?
	string netNum;  //端口所属网络号
};
template<class T, class E>
struct Vertex
{
	int numRouter;				 //路由编号
	T nameRouter;				 //路由器名�?
	string borderNetNum;		//边界路由器网络号
	string subNumber;			 //子网掩码
	port *por;					 //接口数组
	int numofports;				 //当前接口�?
	Edge<T, E> *adj;			 //边链表的头指�?
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
	T getValue(int i)					//取位置为i的顶点中的�?
	{
		return (i >= 0 && i< numVertices) ? NodeTable[i].nameRouter : 0;
	}
	E getWeight(int v1, int v2);			//返回�?v1,v2)的权�?
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
	void getNetMeg(int v1, int v2, string &x, string &y) {						//传出型参�?得到网络号和子网掩码
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
	int getPort(int v1, int v2) {					//得到接口�?
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
void Graphlnk<T, E>::readtext() {
	ifstream vfile, efile;
	Edge<string, int> edge;
	Vertex<string, int> Router;
	int v1, v2;
	vfile.open("routers.txt");
	if (!vfile) {
		cout << "无法打开文件" << endl;
		exit(1);
	}
	while (vfile.eof() != 1)
	{
		vfile >> Router.numRouter >> Router.nameRouter >> Router.borderNetNum >> Router.subNumber;
		insertVertex(Router);
	}
	efile.open("edges.txt");
	if (!efile) {
		cout << "无法打开文件" << endl;
		exit(1);
	}
	while (efile.eof() != 1)
	{
		efile >> edge.netNum >> edge.subNum >> v1 >> v2 >> edge.cost;
		insertEdge(v1, v2, edge);
	}


template<class T, class E>
bool Graphlnk<T, E>::insertVertex(const Vertex<T, E> vertex)
{
	if (numVertices == maxVertices) return false;
	NodeTable[numVertices].nameRouter = vertex.nameRouter;
	NodeTable[numVertices].numRouter = vertex.numRouter;
	NodeTable[numVertices].borderNetNum = vertex.borderNetNum;
	NodeTable[numVertices].subNumber = vertex.subNumber;
	numVertices++;
	return true;

}

template<class T, class E>
bool Graphlnk<T, E>::removeVertex(int v)		//ͬʱɾ����������йصı�
{
	int n = 0;
	v = getVertexPos(v);				//�õ����v1�Ķ���λ��
	if (numVertices == 1 || v<0 || v >= numVertices)return false;
	Edge<T, E> *p, *s, *t;
	int i, k;
	while (NodeTable[v].adj != NULL)			//���ɾ�������ڽӵĵ��Ӧ��v
	{
		p = NodeTable[v].adj;
		k = p->dest;
		s = NodeTable[k].adj;					//�ҵ���������ڽӵĵ㲢ɾ����Ӧ��
		t = NULL;
		while (s != NULL && s->dest != v)
		{
			t = s;                           //tΪs����һ����
			s = s->link;					 //�ҵ��Ǹ���
		}									 //sָ����һ����Ϊv
		if (s != NULL)
		{
			if (t == NULL) NodeTable[k].adj = s->link;  //��ζ��s->dest==v��һ����
			else t->link = s->link;						//ǰһ����ͺ�һ����������
			delete s;
		}
		NodeTable[v].adj = p->link;				//��������һ��λ��
		delete p;
		numEdges--;								//ÿ��ɾ������һ��
	}
	numVertices--;
	NodeTable[v].numRouter = NodeTable[numVertices].numRouter;				//��ɾ�������Ϣ����Ϊ���һ�������Ϣ�����0��ʼ
	NodeTable[v].nameRouter = NodeTable[numVertices].nameRouter;
	NodeTable[v].numofports = NodeTable[numVertices].numofports;
	n = NodeTable[numVertices].numofports;											//�ӿ���Ϊ���һ������ӿ���
	for (int i = 0; i < n; i++)
	{
		NodeTable[v].por[i].num = NodeTable[numVertices].por[i].num;							//���ν���ӿ�
		NodeTable[v].por[i].netNum = NodeTable[numVertices].por[i].netNum;				//������Ŵ��뵽�ӿڵ������
	}
	p = NodeTable[v].adj = NodeTable[numVertices].adj;						//�����ظ���Ϣ
	while (p != NULL)
	{
		k = p->dest;
		s = NodeTable[k].adj;
		while (s != NULL) {
			if (s->dest == numVertices) {								 //ɾ���ظ����һ���㶥��λ�ö�Ӧ�ı�ʹ��ָ�����¸��ڽӵ㣬���������ɾ��
				s->dest = v;
				break;
			}
			else s = s->link;
		}
			p = p->link;									//��������һ���ڽӵ�
	}
	return true;
}

template<class T, class E>
E Graphlnk<T, E>::getWeight(int v1, int v2)
{
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E> *p = NodeTable[v1].adj;
		while (p != NULL && p->dest != v2)
		{
			p = p->link;
		}
		if (p != NULL)
			return p->cost;
		else return maxValue;
	}
}

