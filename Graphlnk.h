#ifndef GRAPHLNK_H
#define GRAPHLNK_H
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
const int DefaultNumOfPort = 8;
const int DefaultVertices = 8;
const int MaxVertices = 100;	//å›¾ä¸­çš„æœ€å¤§é¡¶ç‚¹æ•°ç›?
const int maxValue = 10000;	//æ— ç©·å¤§çš„æƒå€?
const string invalidN = "0.0.0.0";//æ— æ•ˆç½‘ç»œå?
const string invalidS = "255.255.255.255";//æ— æ•ˆå­ç½‘æŽ©ç 
template<class T, class E>
struct Edge
{
	int dest;
	string netNum;          //ç½‘ç»œå?
	E cost;                 //æƒå€?
	Edge<T, E> *link;
	string subNum;          //å­ç½‘æŽ©ç 
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
	int num;		//ç«¯å£å?
	string netNum;  //ç«¯å£æ‰€å±žç½‘ç»œå·
};
template<class T, class E>
struct Vertex
{
	int numRouter;				 //è·¯ç”±ç¼–å·
	T nameRouter;				 //è·¯ç”±å™¨åç§?
	string borderNetNum;		//è¾¹ç•Œè·¯ç”±å™¨ç½‘ç»œå·
	string subNumber;			 //å­ç½‘æŽ©ç 
	port *por;					 //æŽ¥å£æ•°ç»„
	int numofports;				 //å½“å‰æŽ¥å£æ•?
	Edge<T, E> *adj;			 //è¾¹é“¾è¡¨çš„å¤´æŒ‡é’?
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
	T getValue(int i)					//å–ä½ç½®ä¸ºiçš„é¡¶ç‚¹ä¸­çš„å€?
	{
		return (i >= 0 && i< numVertices) ? NodeTable[i].nameRouter : 0;
	}
	E getWeight(int v1, int v2);			//è¿”å›žè¾?v1,v2)çš„æƒå€?
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
	void getNetMeg(int v1, int v2, string &x, string &y) {						//ä¼ å‡ºåž‹å‚æ•?å¾—åˆ°ç½‘ç»œå·å’Œå­ç½‘æŽ©ç 
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
	int getPort(int v1, int v2) {					//å¾—åˆ°æŽ¥å£å?
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
		cout << "æ— æ³•æ‰“å¼€æ–‡ä»¶" << endl;
		exit(1);
	}
	while (vfile.eof() != 1)
	{
		vfile >> Router.numRouter >> Router.nameRouter >> Router.borderNetNum >> Router.subNumber;
		insertVertex(Router);
	}
	efile.open("edges.txt");
	if (!efile) {
		cout << "æ— æ³•æ‰“å¼€æ–‡ä»¶" << endl;
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
bool Graphlnk<T, E>::removeVertex(int v)		//Í¬Ê±É¾³ýºÍÕâ¸öµãÓÐ¹ØµÄ±ß
{
	int n = 0;
	v = getVertexPos(v);				//µÃµ½±àºÅv1µÄ¶¥µãÎ»ÖÃ
	if (numVertices == 1 || v<0 || v >= numVertices)return false;
	Edge<T, E> *p, *s, *t;
	int i, k;
	while (NodeTable[v].adj != NULL)			//Öð¸öÉ¾³ýÓëÆäÁÚ½ÓµÄµã¶ÔÓ¦µÄv
	{
		p = NodeTable[v].adj;
		k = p->dest;
		s = NodeTable[k].adj;					//ÕÒµ½ºÍÕâ¸öµãÁÚ½ÓµÄµã²¢É¾³ýÏàÓ¦±ß
		t = NULL;
		while (s != NULL && s->dest != v)
		{
			t = s;                           //tÎªsµÄÉÏÒ»¸öµã
			s = s->link;					 //ÕÒµ½ÄÇ¸öµã
		}									 //sÖ¸ÏòÏÂÒ»¸öµãÎªv
		if (s != NULL)
		{
			if (t == NULL) NodeTable[k].adj = s->link;  //ÒâÎ¶×Ås->dest==vµÚÒ»¸öµã
			else t->link = s->link;						//Ç°Ò»¸öµãºÍºóÒ»µãÁ¬½ÓÆðÀ´
			delete s;
		}
		NodeTable[v].adj = p->link;				//±éÀúµ½ÏÂÒ»¸öÎ»ÖÃ
		delete p;
		numEdges--;								//Ã¿´ÎÉ¾³ý±ßÊýÒ»Ìõ
	}
	numVertices--;
	NodeTable[v].numRouter = NodeTable[numVertices].numRouter;				//½«É¾³ýµãµÄÐÅÏ¢¸´ÖÆÎª×îºóÒ»¸öµãµÄÐÅÏ¢£¬µã´Ó0¿ªÊ¼
	NodeTable[v].nameRouter = NodeTable[numVertices].nameRouter;
	NodeTable[v].numofports = NodeTable[numVertices].numofports;
	n = NodeTable[numVertices].numofports;											//½Ó¿ÚÊýÎª×îºóÒ»¸ö¶¥µã½Ó¿ÚÊý
	for (int i = 0; i < n; i++)
	{
		NodeTable[v].por[i].num = NodeTable[numVertices].por[i].num;							//ÒÀ´Î½ÓÈë½Ó¿Ú
		NodeTable[v].por[i].netNum = NodeTable[numVertices].por[i].netNum;				//½«ÍøÂçºÅ´«Èëµ½½Ó¿ÚµÄÍøÂçºÅ
	}
	p = NodeTable[v].adj = NodeTable[numVertices].adj;						//Á©¸öÖØ¸´ÐÅÏ¢
	while (p != NULL)
	{
		k = p->dest;
		s = NodeTable[k].adj;
		while (s != NULL) {
			if (s->dest == numVertices) {								 //É¾³ýÖØ¸´×îºóÒ»¸öµã¶¥µãÎ»ÖÃ¶ÔÓ¦µÄ±ßÊ¹ÆäÖ¸ÏòÏÂÏÂ¸öÁÚ½Óµã£¬ÀàËÆÉÏÃæµÄÉ¾³ý
				s->dest = v;
				break;
			}
			else s = s->link;
		}
			p = p->link;									//±éÀúµ½ÏÂÒ»¸öÁÚ½Óµã
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

