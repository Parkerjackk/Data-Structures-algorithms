#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "queue.h"

Graph *create_graph(int num_nodes)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));

    graph->num_nodes = num_nodes;
    graph->adjacency_list = (Node **)malloc(num_nodes * sizeof(Node *));
    for (int i = 0; i < num_nodes; i++)
    {
        graph->adjacency_list[i] = NULL;
    }
    return graph;
}

void add_edge(Graph *g, int from, int to)
{
    if (from < 0 || from >= g->num_nodes || to < 0 || to >= g->num_nodes)
    {
        return;
    }
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = to;

    Node **current = &(g->adjacency_list[from]);

    while (*current != NULL && (*current)->data < to)
    {
        current = &((*current)->next);
    }

    newNode->next = *current;
    *current = newNode;
}

void bfs(Graph *g, int origin)
{
    bool *visited = (bool *)malloc(g->num_nodes * sizeof(bool));
    printf("\nBFS ");

    for (int i = 0; i < g->num_nodes; i++)
    {
        visited[i] = false;
    }
    Queue *queue = create_queue();

    // Enqueue the origin node and mark it as visited
    enqueue(queue, origin);
    visited[origin] = true;

    while (!isEmpty(queue))
    {
        // Dequeue a node
        int current = dequeue(queue);
        printf(" %c ", 'A' + current);
        // Enqueue adjacent nodes in alphabetical order

        Node *neighbor = g->adjacency_list[current];
        while (neighbor != NULL)
        {
            if (!visited[neighbor->data])
            {
                enqueue(queue, neighbor->data);
                visited[neighbor->data] = true;
            }
            neighbor = neighbor->next;
        }
    }

    free(visited);
    delete_queue(queue);
}

void dfs_util(Graph *g, int vert, bool *visited)
{

    visited[vert] = true; // marking curr node as visited
    printf(" %c ", 'A' + vert);

    Node *neighbor = g->adjacency_list[vert];
    while (neighbor != NULL)
    {
        if (!visited[neighbor->data])
        {
            dfs_util(g, neighbor->data, visited);
        }
        neighbor = neighbor->next;
    }
}

void dfs(Graph *g, int origin)
{
    printf("DFS:");
    bool *visited = (bool *)malloc(g->num_nodes * sizeof(bool));
    for (int i = 0; i < g->num_nodes; i++)
    {
        visited[i] = false;
    }
    dfs_util(g, origin, visited);
    free(visited);
}

void delete_graph(Graph *g)
{
    for (int i = 0; i < g->num_nodes; i++)
    {
        Node *curr = g->adjacency_list[i];
        while (curr != NULL)
        {
            Node *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(g->adjacency_list);
    free(g);
}
