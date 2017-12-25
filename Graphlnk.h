#ifndef GRAPHLNK_H
#define GRAPHLNK_H
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
const int DefaultNumOfPort = 8;
const int DefaultVertices = 8;
const int MaxVertices = 100;	//ͼ�е���󶥵���Ŀ
const int maxValue = 10000;	//������Ȩֵ
const string invalidN = "0.0.0.0";//��Ч�����
const string invalidS = "255.255.255.255";//��Ч��������
template<class T, class E>
struct Edge
{
	int dest;
	string netNum;          //�����
	E cost;                 //Ȩֵ
	Edge<T, E> *link;
	string subNum;          //��������
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
	int num;		//�˿ں�
	string netNum;  //�˿����������
};
template<class T, class E>
struct Vertex
{
	int numRouter;				 //·�ɱ��
	T nameRouter;				 //·��������
	string borderNetNum;		//�߽�·���������
	string subNumber;			 //��������
	port *por;					 //�ӿ�����
	int numofports;				 //��ǰ�ӿ���
	Edge<T, E> *adj;			 //�������ͷָ��
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
	T getValue(int i)					//ȡλ��Ϊi�Ķ����е�ֵ
	{
		return (i >= 0 && i< numVertices) ? NodeTable[i].nameRouter : 0;
	}
	E getWeight(int v1, int v2);			//���ر�(v1,v2)��Ȩֵ
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
	void getNetMeg(int v1, int v2, string &x, string &y) {						//�����Ͳ���,�õ�����ź���������
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
	int getPort(int v1, int v2) {					//�õ��ӿں�
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
		cout << "�޷����ļ�" << endl;
		exit(1);
	}
	while (vfile.eof() != 1)
	{
		vfile >> Router.numRouter >> Router.nameRouter >> Router.borderNetNum >> Router.subNumber;
		insertVertex(Router);
	}
	efile.open("edges.txt");
	if (!efile) {
		cout << "�޷����ļ�" << endl;
		exit(1);
	}
	while (efile.eof() != 1)
	{
		efile >> edge.netNum >> edge.subNum >> v1 >> v2 >> edge.cost;
		insertEdge(v1, v2, edge);
	}

}