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
	T getValue(int i);					//ȡλ��Ϊi�Ķ����е�ֵ
	E getWeight(int v1, int v2);			//���ر�(v1,v2)��Ȩֵ
	bool insertVertex(const Vertex<T, E> vertex);
	bool removeVertex(int v);
	bool insertEdge(int v1, int v2, const Edge<T, E> edge);
	bool removeEdge(int v1, int v2);
	void menu();
	string merge(string net, string subnet);
	void readtext();
	int NumberOfVertices();
	void getNetMeg(int v1, int v2, string &x, string &y);//�����Ͳ���,�õ�����ź���������
	int getPort(int v1, int v2);
	int getVertexPos(const int num);
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
#endif