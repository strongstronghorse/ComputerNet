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
	T getValue(int i);					//取位置为i的顶点中的值
	E getWeight(int v1, int v2);			//返回边(v1,v2)的权值
	bool insertVertex(const Vertex<T, E> vertex);
	bool removeVertex(int v);
	bool insertEdge(int v1, int v2, const Edge<T, E> edge);
	bool removeEdge(int v1, int v2);
	void menu();
	string merge(string net, string subnet);
	void readtext();
	int NumberOfVertices();
	void getNetMeg(int v1, int v2, string &x, string &y);//传出型参数,得到网络号和子网掩码
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