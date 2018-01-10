#include"Graphlnk.h"
template class Graphlnk<string, int>;
template<class T, class E>
Graphlnk<T, E>::Graphlnk(int sz /* = DeafultVertices */)
{
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	NodeTable = NULL;
	NodeTable = new Vertex<T, E>[maxVertices];
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
void  Graphlnk<T, E>::getNetMeg(int v1, int v2, string &x, string &y) {						//�����Ͳ���,�õ�����ź���������
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
template<class T, class E>
int Graphlnk<T, E>::getPort(int v1, int v2) {					//�õ��ӿں�
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
template<class T, class E>
void Graphlnk<T, E>::ShortestPath(int v1)
{//Graph��һ����Ȩ����ͼ�����㷨����һ�����飬dist[j],0<=j<n;�ǵ�ǰ�󵽵ĴӶ���v������j�����·�����ȣ�ͬʱ������path����󵽵����·��,S[]��־λ����Ƿ������·��
	int	v = getVertexPos(v1);				//�õ����v1�Ķ���λ��
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
	printRouTable(v, dist, path, S);
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
	NodeTable[v].borderNetNum = NodeTable[numVertices].borderNetNum;
	NodeTable[v].subNumber = NodeTable[numVertices].subNumber;
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
	cout << "ɾ���ɹ�" << endl;
	cout << "(�ֿɹ�ʹ�õ�·������: ";
	traverse();
	cout << endl;
	return true;
}


