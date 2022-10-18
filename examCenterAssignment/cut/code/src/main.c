#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include"header.h"  //header file for structure of candidate list and exam_center
#include<pthread.h>
pthread_mutex_t lock1;
pthread_mutex_t lock2;

/************************threadfor candidate_data************************/
void* func(void* arg){
	start=NULL;
	char buffer[256];
	char* a=(char *)arg;
	FILE *f=fopen(a,"r");
	if(f<=0){
		printf("error in openeing file\n");
		return NULL;}
while(	fgets(buffer , 256, f) != NULL){

//checking valid entry in the list

int len=strlen(buffer), q=0;
for(int i=0;i<len-1;i++){
	char c=buffer[i];
	if((c>=97&&c<=122)||(c>=65&&c<=90)||(c>='0'&&c<='9')||c==','||c=='/'||c==' '||c=='-');
	else
	{
		q++;}
}
if (q!=0){
/* if invalid list is found*/
	pthread_mutex_lock(&lock1);
	int f2= open("invalidCandidates_list.txt",O_RDWR |O_CREAT |O_APPEND,0666);
	printf("invalid entry\n");
	write(f2,buffer,strlen(buffer));
	 pthread_mutex_unlock(&lock1);
	continue;
}
/******************************************************************/

/*********getting the data in the structure ***************************/

buffer[strcspn(buffer,"\n")]=0;
char *tok=strtok(buffer ,",");
data *tem;
tem=(data *)malloc (sizeof(data));
strcpy(tem-> candidate_id,tok);
tok=strtok(NULL,",");
strcpy(tem->name,tok);
tok=strtok(NULL,",");
strcpy(tem->exam_id,tok);
tok=strtok(NULL,",");
strcpy(tem->start_date,tok);
tok=strtok(NULL,",");
strcpy(tem->end_date,tok);
exam_center *p;

//pointing to respective exam_center

p=head;
while(p !=NULL)
{
	if(strcmp(p->exam_id , tem->exam_id)==0){
		tem->center =p;
		break;
	}
	else{
		p=p->next;}
}
if(start == NULL){
	start = tem;
	tem->nxt=NULL;
}
else
{
	tem->nxt=start;
	start=tem;
}

/**********************************************/
/************writing the data to respective <exam_center_list.txt files***********/

pthread_mutex_lock(&lock2);
char path1[10],path2[10];
strcpy(path1,tem->center->exam_center_no);
strcpy(path2,"_list.txt");
strcat(path1,path2);
char buff[50];
sprintf(buff , "%s,%s,%s,%s\n",tem->candidate_id,tem->name,tem->start_date,tem->end_date);
int f1=	open(path1,O_RDWR | O_CREAT|O_APPEND, 0666);
write(f1, buff , strlen(buff));
close(f1);
pthread_mutex_unlock(&lock2);
}
return NULL;
}
/******************************************************************************************/

/***********************thread for exam_center********************************************/
void *ec_func(){
head=NULL;
char buffer[256];
FILE *f=fopen("../data/exam_center.txt","r");
while(	fgets(buffer , 256, f) != NULL){
	buffer[strcspn(buffer,"\n")]=0;
	char *tok=strtok(buffer ,":");
	exam_center *temp;
	temp=(exam_center *)malloc (sizeof(exam_center));
	strcpy(temp->exam_center_no,tok);
	tok=strtok(NULL,":");
	strcpy(temp->exam_id,tok);
	tok=strtok(NULL,":");
	strcpy(temp->college,tok);
	tok=strtok(NULL,":");
	strcpy(temp-> addr,tok);
	if(head == NULL){
		head = temp;
		temp->next=NULL;}
	else{
		temp->next=head;
		head=temp;
	}
}
return NULL;
}
/*******************************************************end*****************************************/


/****************************************main******************************************************/
int main(int argc, char* argv[]){

	if(pthread_mutex_init(&lock1,NULL)!=0){
			printf("mutex init has failed\n");
			
			}
	if(pthread_mutex_init(&lock2,NULL)!=0){
			printf("mutex init has failed\n");
		}
	pthread_t ec_t;
	pthread_create(& ec_t,NULL,&ec_func,(void *)NULL);
	pthread_join(ec_t,NULL);
pthread_t ptid;
for(int i=0;i<argc-1;i++){
	char *a=argv[i+1];
	pthread_create(&ptid, NULL, &func, (void *)a);
}
for(int i=0;i<argc-1;i++){
	pthread_join(ptid,NULL);
}
sleep(1);


/*************************************************user_input*********************************/

while(1){
	printf("enter\n");
	char search[6];
	scanf("%s",search);
	if(strcmp(search,"0")==0){
		return EXIT_SUCCESS;}
	else{
		data* check;
		check=start;
		int num=0;
		while(check!=NULL){
			if(strcmp(check->candidate_id,search)==0)
			{
				printf("Candidate_id : %s\nExam_id : %s\nCandidate_name : %s\nName of college : %s\naddress :%s\nstart_date: %s\nend_date:%s\n",check->candidate_id,check->exam_id,check->name,check->center->college,check->center->addr,check->start_date,check->end_date);
				num++;
				break;
			}
			else{
				check=check->nxt;
			}
			if(check==NULL&&num==0)
			{
				printf("invalid id please enter the valid id or enter 0 to exit\n");
			}
		}
	}
}


exam_center *p;
while(head!=NULL){
	p=head;
	free(head);
	head=p->next;
}
return EXIT_SUCCESS;
}
