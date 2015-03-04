#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int N;

struct Graph //structure of the graph
{
    int N; //number of nodes in graph
    struct Lists* nodes; //pointer to array of nodes
};

struct Lists
{
    struct Listnode *head;  // head node of the linked list of each array
};

struct Listnode //each node in the linked list
{
	int nodeid;
	struct Listnode* next;
};

struct Graph* initgraph(int N) //initializinga graph of N nodes
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    if(graph==NULL)
	{
		printf("Error allocating memory to initialize graph\n");
		exit(1);
	}

    graph->N = N;
     
    graph->nodes = (struct Lists*) malloc(N * sizeof(struct Lists));
    if(graph->nodes==NULL)
	{
		printf("Error allocating memory to initialize nodes in graph\n");
		exit(1);
	}

	int i;
    for (i = 0; i < N; i++)
    {
        graph->nodes[i].head = NULL;
    }
 
    return graph;
}


void addEdge(struct Graph* graph, int src, int dest) //adding edges in the graph
{
    struct Listnode* newnode = (struct Listnode*) malloc(sizeof(struct Listnode));
    if(newnode==NULL)
    {
    	printf("Failed creating new node to add to adjacency list; node: %d\n",dest);
    	exit(1);
    }
    newnode->nodeid = dest;
    newnode->next = graph->nodes[src].head;
    graph->nodes[src].head = newnode;
 
    struct Listnode* newnode2 = (struct Listnode*) malloc(sizeof(struct Listnode));
    if(newnode2==NULL)
    {
    	printf("Failed creating new node to add to adjacency list; node: %d\n",src);
    	exit(1);
    }
    newnode2->nodeid = src;
    newnode2->next = graph->nodes[dest].head;
    graph->nodes[dest].head = newnode2;
}

// used to check if graph was committed to memory correctly
/*
void checkgraph(struct Graph* graph) 
{
    int i;
    for (i = 0; i < graph->N; ++i)
    {
        struct Listnode* crawl = graph->nodes[i].head;
        printf("\n Adjacency list of vertex %d\n head ", i);
        while (crawl)
        {
            printf("-> %d", crawl->nodeid);
            crawl = crawl->next;
        }
        printf("\n");
    }
}
*/

int returnclosestnode(int *visitednodes, int *distance)//greedy algorithm closest node selection
{
	int i, toret;
	int temp = INT_MAX;
	for(i=0; i<N; i++)
	{
		if((visitednodes[i]==0) && (distance[i] <= temp))
		{
			temp = distance[i];
			toret = i;
		}
	}
	//printf("returning %d\n",toret ); //added step for debugging
	return toret;

}

int sumvisit(int *visitednodes) //checks the total number of visited nodes 
{
	int i;
	int sumvis = 0;

	for(i = 0; i<N; i++)
	{
		sumvis = sumvis + visitednodes[i];
	}
	//printf("sumvis is %d\n", sumvis ); //added for debugging
	return sumvis;
}

int maximum(int *distance) //returns furthest node distance in distance array
{
	int i;
	int max = 0;
	for(i=0; i<N; i++)
	{
		if(max<distance[i])
		{
			max = distance[i];
		}
	}
	return max;
}


int djikstra(struct Graph* graph, int node)
{
	int* visitednodes = (int*) malloc((N)*sizeof(int)); //nodes visited in the iteration of program
	int* distance = (int*) malloc((N)*sizeof(int)); //distance from source node
	if((visitednodes==NULL)||(distance==NULL))
	{
		printf("Error initializing memory for djikstra's algorithm\n");
		exit(1);
	}
	struct Listnode* crawler;
	int i,j,farthest;

	for(i=0; i< N; i++) //initializing distance to maximum and visited set to empty
	{
		visitednodes[i] = 0;
		distance[i] = INT_MAX;
	}

	distance[node] = 0; //initializing source node to zero

	//printf("initialized for %d node\n", node ); //added step for debugging
	while(sumvisit(visitednodes)!=(N-1))
	{
		//printf("step\n"); //added for debugging
		j = returnclosestnode(visitednodes,distance);

		visitednodes[j] = 1;
		crawler = graph->nodes[j].head;
		while(crawler)
		{
			if(distance[crawler->nodeid]>(distance[j]+1)) //Relaxation step
			{
				distance[crawler->nodeid] = (distance[j]+1); 
			}

			crawler = crawler->next;
		}
	}

	farthest = maximum(distance);

	free(visitednodes);
	free(distance);
	return farthest;
}


int main(int argc, char* argv[])
{
	if (argc != 2)
    {
        if (argc < 2)
        {
            printf("Missing Arguments to the program\n");
            exit(1);
        }
        else
        {
            printf("Additional command line Arguments found.\n");
            exit(1);    
        }
        
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) 
    {
        printf("Can't open file %s\n", argv[1]);
        exit(1);
    }

    int i, a, b;
    char str[50];
    fscanf(fp, "%d", &N);
    //printf("Total number of nodes is %d \n", N); //added for debugging
    struct Graph* graph = initgraph(N); //initalize graph of N nodes

    
   	for(i=0; i< (N+5); i++)
    {
    	fscanf(fp, "%d\t%d", &a, &b);
    	//printf("Adding edge between %d and %d \n", a, b); //added for debugging
    	addEdge(graph, a, b);
    };

    //checkgraph(graph); //added for debugging
    int diameter, temp;
    
    for (int i = 0; i < N; i++)
    {
    	temp = djikstra(graph, i);
    	//printf("max distance of %d node is %d\n",i, temp ); //added for debugging
    	if (temp>diameter)
    	{
    		diameter = temp;
    	}
    };
    printf("The diameter of the graph is %d\n", diameter);
    
}
