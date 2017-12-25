#ifndef GRAPHLNK_H
#define GRAPHLNK_H
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
const int DefaultNumOfPort = 8;
const int DefaultVertices = 8;
const int MaxVertices = 100;	//鍥句腑鐨勬渶澶ч《鐐规暟�?
const int maxValue = 10000;	//鏃犵┓澶х殑鏉冨�?
const string invalidN = "0.0.0.0";//鏃犳晥缃戠粶�?
const string invalidS = "255.255.255.255";//鏃犳晥瀛愮綉鎺╃爜
template<class T, class E>
struct Edge
{
	int dest;
	string netNum;          //缃戠粶鍙?
	E cost;                 //鏉冨�?
	Edge<T, E> *link;
	string subNum;          //瀛愮綉鎺╃爜
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
	int num;		//绔彛鍙?
	string netNum;  //绔彛鎵�灞炵綉缁滃�?
};
template<class T, class E>
struct Vertex
{
	int numRouter;				 //璺敱缂栧彿
	T nameRouter;				 //璺敱鍣ㄥ悕�?
	string borderNetNum;		//杈圭晫璺敱鍣ㄧ綉缁滃彿
	string subNumber;			 //瀛愮綉鎺╃爜
	port *por;					 //鎺ュ彛鏁扮粍
	int numofports;				 //褰撳墠鎺ュ彛�?
	Edge<T, E> *adj;			 //杈归摼琛ㄧ殑澶存寚閽?
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
	T getValue(int i)					//鍙栦綅缃负i鐨勯《鐐逛腑鐨勫�?
	{
		return (i >= 0 && i< numVertices) ? NodeTable[i].nameRouter : 0;
	}
	E getWeight(int v1, int v2);			//杩斿洖杈?v1,v2)鐨勬潈鍊?
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
	void getNetMeg(int v1, int v2, string &x, string &y) {						//浼犲嚭鍨嬪弬�?寰楀埌缃戠粶鍙峰拰瀛愮綉鎺╃爜
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
	int getPort(int v1, int v2) {					//寰楀埌鎺ュ彛�?
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
		cout << "鏃犳硶鎵撳紑鏂囦�? << endl;
		exit(1);
	}
	while (vfile.eof() != 1)
	{
		vfile >> Router.numRouter >> Router.nameRouter >> Router.borderNetNum >> Router.subNumber;
		insertVertex(Router);
	}
	efile.open("edges.txt");
	if (!efile) {
		cout << "鏃犳硶鎵撳紑鏂囦�? << endl;
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
bool Graphlnk<T, E>::removeEdge(int v1, int v2)
{   cout<<"�ܽ���"<<endl;
	v1 = getVertexPos(v1);										//将路由表编号转化为邻接表中顶点位�?
	v2 = getVertexPos(v2);
	if (v1 != -1 && v2 != -1)
	{
		Edge<T, E> *p = NodeTable[v1].adj, *q = NULL, *s = p;
		while (p != NULL && p->dest != v2)						//找到对应边在邻接表的位置
		{
			q = p;
			p = p->link;
		}
		if (p != NULL)
		{
			if (p == s)											//v1第一个邻接点为v2
				NodeTable[v1].adj = p->link;
			else
			{
				q->link = p->link;
			}
			delete p;
		}
		else
		{
			return false;
		}
		p = NodeTable[v2].adj;
		q = NULL;
		s = p;
		while (p->dest != v1)
		{
			q = p;
			p = p->link;
		}
		if (p == s)
			NodeTable[v2].adj = p->link;
		else
		{
			q->link = p->link;
		}
		delete p;
		numEdges--;
		cout << " 删除成功" << endl;
		
		return true;
	}
	cout << "删除失败" << endl;
	return false;
}
string Graphlnk<T, E>::merge(string net, string subnet)
{   cout<<"�ܽ���"<<endl;
	string a;
	int length = net.length() - 1;
	int sublength = subnet.length() - 1;
	int part[4] = { 0,0,0,0 };
	int spart[4] = { 0,0,0,0 };
	int fpart[4] = { 0,0,0,0 };
	int tag = 1;
	int t = 0;
	int temp = 0;
	for (int i = length; i >= 0; i--)
	{
		if (net[i] != '.')
		{
			temp = (net[i] - 48) * tag;
			part[t] += temp;
			tag *= 10;

		}
		else {
			t++;
			tag = 1;
		}
	}
	tag = 1;
	t = 0;
	temp = 0;
	for (int i = sublength; i >= 0; i--)
	{
		if (subnet[i] != '.')
		{
			temp = (subnet[i] - 48) * tag;
			spart[t] += temp;
			tag *= 10;

		}
		else {
			t++;
			tag = 1;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		fpart[i] = spart[i] & part[i];
	}
	a = to_string(fpart[3]) + '.' + to_string(fpart[2]) + '.' + to_string(fpart[1]) + '.' + to_string(fpart[0]);
	return  a;
}


template<class T, class E>
bool Graphlnk<T, E>::removeVertex(int v)		//同时删除和这个点有关的边
{   
	int n = 0;
	v = getVertexPos(v);				//得到编号v1的顶点位�?
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
			s = s->link;					 //找到那个�?
		}									 //s指向下一个点为v
		if (s != NULL)
		{
			if (t == NULL) NodeTable[k].adj = s->link;  //意味着s->dest==v第一个点
			else t->link = s->link;						//前一个点和后一点连接起�?
			delete s;
		}
		NodeTable[v].adj = p->link;				//遍历到下一个位�?
		delete p;
		numEdges--;								//每次删除边数一�?
	}
	numVertices--;
	NodeTable[v].numRouter = NodeTable[numVertices].numRouter;				//将删除点的信息复制为最后一个点的信息，点从0开�?
	NodeTable[v].nameRouter = NodeTable[numVertices].nameRouter;
	NodeTable[v].numofports = NodeTable[numVertices].numofports;
	n = NodeTable[numVertices].numofports;											//接口数为最后一个顶点接口数
	for (int i = 0; i < n; i++)
	{
		NodeTable[v].por[i].num = NodeTable[numVertices].por[i].num;							//依次接入接口
		NodeTable[v].por[i].netNum = NodeTable[numVertices].por[i].netNum;				//将网络号传入到接口的网络�?
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

#endif
