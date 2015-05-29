#include <stdio.h>
#include <stdlib.h>

typedef struct graph *Graph;

typedef struct node *link; /*Lista de adjacencias*/

struct node {
	int v;
    int F;
	link next;
};

int len_path=0;

int *color;

link pilha=NULL;         /*pilha de vertices*/
link pointer_fim;
struct graph {
	int V;               /*numero maximo de vertices no grafo*/
    link *list_adj;
};

void Push(int valor){
    link novo = malloc(sizeof(struct node));
    novo->v=valor;
    novo->next=NULL;
    if(pilha==NULL){
        pilha=novo;
        pointer_fim=novo;
    }
    else{
        pointer_fim->next=novo;
        pointer_fim=novo;
    }
}

int Pop(){
    int val=pilha->v;
    link pointer;
    pointer=pilha;
    pilha=pilha->next;
    free(pointer);
    return val;
}

link NEW( int v, link next){
	link x = (link) malloc(sizeof(struct node));
	x->v = v;           /*passar o valor do vertice seguinte para o arco*/
    x->F = 0;
	x ->next = next;
	return x;
}

Graph GraphInit(int V){
    int i;
    Graph G=(struct graph*)malloc(sizeof(struct graph));
    G->list_adj=malloc(sizeof(link)*V);
    G->V=V;
    color=malloc(sizeof(int)*V);
    for(i=0;i<V;i++){
        G->list_adj[i]=NULL;
        color[i]=-1;
    }
    pilha=NULL;
    return G;
}

void GRAPHinsertE (Graph G, int v1,int v2,int valor){
	G->list_adj[v1] = NEW(v2, G->list_adj[v1]);
    G->list_adj[v2] = NEW(v1, G->list_adj[v2]);
}

void read_edges(Graph G,int num_arcos){
    int v1,v2;
    for(;num_arcos>0;num_arcos--){
        scanf("%d %d",&v1,&v2);
        GRAPHinsertE(G,v1,v2,1);
    }
}

void read_input(int * vertex,int *edge){
    scanf("%d %d",vertex,edge);
}

int soma(Graph G,int inicio){
    int sum=0;
    link list=G->list_adj[inicio];
    for (;list!=NULL;list=list->next){
        sum += list->F;
    }
    return sum;
}

void insert_path(v){
    color[len_path]=v;
    len_path++;
}

void limpa_lista(Graph G){
    int i;
    
    link pointer ;
    for(i=0;i<G->V;i++){
        pointer=G->list_adj[i];
        while(pointer!=NULL){
            pointer->F=0;
            pointer=pointer->next;
        }
    }
}

int bfs(Graph G,int inicio,int fim,int num_vertices){
    int *P;
    int u;
    int v;
    link pointer;
    link *lista=G->list_adj;
    P=malloc(sizeof(int)*num_vertices);
    for(u=0;u<num_vertices;u++){
        P[u]=-1;
    }
    P[inicio]=inicio;
    Push(inicio);
    while(pilha!=NULL){
        u=Pop();
        pointer=lista[u];
        for(;pointer!=NULL;pointer=pointer->next){
            v=pointer->v;
            if((pointer->F < 1) && P[v]==-1){
                P[v]=u;
                Push(v);
                if(v==fim){
                    while(1){
                        insert_path(v);
                        if(v==inicio){
                            break;
                        }
                        v=P[v];
                    }
                    free(P);
                    return 0;
                }
            }
        }
    }
    free(P);
    return 1;
}

int min(int v1,int v2){
    if(v1>v2){
        return v2;
    }
    else{
        return v1;
    }
}

void reset(int *cena){
    int i;
    for(i=0;i<len_path;i++){
        cena[i]=-1;
    }
    len_path=0;
    pilha=NULL;
}

void libertar_pilha(){
    link pointer;
    pointer=pilha;
    while(pointer!=NULL){
        pilha=pointer->next;
        free(pointer);
        pointer=pilha;
    }
}

int get_F(Graph G,int u,int v){
    link lista=G->list_adj[u];
    while(lista!=NULL ){
        if(lista->v == v){
            return lista->F;
        }
        lista=lista->next;
    }
    return 0;
}

void put_F(Graph G,int u,int v,int flow){
    link lista=G->list_adj[u];
    while(lista!=NULL ){
        if(lista->v == v){
            lista->F+=flow;
        }
        lista=lista->next;
    }
}

int edmonds_karp(Graph G, int inicio,int fim){
    int len=G->V;
    int u;
    int v;
    int i;
    int flow;
    while(1>0){
        i=bfs(G,inicio,fim,len);
        libertar_pilha();
        if(i==1){
            break;
        }
        u=color[len_path-1];
        v=color[len_path-2];
        flow = 1-get_F(G,u,v);
        for(i=len_path-2;i>0;i--){
            u=color[i];
            v=color[i-1];
            flow=min(flow,1-get_F(G,u,v));
        }
        for(i=len_path-1;i>0;i--){
            u=color[i];
            v=color[i-1];
            put_F(G,u,v,flow);
            put_F(G,v,u,-flow);
        }
        len_path=0;
    }
    return soma(G,inicio);
}

int read_inteiro(){
    int val;
    scanf("%d",&val);
    return val;
}

int *read_conjuntos_criticos(){
    int *var;
    int i=1;
    int num_crit;
    scanf("%d",&num_crit);
    var=malloc(sizeof(int)*(num_crit+1));
    var[0]=num_crit;
    while(num_crit>0){
        scanf("%d",&var[i]);
        num_crit--;
        i++;
    }
    return var;
}

void destroy_Graph(Graph G){
    free(G);
}

void destroy_list_adj(link head){
    link tmp;
    while (head != NULL){
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void destroy_adj(Graph G){
    int i;
    for(i=0;i < G->V;i++){
        if(G->list_adj[i]!=NULL){
            destroy_list_adj(G->list_adj[i]);
        }
    }
    free(G->list_adj);
}

int get_output(int *array,Graph G){
    int i;
    int j;
    int output;
    int min=(G->V-1);
    for(i=1;i<=array[0] ;i++){
        for(j=(i+1);j<=array[0];j++){
            output=edmonds_karp(G,array[i],array[i+1]);
            limpa_lista(G);
            if(output<min){
                min=output;
            }
        }
    }
    return min;
}

void show_output(int num_crit,Graph G){
    int *input;
    int output=0;
    int crit=num_crit;
    while(crit>0){
        input=read_conjuntos_criticos();
        output=get_output(input,G);
        printf("%d\n",output);
        crit--;
        free(input);
    }
    free(color);
    destroy_adj(G);
    destroy_Graph(G);
}

int main(){
    int num_vertices=75000000;
    int i;
    Graph G;
    G=GraphInit(num_vertices);
    for(i=0;i<(num_vertices-1);i++){
        GRAPHinsertE(G,i,i+1,1);
    }
    printf("%d\n",edmonds_karp(G,0,(num_vertices-1)));
    return 0;
}
