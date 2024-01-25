#include "t2.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//function to convert integer values to chars
char int2char(int num){
    num = num + 'A';
    return (char)num;
}

// Function to create a graph with num_nodes nodes
Graph *create_graph(int num_nodes)
{
    //Allocate memory in the heap for the graph
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->num_nodes = num_nodes;      //set number of nodes of the graph object to be the value passed to the function

    // Allocate memory in heap for adjacency matrix
    graph->adjacency_matrix = (int **)malloc(num_nodes * sizeof(int *));
    for (int i = 0; i < num_nodes; ++i)
    {
        graph->adjacency_matrix[i] = (int *)malloc(num_nodes * sizeof(int));
        for (int j = 0; j < num_nodes; ++j)
        {
            graph->adjacency_matrix[i][j] = INT_MAX; // Initialize all with ihighest Integer value
        }
    }

    return graph;
}

//function that adds a weighted edge between from and to 
void add_edge(Graph *g, int from, int to, int weight)
{
    g->adjacency_matrix[from][to] = weight;
    g->adjacency_matrix[to][from] = weight; //needs to be symetric for undirected graph
}

//Function to find the vertex with the minimum distance value
int min_distance(int dist[], int perm[], int num_nodes)
{
    int min = INT_MAX, min_dist = -1;

    for (int v = 0; v < num_nodes; ++v)
    {
        //iterates through from 0 - num_nodes, if the condition is true, values are updated
        if (perm[v] == 0 && dist[v] < min)
        {
            min = dist[v];
            min_dist = v;
        }
    }

    return min_dist;
}

// Function taht implements the Dikstra algorithm
void dijkstra(Graph *g, int origin)
{
    int *distance = (int *)malloc(g->num_nodes * sizeof(int));
    int *permanent = (int *)malloc(g->num_nodes * sizeof(int));

    // initalize distance and permanent arrays
    for (int i = 0; i < g->num_nodes; ++i)
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
    }
    // Distance to origin = 0
    distance[origin] = 0;

    for (int count = 0; count < g->num_nodes; ++count)
    {
        // Search for node with min distance among the current unvisited nodes
        int u = min_distance(distance, permanent, g->num_nodes);
        permanent[u] = 1; // Becomes permanent

        printf("%c ", int2char(u));
        // update distance for neighbour nodes if a shorter path is then found
        for (int v = 0; v < g->num_nodes; ++v)
        {
            //preventing overflow
            if (!permanent[v] && g->adjacency_matrix[u][v] != INT_MAX &&
                distance[u] != INT_MAX &&
                distance[u] + g->adjacency_matrix[u][v] < distance[v])
            {
                distance[v] = distance[u] + g->adjacency_matrix[u][v];
            }
        }
    }

    printf("\n");

    // Print the shortest paths
    for (int i = 0; i < g->num_nodes; ++i)
    {
        printf("The length of the shortest path between %c and %c is %d\n", int2char(origin), int2char(i), distance[i]);
    }

    // Free arrays
    free(distance);
    free(permanent);
}

// Function to delete the graph and all its data structures
void delete_graph(Graph *g)
{
    for (int i = 0; i < g->num_nodes; ++i)
    {
        free(g->adjacency_matrix[i]);
    }
    free(g->adjacency_matrix);
    free(g);
}
