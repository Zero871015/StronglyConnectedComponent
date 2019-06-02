//TODO: Get strongly connected component(SCC) given a directed graph.
//Date: 2019/06/02
//Auther: Zero871015(B10615032)

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Vertex
{
	vector<char> neighbors;	//this vertex can go where
	int start, end;			//DFS start and end time
	bool isTravered;
	char name;
};

map<char, Vertex> makeGraph();
void DFS(map<char, Vertex>& vertexs, Vertex* now, int& step);
bool isAllOK(map<char, Vertex> vertexs);
void initVertexs(map<char, Vertex> &vertexs, bool onlyTravered = false);
map<char, Vertex> getTransposeGraph(map<char, Vertex> &vertexs);
void getSCC(map<char, Vertex> vertexs);
void DFS(map<char, Vertex>& vertexs, Vertex* now, vector<char>& outputBuffer);

int main()
{
	int num_v;
	map<char, Vertex> vertexs;
	while (cin >> num_v)
	{
		//make graph
		vertexs = makeGraph();
		//initialize all vertexs
		initVertexs(vertexs);

		//get time of each vertex
		//first DFS
		int step = 0;
		for (char i = 'a'; !isAllOK(vertexs); i++)
		{
			if(!vertexs[i].isTravered)
				DFS(vertexs, &vertexs[i], ++step);
		}

		//get transpose of the graph
		map<char, Vertex> t_vertexs = getTransposeGraph(vertexs);

		//get SCC
		//second DFS
		getSCC(t_vertexs);
	}
	return 0;
}

//TODO: Given edges, make the directed graph.
//Each edge Separated by ';'
//In each edge, "pointA,pointB" mean pointA can go to pointB.
map<char, Vertex> makeGraph()
{
	char a, b;
	map<char, Vertex> vertexs;
	while (getchar() == ';')
	{
		scanf("%c,%c", &a, &b);
		if (vertexs.find(a) == vertexs.end())
		{
			Vertex v;
			v.name = a;
			vertexs.insert(pair<char, Vertex>(a, v));
		}
		if (vertexs.find(b) == vertexs.end())
		{
			Vertex v;
			v.name = b;
			vertexs.insert(pair<char, Vertex>(b, v));
		}
		vertexs[a].neighbors.push_back(b);
	}
	return vertexs;
}

//TODO: DFS to get start time and finish time for a graph.
void DFS(map<char, Vertex>& vertexs, Vertex* now, int& step)
{
	now->start = step;
	now->isTravered = true;

	for (int i = 0; i < now->neighbors.size(); i++)
	{
		if (vertexs[now->neighbors[i]].isTravered == false)
			DFS(vertexs, &vertexs[now->neighbors[i]], ++step);
	}
	step++;
	now->end = step;
}

//TODO: Give a graph, check all verteies in the graph has been gone.
bool isAllOK(map<char, Vertex> vertexs)
{
	for (auto i = vertexs.begin(); i != vertexs.end() ; i++)
	{
		if (i->second.isTravered == false)
			return false;
	}
	return true;
}

//TODO: Initialize a graph.
void initVertexs(map<char, Vertex> &vertexs,bool onlyTravered)
{
	for (auto i = vertexs.begin(); i != vertexs.end(); i++)
	{
		if (!onlyTravered)
		{
			i->second.start = 0;
			i->second.end = 0;
		}
		i->second.isTravered = false;
	}
}

//TODO: Given a graph, make a transpose graph.
map<char, Vertex> getTransposeGraph(map<char, Vertex> &vertexs)
{
	map<char, Vertex> t;
	for (auto i = vertexs.begin(); i != vertexs.end(); i++)
	{
		t[i->first].start = i->second.start;
		t[i->first].end = i->second.end;
		t[i->first].name = i->second.name;
	}
	for (auto i = vertexs.begin(); i != vertexs.end(); i++)
	{
		for (int j = 0; j < i->second.neighbors.size(); j++)
		{
			t[i->second.neighbors[j]].neighbors.push_back(i->first);
		}
	}

	return t;
}

bool cmp(Vertex a, Vertex b) { return a.end > b.end; };

//TODO: Given a graph, the graph should already have all finish time on every vertex.
//Print the SCC but not save data.
void getSCC(map<char, Vertex> vertexs)
{
	vector<vector<char>> ans;	//save all SCC

	//sort verteies by finish time from large to small
	vector<Vertex> s;
	for (auto i = vertexs.begin(); i != vertexs.end(); i++)
	{
		s.push_back(i->second);
	}
	sort(s.begin(), s.end(), cmp);


	//second DFS, order by finish time
	vector<char> SCC;	//save one SCC
	int step = 0;
	for (int i = 0; !isAllOK(vertexs); i++)
	{
		if (!vertexs[s[i].name].isTravered)
		{
			DFS(vertexs, &vertexs[s[i].name], SCC);
			//sort the SCC by letter
			sort(SCC.begin(), SCC.end());
			ans.push_back(SCC);
			SCC.clear();
		}
	}
	//sort all SCC by letter
	sort(ans.begin(), ans.end());

	//output answer
	for (int i = 0; i < ans.size(); i++)
	{
		if (i != 0)cout << ';';
		for (int j = 0; j < ans[i].size(); j++)
		{
			if (j != 0)cout << ',';
			cout << ans[i][j];
		}
	}
}

//TODO: DFS to get one SCC.
//The graph should already have finish time by first DFS.
void DFS(map<char, Vertex>& vertexs, Vertex* now, vector<char>& outputBuffer)
{
	now->isTravered = true;
	outputBuffer.push_back(now->name);

	for (int i = 0; i < now->neighbors.size(); i++)
	{
		if (vertexs[now->neighbors[i]].isTravered == false)
			DFS(vertexs, &vertexs[now->neighbors[i]], outputBuffer);
	}
}