/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

typedef struct item {
    int id;				// id
    int left_stock;		// 잔여주식 
    int price;			// 주식단가 
    int readcnt;
    struct item* left;
    struct item* right;
    sem_t mutex;
} Item;

Item* root = NULL;
sem_t mutex;
int count = 0;

void *thread(void* vargp);
void echo(int connfd);
Item* Stock_insert(Item** itm, int id, int stock_cnt, int price); 
Item* Stock_search(Item* itm, int id);
void Stock_show(Item* itm, char* lst);

int main(int argc, char **argv) {
	
	FILE *fp = NULL;
	int id = 0, cnt = 0, price = 0;
	int listenfd, *connfdp;
    	socklen_t clientlen;
    	struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    	char client_hostname[MAXLINE], client_port[MAXLINE];
    	char buf[MAXLINE] = {'\0',};
    	pthread_t tid;
	Sem_init(&mutex,0,1);

    	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
    	}
	
	// file read & binary search tree save
	fp = Fopen("stock.txt", "r");
	while(Fgets(buf, MAXLINE, fp)){
		sscanf(buf, "%d %d %d\n", &id, &cnt, &price);;
		Stock_insert(&root,id,cnt,price);
		memset(buf,'\0',MAXLINE);
	}
	Fclose(fp);
	
	// 실행 
    	listenfd = Open_listenfd(argv[1]);
    	while (1) {
		clientlen = sizeof(struct sockaddr_storage); 
		connfdp = Malloc(sizeof(int));
        	*connfdp = Accept(listenfd, (SA*)&clientaddr, &clientlen);
        	Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        	printf("Connected to (%s, %s)\n", client_hostname, client_port);
		Pthread_create(&tid, NULL, thread, connfdp);
		P(&mutex);
		count++;
		V(&mutex);
    	}
 
    	exit(0);
}

void *thread(void* vargp){
	
    	int connfd = *((int*)vargp);
    	Pthread_detach(pthread_self());
    	Free(vargp);
    	echo(connfd);
    	Close(connfd);
	
	P(&mutex);
	count--;
	if(count == 0){
		FILE* fp = Fopen("stock.txt", "w");
        	char lst[MAXLINE] ={'\0',};
		Stock_show(root,lst);
		Fputs(lst,fp);
		Fclose(fp);
        }
	V(&mutex);
    	return NULL;
}

void echo(int connfd){

    	int n;
    	char buf[MAXLINE];
    	char first[MAXLINE];
	int second = 0, thrid = 0, cnt = 0;
    	rio_t rio;

    	Rio_readinitb(&rio, connfd);
    	while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		printf("%s\n",buf);
		//printf("server received %d bytes\n", n);
    		// parsing
        	cnt = sscanf(buf, "%s %d %d", first, &second, &thrid);
        
        	// command 
        	if(cnt == 3 && !strcmp(first, "buy")){
        		Item* tmp  = Stock_search(root,second);
			if(tmp->left_stock < thrid){
				Rio_writen(connfd,"Not enough left stocks\n",MAXLINE);	
			}
			else{	
				P(&(tmp->mutex));
				tmp->left_stock -= thrid;
				V(&(tmp->mutex));
				Rio_writen(connfd,"[buy] success\n",MAXLINE);
			} 
		}
        	else if(cnt == 3 && !strcmp(first, "sell")){
        		Item* tmp = Stock_search(root,second);
			P(&(tmp->mutex));
                        tmp->left_stock += thrid;
                        V(&(tmp->mutex));
                        Rio_writen(connfd,"[sell] success\n",MAXLINE);
		}
        	else if(cnt == 1 && !strcmp(first, "show")){
        		char lst[MAXLINE] = "";
			Stock_show(root,lst);
			Rio_writen(connfd,lst,MAXLINE);
		}
 	        else if(cnt == 1 && !strcmp(first, "exit")){
        		Rio_writen(connfd,"exit",MAXLINE);
			break;
		}
		else{
			Rio_writen(connfd,"command error\n",MAXLINE);
		}
        
        	// reset
		memset(buf,'\0',MAXLINE);
		memset(first,'\0',MAXLINE);
    	}	
}

Item* Stock_insert(Item** itm, int id, int stock_cnt, int price) {

    	if ((*itm) == NULL) {
        	(*itm) = (Item*)malloc(sizeof(Item));
        	(*itm)->left = (*itm)->right = NULL;
        	(*itm)->id = id;
        	(*itm)->left_stock = stock_cnt;
        	(*itm)->price = price;
        	(*itm)->readcnt = 0;
        	Sem_init(&((*itm)->mutex), 0, 1);
        	return (*itm);
    	}
    	else {
        	if ((*itm)->id > id)
            		(*itm)->left = Stock_insert(&((*itm)->left), id, stock_cnt, price);
        	else
            		(*itm)->right = Stock_insert(&((*itm)->right), id, stock_cnt, price);
    	}
    	return (*itm);
}
Item* Stock_search(Item* itm, int id){

    	if (itm == NULL)
        	return NULL;
        
    	if (itm->id == id)
        	return itm; 
    	else if (itm->id > id)
        	return Stock_search(itm->left, id);
   	else
        	return Stock_search(itm->right, id);
}

void Stock_show(Item* itm, char *lst){

    	char buf[MAXLINE] = {'\0',};

    	if (itm == NULL)
        	return;

	P(&(itm->mutex));
	sprintf(buf,"%d %d %d",itm->id,itm->left_stock,itm->price);
	strcat(lst,buf);
	strcat(lst,"\n");
	V(&(itm->mutex));
	
	Stock_show(itm->left, lst);
    	Stock_show(itm->right,lst);
}

