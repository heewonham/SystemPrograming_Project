#include "csapp.h"

typedef struct item {
    int id;				
    int left_stock;		
    int price;		
    int readcnt;
    struct item* left;
    struct item* right;
    sem_t mutex;
} Item;

Item* root = NULL;
sem_t mutex;
int count = 0;

void echo(char* command, char* ans);
Item* Stock_insert(Item** itm, int id, int stock_cnt, int price); 
Item* Stock_search(Item* itm, int id);
void Stock_show(Item* itm, char* lst);

int main(int argc, char **argv) {
	
	FILE *fp = NULL;
	int id = 0, cnt = 0, price = 0;
	int sockfd, newfd; 
	struct addrinfo hints, *serfo; 
	struct sockaddr_storage clientaddr;
	char client_hostname[MAXLINE], client_port[MAXLINE];
	socklen_t clientlen;
	char buf[MAXLINE] = {'\0',};
 	int fdmax, n;
 	fd_set master, read_fds; 
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
	
	
	memset(&hints, 0, sizeof hints); 
 	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM; 
 	hints.ai_flags = AI_PASSIVE; // use my IP 
	Getaddrinfo(NULL, argv[1], &hints, &serfo);
	sockfd = Socket(serfo->ai_family, serfo->ai_socktype, serfo->ai_protocol);
	Bind(sockfd, serfo->ai_addr, serfo->ai_addrlen);
	Freeaddrinfo(serfo); 
	Listen(sockfd, 10);

	// accept를 하기전에 수행
	FD_ZERO(&master); 
	FD_SET(sockfd, &master);
	fdmax = sockfd; 

	while(1) { 

		read_fds = master; 
		Select(fdmax+1, &read_fds, NULL, NULL, NULL);

		for(int i=0; i<=fdmax; i++) { 
			if(FD_ISSET(i, &read_fds)) { 

				if(i == sockfd) { 
					clientlen = sizeof clientaddr; 
 					newfd = Accept(sockfd, (SA*)&clientaddr, &clientlen);
					FD_SET(newfd, &master);
 					if(newfd > fdmax) {  fdmax = newfd; }
 					Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        				printf("Connected to (%s, %s)\n", client_hostname, client_port);
 					P(&mutex);
					count++;
					V(&mutex);
				}
				else { 
 					if((n = recv(i, buf, sizeof buf, 0)) <= 0) { 
						Close(i); 
 						FD_CLR(i, &master);
						P(&mutex);
						count--;
						V(&mutex); 
					}
					else{
						char ans[MAXLINE] = "";
						echo(buf,ans);
						Rio_writen(i,ans,MAXLINE);
					}

 				}
 			} 
		}
		// 모든 커넥션 종료시 파일저장
		if(count == 0){
			P(&mutex);
			FILE* fp = Fopen("stock.txt","w");
			char lst[MAXLINE] = {'\0',};
			Stock_show(root,lst);
			Fputs(lst,fp);
			Fclose(fp);
			V(&mutex);
		}	
		memset(buf,'\0',MAXLINE); 
 	}
	exit(0); 
}

void echo(char* command,char *ans){

    	char first[MAXLINE];
	int second = 0, thrid = 0, cnt = 0;
	char *warn = "Not enough left stocks\n";
	char *buy = "[buy] success\n";
	char *sell = "[sell] sucess\n";
	char *err = "command error\n";

	printf("server received %d bytes\n",(int)strlen(command));
    	// parsing
        cnt = sscanf(command, "%s %d %d", first, &second, &thrid);

        // command 
        if(cnt == 3 && !strcmp(first, "buy")){
		Item* tmp  = Stock_search(root,second);
		if(tmp->left_stock < thrid){	
			strcpy(ans,warn);
		}
		else{	
			P(&(tmp->mutex));
			tmp->left_stock -= thrid;
			V(&(tmp->mutex));
			strcpy(ans,buy);
		}
	}
	else if(cnt == 3 && !strcmp(first, "sell")){
        	Item* tmp = Stock_search(root,second);
		P(&(tmp->mutex));
            	tmp->left_stock += thrid;
            	V(&(tmp->mutex));
		strcpy(ans,sell);
	}
        else if(cnt == 1 && !strcmp(first, "show")){
        	char lst[MAXLINE] = "";
		Stock_show(root,lst);
		strcpy(ans,lst);	
	}
	else{
		strcpy(ans,err);
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

