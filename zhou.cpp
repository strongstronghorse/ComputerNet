#include"Graphlnk.h"
template class Graphlnk<string, int>;
template<class T, class E>
void Graphlnk<T, E>::menu()
{
	Edge<T, E> edge;
	Vertex<T, E> Router;
	char x, c;
	int v1, v2, num;
	readtext();
	while (1)
	{
		cout << "=======路由器功能实现======" << endl;
		cout << "=     1.添加路由器        =" << endl;
		cout << "=     2.添加链路          =" << endl;
		cout << "=     3.删除路由器        =" << endl;
		cout << "=     4.删除链路          =" << endl;
		cout << "=     5.输出路由表        =" << endl;
		cout << "=     6.退出              =" << endl;
		cin >> x;
		switch (x)
		{
		case'1':
			cout << "(现已存在的路由器有: ";
			traverse();					//读取当前信息
			cout << endl;
			cout << " 请输入路由器的编号，路由器的名称，网关网络号，网关子网掩码" << endl;
			cin >> Router.numRouter;
			cin >> Router.nameRouter;
			cin >> Router.borderNetNum;
			cin >> Router.subNumber;
			insertVertex(Router);
			break;
		case'2':
			cout << "请输入两端路由的编号" << endl;
			cin >> v1 >> v2;
			cout << "请为这条链路分配网络号、子网掩码以及权值" << endl;
			cin >> edge.netNum >> edge.subNum >> edge.cost;
			insertEdge(v1, v2, edge);
			break;

		case'3':
			cout << "(现可供删除的路由器有: ";
			traverse();
			cout << endl;
			cout << "请输入你要删除的路由器的编号" << endl;
			cin >> num;
			removeVertex(num);
			break;
		case'4':
			cout << "(现可供使用的路由器有: ";
			traverse();
			cout << endl;
			cout << "请输入你要删除的链路两端的路由器编号" << endl;
			cin >> v1 >> v2;
			removeEdge(v1, v2);
			break;
		case'5':
			cout << "(现可供查询的路由器: ";
			traverse();
			cout << endl;
			cout << "请输入你需要查询的路由器的编号" << endl;
			cin >> v1;
			ShortestPath(v1);
			break;
		case'6':
			exit(0);
			break;
		}

	}
}
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

}
template<class T, class E>
string Graphlnk<T, E>::merge(string net, string subnet)
{
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
void Graphlnk<T, E>::printRouTable(int v, E dist[], int path[], bool S[])		//v为nodeta位置
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
int  Graphlnk<T, E>::NumberOfVertices()
{
	return this->numVertices;
}
template<class T, class E>
T  Graphlnk<T, E>::getValue(int i)
{
	return (i >= 0 && i< numVertices) ? NodeTable[i].nameRouter : 0;
}
