#include "Mesh.h"
#include "Painter.h"
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>

using namespace std;

int getRandVertexPoint(Mesh* mesh);
float getDistance(float* refCoord, float* targetCoord);
float* Dijkstra(float** M, int src, int targ, int N, int* prev);
int minDistance(float* dist, bool* marked, int N);

int main(int, char** argv)
{
	

	srand(time(NULL)); //to get random two vertices

	//Coin3D Initializations
	HWND window = SoWin::init(argv[0]);
	SoWinExaminerViewer* viewer = new SoWinExaminerViewer(window);
	SoSeparator* root = new SoSeparator;
	root->ref();

	Mesh* mesh = new Mesh();
	Painter* painter = new Painter();
	mesh->loadOff("man0.off");

	int N = mesh->verts.size();
	float **M;
	float** M_print;
	int* prev;
	prev = new int[N];
	M = new float* [N];
	M_print = new float* [N]; //we will save this matrix after filling it

	//memory allocation
	for (int i = 0; i < N; i++) { 
		M[i] = new float[N];
		M_print[i] = new float[N];
	}

	//initialization of matrices
	for (int i = 0; i < N; i++) { 
		for (int j = 0; j < N; j++) {
			M[i][j] = 0.0f;
			M_print[i][j] = 0.0f;
		}
	}

	//adjacency matrix creation 
	for (int i = 0; i < N; i++) { 
		for (int j = 0; j < mesh->verts[i]->vertList.size(); j++) { 
			float dist = getDistance(mesh->verts[i]->coords, mesh->verts[mesh->verts[i]->vertList[j]]->coords);
			M[i][mesh->verts[i]->vertList[j]] = dist;
			M_print[i][mesh->verts[i]->vertList[j]] = dist;
		}
	}

	//These are the vertices which will geodesic distance calculated on
	int V_source = getRandVertexPoint(mesh);
	int V_target = getRandVertexPoint(mesh);

	//Prev will be used for painting shortest path
	Dijkstra(M, V_source,V_target, N, prev);

	


	root->addChild(painter->getShapeSep(mesh));
	root->addChild(painter->get1PointSep(mesh, V_source, 2, 0, 0, 1));
	root->addChild(painter->get1PointSep(mesh, V_target, 2, 0, 0, 1));
	root->addChild(painter->DrawLines(mesh,V_source,V_target,N, prev));
	viewer->setSize(SbVec2s(640, 480));
	viewer->setSceneGraph(root);
	viewer->show();

	SoWin::show(window);
	SoWin::mainLoop();
	delete viewer;
	root->unref();



	for (int i = 0; i < N; i++) {
		M_print[i] = Dijkstra(M, i, 0, N, prev);
	}

	FILE* matrix_text;

	matrix_text = fopen("geodesic_distance_matrix.txt", "w+");

	for (int y = 0; y < N; y++) {
		fprintf(matrix_text, "\n");
		for (int x = 0; x < N; x++)
		{
			fprintf(matrix_text, "%0.7g ",M_print[y][x]);
		}
	}
	fclose(matrix_text);

	//Memory leakage prevention
	for (int i = 0; i < N; i++) {
		delete[] M_print[i];
		delete[] M[i];
	}
	
	delete[] prev;
	return 0;
}

int minDistance(float* dist, bool* marked, int N)
{
	// Initialize min value
	float min = INT_MAX, min_index;

	for (int v = 0; v < N; v++)
		if (marked[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

//Dijkstra algorithm
float* Dijkstra(float** M, int src,int targ, int N, int* prev )
{
	float* dist = new float[N];
	bool* marked = new bool[N];
	for (int i = 0; i < N; i++)
	{
		prev[i] = -1; //source node
		dist[i] = INT_MAX;
		marked[i] = false;
	}
	dist[src] = 0;

	for (int count = 0; count < N - 1; count++)
	{
		int u = minDistance(dist, marked, N);

		marked[u] = true;

		for (int v = 0; v < N; v++) {
			if (!marked[v] && M[u][v] && (dist[u] + M[u][v] < dist[v]))
			{
				prev[v] = u;
				dist[v] = dist[u] + M[u][v];
			}
		}
	}
	delete marked;
	return dist;
}

int getRandVertexPoint(Mesh* mesh) {

	int randV1 = rand() % (mesh->verts.size());
	return randV1;
}

//returns distance between two 3D points
float getDistance(float* refCoord, float* targetCoord) {
	
	float distance = (
		pow(refCoord[0] - targetCoord[0], 2) +
		pow(refCoord[1] - targetCoord[1], 2) +
		pow(refCoord[2] - targetCoord[2], 2));

	return sqrt(distance);


}