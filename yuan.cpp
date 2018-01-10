#include"Graphlnk.h"
template class Graphlnk<string, int>;
template<class T, class E>
bool Graphlnk<T, E>::insertEdge(int v1, int v2, const Edge<T, E> edge)
{
	v1 = getVertexPos(v1);										//将路由表编号转化为邻接表中顶点位置
	v2 = getVertexPos(v2);
	int n;
	if (v1 >= 0 && v1< numVertices && v2 >= 0 && v2<numVertices)
	{
		Edge<T, E> *q, *p = NodeTable[v1].adj;
		while (p != NULL && p->dest != v2)
		{
			p = p->link;
		}
		if (p != NULL) return false;
		p = new Edge<T, E>;
		q = new Edge<T, E>;
		p->dest = v2;
		p->cost = edge.cost;
		p->link = NodeTable[v1].adj;
		p->netNum = edge.netNum;								//将网络号赋予边
		p->subNum = edge.subNum;								//将子网掩码赋给变
		n = NodeTable[v1].numofports;
		NodeTable[v1].por[n].num = n;							//依次接入接口
		NodeTable[v1].por[n].netNum = edge.netNum;				//将网络号传入到接口的网络号
		NodeTable[v1].numofports++;
		NodeTable[v1].adj = p;
		q->dest = v1;
		q->cost = edge.cost;
		q->netNum = edge.netNum;
		q->subNum = edge.subNum;
		n = NodeTable[v2].numofports;
		NodeTable[v2].por[n].num = n;							//依次接入接口
		NodeTable[v2].por[n].netNum = edge.netNum;				//将网络号传入到接口的网络号
		NodeTable[v2].numofports++;								//当前接口数目加一
		q->link = NodeTable[v2].adj;
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}
template<class T, class E>
bool Graphlnk<T, E>::removeEdge(int v1, int v2)
{
	v1 = getVertexPos(v1);										//将路由表编号转化为邻接表中顶点位置
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
int Graphlnk<T, E>::getVertexPos(const int num) {
	for (int i = 0; i< numVertices; i++)
		if (NodeTable[i].numRouter == num)
			return i;
	return -1;
}

