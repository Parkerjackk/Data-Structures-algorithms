#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "t3.h"

stop *vertices[MAX_VERTICES];
Graph *g;
int vertCount = 0;

// CSV parser

int nextField(char *buf, FILE *f, int max)
{
    int i = 0, end = 0, quoted = 0;

    while (!feof(f))
    {
        // fetch the next character from file
        buf[i] = fgetc(f);
        // if we encounter quotes then flip our state and immediately fetch next char
        if (buf[i] == '"')
        {
            quoted = !quoted;
            buf[i] = fgetc(f);
        }
        // end of field on comma if we're not inside quotes
        if (buf[i] == ',' && !quoted)
        {
            break;
        }
        // end record on newline or end of file
        if (feof(f) || buf[i] == '\n')
        {
            end = 1;
            break;
        }
        // truncate fields that would overflow the buffer
        if (i < max - 1)
        {
            ++i;
        }
    }

    buf[i] = 0; // null terminate the string
    return end; // flag stating whether or not this is end of the line
}

stop *stopCollect(FILE *csv)
{
    char buf[MAX_NAME_LENTGH];

    stop *newStop = (stop *)malloc(sizeof(stop));

    nextField(buf, csv, MAX_NAME_LENTGH);
    newStop->StopId = atoi(buf);
    nextField(newStop->name, csv, MAX_NAME_LENTGH);
    nextField(buf, csv, MAX_NAME_LENTGH);
    newStop->Latitude = atof(buf);
    nextField(buf, csv, MAX_NAME_LENTGH);
    newStop->Longitude = atof(buf);
    vertCount++;
    return newStop;
}

edges *edgeCollect(FILE *csv)
{
    char buf[MAX_NAME_LENTGH];

    edges *newEdge = (edges *)malloc(sizeof(edges));

    nextField(buf, csv, MAX_NAME_LENTGH);
    newEdge->vert1 = atoi(buf);
    nextField(buf, csv, MAX_NAME_LENTGH);
    newEdge->vert2 = atoi(buf);
    nextField(buf, csv, MAX_NAME_LENTGH);
    newEdge->weight = atoi(buf);
    //vertCount++;
    return newEdge;
}

void initializeGraph()
{
    g = (Graph *)malloc(sizeof(Graph));
    g->numVertices = MAX_VERTICES;
    g->adjacency_matrix = (int **)malloc(MAX_VERTICES * sizeof(int *));
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        g->adjacency_matrix[i] = (int *)malloc(MAX_VERTICES * sizeof(int));
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            g->adjacency_matrix[i][j] = 0;
        }
    }
}

void addEdge(Graph *g, int from, int to, int weight)
{
    g->adjacency_matrix[from][to] = g->adjacency_matrix[to][from] = weight;
}

int load_edges(char *fname)
{
    int to = 0, from = 0, counter = 0;
    int weight;

    FILE *f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open file %s \n", fname);
        return 0;
    }
    edgeCollect(f); // gets rid of header
    initializeGraph();

    edges *tmp = (edges *)malloc(sizeof(edges));

    while (!feof(f))
    {
        tmp = edgeCollect(f);
        from = tmp->vert1;
        to = tmp->vert2;
        weight = tmp->weight;
        addEdge(g, from, to, weight);
        counter++; // just to check if all values are being collected
    }

    printf("Loaded %d edges\n", counter);
    fclose(f);
    free(tmp);

    return 1;
}

int load_vertices(char *fname)
{
    int counter = 0;
    FILE *f = fopen(fname, "r");
    if (!f)
    {
        printf("Unable to open file %s\n", fname);
        return 0;
    }
    stopCollect(f); // disregard header

    stop *tmp = (stop *)malloc(sizeof(stop));

    while (!feof(f))
    {
        tmp = stopCollect(f);
        vertices[tmp->StopId] = tmp;
        counter++;
    }
    tmp = NULL;

    printf("Loaded %d vertices\n", counter);
    fclose(f);
    free(tmp);

    return 1;
}

int min_distance(int dist[], int perm[], int vertCount)
{
    int min = INT_MAX, min_dist = -1;

    for (int v = 0; v < vertCount; ++v)
    {
        // iterates through from 0 - num_nodes, if the condition is true, values are updated
        if (perm[v] == 0 && dist[v] < min)
        {
            min = dist[v];
            min_dist = v;
        }
    }

    return min_dist;
}

void printInfo(int nodeIndex, stop **vertices)
{
    printf("%d %s \n", vertices[nodeIndex]->StopId, vertices[nodeIndex]->name);
}

void printPath(int *path, int origin, int destination, stop **vertices)
{
    if (path[destination] == -1 || origin == destination)
    {
        printInfo(destination, vertices);
    }
    else
    {
        printPath(path, origin, path[destination], vertices);
        printInfo(destination, vertices);
    }
}

// Function taht implements the Dikstra algorithm
void dijkstra(Graph *g, int origin, int destination, stop **vertices)
{
    //int vertCount = g->numVertices;
    int *distance = (int *)malloc(vertCount * sizeof(int));
    int *permanent = (int *)malloc(vertCount * sizeof(int));
    int *path = (int *)malloc(vertCount * sizeof(int));

    // initalize distance and permanent arrays
    for (int i = 0; i < vertCount; ++i)
    {
        if (i == origin)
        {
            distance[i] = 0;
        }
        else
        {
            distance[i] = INT_MAX;
        }
        permanent[i] = 0;
        path[i] = -1; // Initializing path array with -1
    }
    // Distance to origin = 0
    distance[origin] = 0;

    for (int count = 0; count < vertCount; ++count)
    {
        // Search for node with min distance among the current unvisited nodes
        int u = min_distance(distance, permanent, vertCount);
        permanent[u] = 1; // Becomes permanent

        // update distance for neighbour nodes if a shorter path is then found
        for (int v = 0; v < vertCount; ++v)
        {
            // preventing overflow
            if (!permanent[v] && g->adjacency_matrix[u][v] != INT_MAX &&
                distance[u] != INT_MAX &&
                distance[u] + g->adjacency_matrix[u][v] < distance[v])
            {
                distance[v] = distance[u] + g->adjacency_matrix[u][v];
                path[v] = u; //stores the previous node in the path
            }
        }
    }
    // for (int i = 0; i < 10; i++){
    //     printf("%d %d %d\n", origin, path[i], destination);
    // }
    printPath(path, origin, destination, vertices);
    printf("\n");



    // Free arrays
    free(distance);
    free(permanent);
    free(path);
}

void shortest_path(int startNode, int endNode)
{
    dijkstra(g, startNode, endNode, vertices);
}

// Function to delete the graph and all its data structures
void delete_graph(Graph *g)
{
    for (int i = 0; i < vertCount; ++i)
    {
        free(g->adjacency_matrix[i]);
    }
    free(g->adjacency_matrix);
    free(g);
}
void free_memory(void)
{
    delete_graph(g);
}

    int main() {
    char edges_file[] = "edges.csv";
    char vertices_file[] = "vertices.csv";
    int startNode = 300;  // Example start node
    int endNode = 253;    // Example end node

    // Load vertices and edges
    if (!load_vertices(vertices_file) || !load_edges(edges_file)) {
        printf("Failed to load vertices or edges.\n");
        return 1;
    }

    // Print the shortest path
    shortest_path(startNode, endNode);

    // Free memory
    free_memory();

    return 0;
}
