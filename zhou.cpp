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
		cout << "=======·��������ʵ��======" << endl;
		cout << "=     1.���·����        =" << endl;
		cout << "=     2.�����·          =" << endl;
		cout << "=     3.ɾ��·����        =" << endl;
		cout << "=     4.ɾ����·          =" << endl;
		cout << "=     5.���·�ɱ�        =" << endl;
		cout << "=     6.�˳�              =" << endl;
		cin >> x;
		switch (x)
		{
		case'1':
			cout << "(���Ѵ��ڵ�·������: ";
			traverse();					//��ȡ��ǰ��Ϣ
			cout << endl;
			cout << " ������·�����ı�ţ�·���������ƣ���������ţ�������������" << endl;
			cin >> Router.numRouter;
			cin >> Router.nameRouter;
			cin >> Router.borderNetNum;
			cin >> Router.subNumber;
			insertVertex(Router);
			break;
		case'2':
			cout << "����������·�ɵı��" << endl;
			cin >> v1 >> v2;
			cout << "��Ϊ������·��������š����������Լ�Ȩֵ" << endl;
			cin >> edge.netNum >> edge.subNum >> edge.cost;
			insertEdge(v1, v2, edge);
			break;

		case'3':
			cout << "(�ֿɹ�ɾ����·������: ";
			traverse();
			cout << endl;
			cout << "��������Ҫɾ����·�����ı��" << endl;
			cin >> num;
			removeVertex(num);
			break;
		case'4':
			cout << "(�ֿɹ�ʹ�õ�·������: ";
			traverse();
			cout << endl;
			cout << "��������Ҫɾ������·���˵�·�������" << endl;
			cin >> v1 >> v2;
			removeEdge(v1, v2);
			break;
		case'5':
			cout << "(�ֿɹ���ѯ��·����: ";
			traverse();
			cout << endl;
			cout << "����������Ҫ��ѯ��·�����ı��" << endl;
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
void Graphlnk<T, E>::printRouTable(int v, E dist[], int path[], bool S[])		//vΪnodetaλ��
{//���path�����д洢��ͼG�Ӷ���v������������·���;���
	cout << "·�ɱ�" << getValue(v) << "��·�ɱ�Ϊ:" << endl;
	int i, j, k, n = numVertices;
	int m = 0;
	string netN, subN;
	cout << "Ŀ �� �� ��  �� �� �� ��      ��һ��               | �� ��" << endl;
	int * d = new int[n];						//�������·��
	for (i = 0; i<n; i++)
	{
		if (i != v)
		{
			netN = NodeTable[i].borderNetNum;
			subN = NodeTable[i].subNumber;
			if (S[i] == false) {
				cout << netN << " |" << subN << " |" << "  ���ɴ�    " << "    |      " << maxValue << endl;
				continue;
			}
			j = i;
			k = 0;
			while (j != v)
			{
				d[k++] = j;					//d[k]����Ǳ��
				j = path[j];
			}
			k--;
			if (k == 0)
			{
				m = getPort(v, d[k]);				//�õ��Ľӿں�
				cout << netN << " |" << subN << " |" << "  ֱ�ӽ����ӿ�" << NodeTable[v].por[m].num << "    |    " << dist[i] << endl;
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
