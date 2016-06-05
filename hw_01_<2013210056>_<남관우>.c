#include <stdio.h>
#include <string.h>
//#include <malloc.h>
#include <stdlib.h>

#define WHITE 0
#define GRAY  1
#define BLACK 2

typedef struct Adj
{
    int n;
    struct Adj *next;
} Adj;

void Adj_init(Adj *self)
{
    self->n = 0;
    self->next = NULL;
}

typedef struct
{
    int color;
    int parent;
    char name[10];
    int n;
    Adj *first;
} Vertex;

void Vertex_init(Vertex *self)
{
    self->color = 0;
    self->parent = -1;
    strcpy(self->name, "(none)");
    self->n = 0;
    self->first = NULL;
}

void Vertex_add(Vertex *self, Vertex *v)
{
    Adj *a = (Adj *) malloc(sizeof(Adj));
    a->n = v->n;
    a->next = self->first;
    self->first = a;
}

typedef struct
{
    Vertex super;
    double d;
} BFSVertex;

void BFSVertex_init(BFSVertex *self)
{
    Vertex_init(&self->super);
    self->d = 1E10;
}

typedef struct
{
    Vertex super;
    int dfs_in, dfs_out;
} DFSVertex;

void DFSVertex_init(DFSVertex *self)
{
    Vertex_init(&self->super);
    self->dfs_in = 0;
    self->dfs_out = 0;
}

typedef struct
{
    int front;
    int rear;
    int sz;
    int *buf;
} Queue;

void Queue_init(Queue *self)
{
    self->front = 0;
    self->rear = 0;
    self->sz = 0;
    self->buf = NULL;
}

void Queue_create_queue(Queue *self, int sz)
{
    self->sz = sz;
    self->buf = (int *) malloc(sizeof(int) * sz);
}

void Queue_enqueue(Queue *self, int val)
{
    self->buf[self->rear] = val;
    self->rear = (self->rear + 1) % self->sz;
}

int Queue_dequeue(Queue *self)
{
    int res = self->buf[self->front];
    self->front = (self->front + 1) % self->sz;
    return res;
}

int Queue_is_empty(Queue *self)
{
    return self->front == self->rear;
}

void print_bfsvertex(BFSVertex *vertices, int n)
{
    printf("%-8s ", vertices[n].super.name);
    printf("%-8d ", vertices[n].super.color);
    printf("%-8d ", vertices[n].super.parent);
    printf("%-16.0f\t: ", vertices[n].d);
    for (Adj *p = vertices[n].super.first; p; p = p->next)
    {
        printf("%s ", vertices[p->n].super.name);
    }
    printf("\n");
}

void print_dfsvertex(DFSVertex *vertices, int n)
{
    printf("%-8s ", vertices[n].super.name);
    printf("%-8d ", vertices[n].super.color);
    printf("%-8d ", vertices[n].super.parent);
    printf("%-8d ", vertices[n].dfs_in);
    printf("%-8d", vertices[n].dfs_out);

    printf("   \t: ");

    for (Adj *p = vertices[n].super.first; p; p = p->next)
    {
        printf("%s ", vertices[p->n].super.name);
    }

    printf("\n");
}

void bfs(BFSVertex *vertices, int nelem, int s)
{
    Queue q;
    //Start from S
    for (int u = 0; u < nelem; u++)
    {
        if (vertices[u].super.n != s)
        {
            vertices[u].super.color = WHITE;
            vertices[u].d = 1E10;
            vertices[u].super.parent = -1;
        }
    }
    // Except for s, all init -1, White;


    vertices[s].super.color = GRAY;
    vertices[s].d = 0;
    vertices[s].super.parent = -1;

    Queue_init(&q);
    Queue_create_queue(&q, nelem);
    Queue_enqueue(&q, s);

    while (!Queue_is_empty(&q))
    {
        int u = Queue_dequeue(&q);
        for (Adj *adj_v = vertices[u].super.first; adj_v; adj_v = adj_v->next)
        {
            if (vertices[adj_v->n].super.color == WHITE)
            {
                vertices[adj_v->n].super.color = GRAY;
                vertices[adj_v->n].d = vertices[u].d + 1;
                vertices[adj_v->n].super.parent = u;
                Queue_enqueue(&q, adj_v->n);
            }
        }
        vertices[u].super.color = BLACK;
    }

    // (s), (w r), (x t v), (y u)
}

int dfs_time = 0;


void dfs_visit(DFSVertex *vertices, int nelem, int u)
{
    dfs_time++;
    vertices[u].dfs_in = dfs_time;
    vertices[u].super.color = GRAY;
    for (Adj *v = vertices[u].super.first; v; v = v->next)
    {
        if (vertices[v->n].super.color == WHITE)
        {
            vertices[v->n].super.parent = u;
            dfs_visit(vertices, nelem, v->n);
        }
    }
    vertices[u].super.color = BLACK;
    dfs_time++;
    vertices[u].dfs_out = dfs_time;
}

