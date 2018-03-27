#include<stdlib.h>
#include<stdio.h>

typedef struct {
    unsigned int a;
    unsigned int b;
    unsigned int dist;
} obj;

static int check(const obj *x, const obj *y)
{
    return x->dist - y->dist;
}

static unsigned int cr(const obj *edges, unsigned int n,
        unsigned int *visited, unsigned int order, unsigned int vertex,
        unsigned int predecessor)
{
    unsigned int i;
    unsigned int cycle_found = 0;
    visited[vertex] = 1;
    for (i = 0; i < n && !cycle_found; i++) {
        if (edges[i].a == vertex || edges[i].b == vertex) {
            /* SIDE-BY-SIDE-EDGES */
            const unsigned int neighbour = edges[i].a == vertex ?
                    edges[i].b : edges[i].a;
            if (visited[neighbour] == 0) {
                /* Not visited */
                cycle_found = cr(edges, n, visited, order, neighbour, vertex);
            }
            else if (neighbour != predecessor) {
                /* Found a cycle */
                cycle_found = 1;
            }
        }
    }
    return cycle_found;
}

unsigned int cyclic(const obj *edges, unsigned int n, unsigned int order)
{
    unsigned int *visited = calloc(order, sizeof(unsigned int));
    unsigned int cycle_found;
    if (visited == NULL) {
        return 0;
    }
    cycle_found  = cr(edges, n, visited, order, 0, 0);
    free(visited);
    return cycle_found;
}

obj *my_algorithm(obj *edges, unsigned int size, unsigned int order)
{
    unsigned int t, e = 0;
    obj *path = malloc(order * sizeof(obj));
    unsigned int *degrees = calloc(order, sizeof(unsigned int));
    if (path == NULL || degrees == NULL) {
        free(path);
        free(degrees);
        return NULL;
    }
    /* Sort the edges by distance */
    qsort(edges, size, sizeof(obj),
            (int(*)(const void *, const void *))check);
    /* Main algorithm */
    for (t = 0; t < order; t++) {
        unsigned int added = 0;
        while (!added && e < size) {
            if (degrees[edges[e].a] < 2 && degrees[edges[e].b] < 2) {
                path[t] = edges[e];
                if (t == order - 1 /* It's the last edge */
                    || !cyclic(path, t + 1, order)) /* It doesn't close the circuit */
                {
                    added = 1;
                    degrees[edges[e].a]++;
                    degrees[edges[e].b]++;
                }
            }
            e++;
        }
        if (!added) {
            /* Edges were not correct */
            free(path);
            free(degrees);
            return NULL;
        }
    }
    free(degrees);
    return path;
}


/* Connect two edges */
void join(obj *edges, unsigned int a, unsigned int b,
        unsigned int dist, unsigned int *pos)
{
    edges[*pos].a = a;
    edges[*pos].b = b;
    edges[*pos].dist = dist;
    (*pos)++;
}

static void print_edges(const obj *edges, unsigned int n)
{
    unsigned int e;
    for (e = 0; e < n; e++) {
        printf("(%u, %u, %u) ", edges[e].a, edges[e].b, edges[e].dist);
    }
    putchar('\n');
}

int main(void)
{
    unsigned int i = 0,i1,j,k,sum=0;
    int size; /* Edges */
    int order; /* Vertices */
    printf("Enter the number of cities :");
    scanf("%d",&order);
    printf("\n\n\n");
    if(order>2){
    for(k=0;k<order;k++){
        sum = sum + k;
    }
    size = sum;

    obj *edges = malloc(size * sizeof(obj));
    obj *path;
    int arr[order][order];

    for(i1=0;i1<order-1;i1++){
        for(j=i1+1;j<order;j++){
            printf("Enter %d %d th city's distance :",i1,j);
            scanf("%d",&arr[i1][j]);
        }
    }
    printf("\n\n\nThe path that should be formed is as below :\n\n");

    for(i1=0;i1<order-1;i1++){
        for(j=i1+1;j<order;j++){
            join(edges, i1, j, arr[i1][j], &i);
        }
    }

    path = my_algorithm(edges, size, order);
    print_edges(path, order);

    free(path);
    free(edges);
    printf("\nNOTE: The above representation is: \(city 1,city 2, distance between them\)\n\n\n");
    }
    else{
        printf("\nOnly for 3 or more cities !!!\n");
    }
    return 0;
}