void dfs(DFSVertex *vertices, int nelem)
{
    for (int u = 0; u < nelem; u++)
    {
        vertices[u].super.color = WHITE;
        vertices[u].super.parent = -1;
    }
    dfs_time = 0;
    for (int u = 0; u < nelem; u++)
    {
        if (vertices[u].super.n == WHITE)
        {
            dfs_visit(vertices, nelem, u);
        }
    }
}

void Tran_Vertex_add(Vertex *self, Vertex *v)
{
    Adj *a = (Adj *) malloc(sizeof(Adj));
    Adj *tmp = self->first;
    a->n = v->n;

    if (self->first == NULL)
    {
        a->next = self->first;
        self->first = a;
        return;
    }

    // Insert to tail
    while (tmp->next != NULL)
        tmp = tmp->next;

    a->next = NULL;
    tmp->next = a;
}


#define NELEM 8

int testmain()
{
    BFSVertex vertices[NELEM];
    for (int i = 0; i < NELEM; i++)
    {
        BFSVertex_init(&vertices[i]);
    }
    strcpy(vertices[0].super.name, "r");
    strcpy(vertices[1].super.name, "s");
    strcpy(vertices[2].super.name, "t");
    strcpy(vertices[3].super.name, "u");
    strcpy(vertices[4].super.name, "v");
    strcpy(vertices[5].super.name, "w");
    strcpy(vertices[6].super.name, "x");
    strcpy(vertices[7].super.name, "y");

    for (int i = 0; i < NELEM; i++)
    {
        vertices[i].super.n = i;
    }
    BFSVertex *r = vertices + 0;
    BFSVertex *s = vertices + 1;
    BFSVertex *t = vertices + 2;
    BFSVertex *u = vertices + 3;
    BFSVertex *v = vertices + 4;
    BFSVertex *w = vertices + 5;
    BFSVertex *x = vertices + 6;
    BFSVertex *y = vertices + 7;

    Vertex_add(&r->super, &s->super);
    Vertex_add(&r->super, &v->super);

    Vertex_add(&s->super, &r->super);
    Vertex_add(&s->super, &w->super);

    Vertex_add(&t->super, &w->super);
    Vertex_add(&t->super, &x->super);
    Vertex_add(&t->super, &u->super);

    Vertex_add(&u->super, &t->super);
    Vertex_add(&u->super, &x->super);
    Vertex_add(&u->super, &y->super);

    Vertex_add(&v->super, &r->super);

    Vertex_add(&w->super, &s->super);
    Vertex_add(&w->super, &t->super);
    Vertex_add(&w->super, &x->super);

    Vertex_add(&x->super, &w->super);
    Vertex_add(&x->super, &t->super);
    Vertex_add(&x->super, &u->super);
    Vertex_add(&x->super, &y->super);

    Vertex_add(&y->super, &x->super);
    Vertex_add(&y->super, &u->super);

    printf("%30s\n", "<BFS>");
    printf("%s\n", "=>Before transpose");
    printf("%-8s%-8s%-11s%-17s%-5s\n", "name", "color", "parent", "Bfs_Order", "child");
    for (int i = 0; i < NELEM; i++)
    {
        print_bfsvertex(vertices, i);
    }
    printf("\n\n");
    bfs(vertices, NELEM, s->super.n);

    printf("%-8s%-8s%-11s%-17s%-5s\n", "name", "color", "parent", "Bfs_Order", "child");
    for (int i = 0; i < NELEM; i++)
    {
        print_bfsvertex(vertices, i);
    }


    printf("\n\n%s\n", "=>After transpose");
    int i;
    BFSVertex newVertex[NELEM];

    for (i = 0; i < NELEM; i++)
        BFSVertex_init(&newVertex[i]);

    strcpy(newVertex[0].super.name, "r");
    strcpy(newVertex[1].super.name, "s");
    strcpy(newVertex[2].super.name, "t");
    strcpy(newVertex[3].super.name, "u");
    strcpy(newVertex[4].super.name, "v");
    strcpy(newVertex[5].super.name, "w");
    strcpy(newVertex[6].super.name, "x");
    strcpy(newVertex[7].super.name, "y");

    for (i = 0; i < NELEM; i++)
        newVertex[i].super.n = i;


    for (i = 0; i < NELEM; i++)
    {
        int parentNode = vertices[i].super.n;

        for (Adj *p = vertices[i].super.first; p; p = p->next)
        {
            int temp = p->n;
            Tran_Vertex_add(&newVertex[temp].super, &vertices[parentNode].super);
        }
    }

    printf("%-8s%-8s%-11s%-17s%-5s\n", "name", "color", "parent", "Bfs_Order", "child");
    for (int i = 0; i < NELEM; i++)
    {
        print_bfsvertex(newVertex, i);
    }
    printf("\n\n");
    bfs(newVertex, NELEM, s->super.n);

    printf("%-8s%-8s%-11s%-17s%-5s\n", "name", "color", "parent", "Bfs_Order", "child");
    for (int i = 0; i < NELEM; i++)
    {
        print_bfsvertex(newVertex, i);
    }

    return 0;
}

int main()
{
    DFSVertex vertices[NELEM];
    for (int i = 0; i < NELEM; i++)
    {
        DFSVertex_init(&vertices[i]);
    }

    strcpy(vertices[0].super.name, "r");
    strcpy(vertices[1].super.name, "s");
    strcpy(vertices[2].super.name, "t");
    strcpy(vertices[3].super.name, "u");
    strcpy(vertices[4].super.name, "v");
    strcpy(vertices[5].super.name, "w");
    strcpy(vertices[6].super.name, "x");
    strcpy(vertices[7].super.name, "y");

    for (int i = 0; i < NELEM; i++)
    {
        vertices[i].super.n = i;
    }

    DFSVertex *r = vertices + 0;
    DFSVertex *s = vertices + 1;
    DFSVertex *t = vertices + 2;
    DFSVertex *u = vertices + 3;
    DFSVertex *v = vertices + 4;
    DFSVertex *w = vertices + 5;
    DFSVertex *x = vertices + 6;
    DFSVertex *y = vertices + 7;

    Vertex_add(&r->super, &s->super);
    Vertex_add(&r->super, &v->super);

    Vertex_add(&s->super, &r->super);
    Vertex_add(&s->super, &w->super);

    Vertex_add(&t->super, &w->super);
    Vertex_add(&t->super, &x->super);
    Vertex_add(&t->super, &u->super);

    Vertex_add(&u->super, &t->super);
    Vertex_add(&u->super, &x->super);
    Vertex_add(&u->super, &y->super);

    Vertex_add(&v->super, &r->super);

    Vertex_add(&w->super, &s->super);
    Vertex_add(&w->super, &t->super);
    Vertex_add(&w->super, &x->super);

    Vertex_add(&x->super, &w->super);
    Vertex_add(&x->super, &t->super);
    Vertex_add(&x->super, &u->super);
    Vertex_add(&x->super, &y->super);

    Vertex_add(&y->super, &x->super);
    Vertex_add(&y->super, &u->super);


    // Before transpose DFS
    printf("%30s\n", "<DFS>");
    printf("%s\n", "=>Before transpose");
    printf("%-8s%-8s%-11s%-9s%-12s%-5s\n", "name", "color", "parent", "dfs_in", "dfs_out", "child");
    for (int i = 0; i < NELEM; i++)
        print_dfsvertex(vertices, i);

    dfs(vertices, NELEM);
    printf("\n");
    printf("%-8s%-8s%-11s%-9s%-12s%-5s\n", "name", "color", "parent", "dfs_in", "dfs_out", "child");

    for (int i = 0; i < NELEM; i++)
        print_dfsvertex(vertices, i);


    printf("\n\n");
    printf("%s\n", "=>After transpose");

    int i;
    DFSVertex newVertex[NELEM];

    for (i = 0; i < NELEM; i++)
        DFSVertex_init(&newVertex[i]);

    strcpy(newVertex[0].super.name, "r");
    strcpy(newVertex[1].super.name, "s");
    strcpy(newVertex[2].super.name, "t");
    strcpy(newVertex[3].super.name, "u");
    strcpy(newVertex[4].super.name, "v");
    strcpy(newVertex[5].super.name, "w");
    strcpy(newVertex[6].super.name, "x");
    strcpy(newVertex[7].super.name, "y");

    for (i = 0; i < NELEM; i++)
        newVertex[i].super.n = i;


    for (i = 0; i < NELEM; i++)
    {
        int parentNode = vertices[i].super.n;

        for (Adj *p = vertices[i].super.first; p; p = p->next)
        {
            int temp = p->n;
            Tran_Vertex_add(&newVertex[temp].super, &vertices[parentNode].super);
        }
    }

    printf("%-8s%-8s%-11s%-9s%-12s%-5s\n", "name", "color", "parent", "dfs_in", "dfs_out", "child");
    for (int i = 0; i < NELEM; i++)
        print_dfsvertex(newVertex, i);

    dfs(newVertex, NELEM);
    printf("\n");
    printf("%-8s%-8s%-11s%-9s%-12s%-5s\n", "name", "color", "parent", "dfs_in", "dfs_out", "child");

    for (int i = 0; i < NELEM; i++)
        print_dfsvertex(newVertex, i);


    printf("\n\n");

    testmain();


    return 0;
}