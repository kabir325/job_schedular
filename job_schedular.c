#include<stdio.h>
#include<stdlib.h>
#include<time.h>

struct job{
    char *work;
    char *description;
    char *status;
    int time_s;
    int time_m;
    int importance;
    struct job *next;
    struct job *prev;
};

struct queue {
    struct job *front;
    struct job *last;
};

void run(struct queue **Q){
    struct job*temp=(*Q)->last;
    int count=1,next_condition=0,prev_condition=0;
    int totalMinutes,totalSeconds;
    while(temp!=NULL){
        next_condition=0;
        prev_condition=0;
        printf("job no->>>%d\n",count);
        printf("job->%s\n",temp->work);
        time_t startTime, currentTime;
        time(&startTime);
        totalMinutes=temp->time_m;
        totalSeconds=temp->time_s;
        int elapsedTime = 0;
        int remainingTime = totalMinutes * 60+totalSeconds;
        printf("(Press 'x' to exit)\n(Press 'n' to run the next task)\n(Press 'p' to run the previous task)\n");
        while (remainingTime > 0) {
            time(&currentTime);
            elapsedTime = difftime(currentTime, startTime);
            remainingTime = (totalMinutes * 60+totalSeconds )- elapsedTime;

            int minutes = remainingTime / 60;
            int seconds = remainingTime % 60;

            printf("Time remaining: %02d:%02d \r", minutes, seconds);
            fflush(stdout);

            if (_kbhit()) {
                char ch = getchar();
                if (ch == 'x' || ch == 'X') {
                    temp->time_m = remainingTime / 60;
                    temp->time_s= remainingTime % 60;
                    return ;
                }
                else if(ch=='n'||ch=='N'){
                    temp->time_m = remainingTime / 60;
                    temp->time_s= remainingTime % 60;
                    next_condition=1;
                    break;
                }
                else if (ch=='p'||ch=='P'){
                    temp->time_m = remainingTime / 60;
                    temp->time_s= remainingTime % 60;
                    prev_condition=1;
                    break;
                }
            }

            sleep(1);
        }
        if(next_condition==1){
            printf("\nTASK SKIPPED\n");
            printf("\nEXECUTING NEXT TASK\n");
            if(temp->prev==NULL){
                printf("no task lined up after\n");
                return ;
            }
            temp=temp->prev;
        }
        else if(prev_condition==1){
            printf("\nTASK SKIPPED\n");
            printf("\nEXECUTING PREVIOUS TASK\n");
            if(temp->next==NULL){
                printf("no task lined up before\n");
                return ;
            }
            temp=temp->next;
            continue;
        }
        else{
            printf("\nTASK COMPLETED!\n");
            strcpy(temp->status,"complete");
            temp->time_m=0;
            temp->time_s=0;
            temp=temp->prev;
        }
        count++;
    }
}

struct queue *createQueue(){
    struct queue *newQueue=(struct queue*)malloc(sizeof(struct queue));
    newQueue->front=newQueue->last=NULL;
    return newQueue;
};

struct job *CreateJob(char work[],char description[],int priority,int time_m,int time_s){
    struct job *newJob=(struct job*)malloc(sizeof(struct job));
    if(!newJob){
        printf("error creating job\n");
        return NULL;
    }
    newJob->work=(char*)malloc(sizeof(char)*20);
    strcpy(newJob->work,work);
    newJob->description=(char*)malloc(sizeof(char)*100);
    strcpy(newJob->description,description);
    newJob->status=(char*)malloc(sizeof(char)*15);
    if(time_m!=0||time_s!=0)
        strcpy(newJob->status,"incomplete");
    else
        strcpy(newJob->status,"complete");
    newJob->importance=priority;
    newJob->time_s=time_s;
    newJob->time_m=time_m;
    newJob->next=NULL;
    newJob->prev=NULL;
    return newJob;
}

int addJob(struct queue **Q, char work[], char description[], int time_m,int time_s, int priority){
    struct job*temp=(*Q)->front;
    while(temp!=NULL){
        if(strcmp(temp->work,work)==0)
            break;
        temp=temp->next;
    }
    if(temp!=NULL){
        printf("job already exists\n");
        return 0;
    }
    struct job *newJob = CreateJob(work, description, priority,time_m ,time_s);
    if (!newJob) {
        return 0;
    }
    if ((*Q)->front == NULL) {
        (*Q)->front = (*Q)->last = newJob;
    } else {
        if (priority > (*Q)->front->importance) {
            newJob->next = (*Q)->front;
            (*Q)->front->prev = newJob;
            (*Q)->front = newJob;
        } else {
            struct job *current = (*Q)->front;
            while (current->next != NULL && priority <= current->next->importance) {
                current = current->next;
            }
            newJob->next = current->next;
            newJob->prev = current;
            if (current->next != NULL) {
                current->next->prev = newJob;
            } else {
                (*Q)->last = newJob;
            }
            current->next = newJob;
        }
    }

    return 1;
}

int editJob(struct queue *Q,char work[]){
    struct job *temp=Q->front;
    while(temp!=NULL){
        if(strcmp(temp->work,work)==0)
            break;
        temp=temp->next;
    }
    if(temp==NULL){
        printf("job not found\n");
        return 0;
    }

    int time_m,time_s,ch;
    char exit_choice;
    char job_title[20],job_description[100];
    while(1){
        printf("Select what you want to edit\n");
        printf("press 1 to edit job name\n");
        printf("press 2 to edit job description\n");
        printf("press 3 to edit job time\n");
        printf("press 4 to edit job status\n");
        printf("press 5 to exit edit option\n");
        scanf("%d",&ch);
        printf("_____________________________________________________________________________________________________________\n");
        switch(ch){
            case 1:
                printf("OPTION 1 CHOOSEN\n");
                printf("enter the new job title\n");
                scanf(" %[^\n]s",job_title);
                strcpy(temp->work,job_title);
                printf("JOB TITLE CHANGED SUCCESSFULLY\n");
                printf("_____________________________________________________________________________________________________________\n");
                break;
            case 2:
                printf("OPTION 2 CHOOSEN\n");
                printf("enter the new job description\n");
                scanf(" %[^\n]s",job_description);
                strcpy(temp->description,job_description);
                printf("JOB DESCRIPTION CHANGED SUCCESSFULLY\n");
                printf("_____________________________________________________________________________________________________________\n");
                break;
            case 3:
                printf("OPTION 3 CHOOSEN\n");
                printf("enter the new job time in minutes\n");
                scanf(" %d",&time_m);
                printf("enter the new job time in seconds\n");
                scanf(" %d",&time_s);
                temp->time_m=time_m;
                temp->time_s=time_s;
                if(time_m==0&&time_s==0){
                    strcmp(temp->status,"complete");
                }
                printf("TIME CHANGED SUCCESFULLY\n");
                printf("_____________________________________________________________________________________________________________\n");
                break;
            case 4:
                printf("OPTION 4 CHOOSEN\n");
                if(strcmp(temp->status,"complete")==0){
                    printf("job was initially completed but changing status so,\n");
                    printf("enter the job time in minutes\n");
                    scanf(" %d",&time_m);
                    printf("enter the job time in seconds\n");
                    scanf(" %d",&time_s);
                    temp->time_m=time_m;
                    temp->time_s=time_s;
                    if(time_m==0&&time_s==0){
                        strcmp(temp->status,"complete");
                    }
                }
                else{
                    strcpy(temp->status,"complete");
                    temp->time_m=0;
                    temp->time_s=0;
                }
                printf("STATUS CHANGES SUCCESFULLY\n");
                printf("_____________________________________________________________________________________________________________\n");
                break;
            case 5:
                printf("OPTION 5 CHOOSEN\n");
                printf("Are you sure you want to exit?\n");
                printf("If yes, enter Y\n");
                scanf(" %c", &exit_choice);
                if(exit_choice == 'Y'||exit_choice=='y'){
                    return 1;
                }
                printf("_____________________________________________________________________________________________________________\n");
                break;
            default:
                printf("No such functionality available\n");
                printf("_____________________________________________________________________________________________________________\n");
        }
    }
    return 1;
}

int prioJob(struct queue **Q,char work[],int prio){
    struct job *temp=(*Q)->front;
    while(temp!=NULL){
        if(strcmp(temp->work,work)==0)
            break;
        temp=temp->next;
    }
    if(temp==NULL){
        printf("job not found\n");
        return 0;
    }
    if (temp->prev == NULL && temp->next == NULL) {
        (*Q)->front = (*Q)->last = NULL;
    } else if (temp->prev != NULL) {
        if (temp->next != NULL) {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        } else {
            (*Q)->last = temp->prev;
            temp->prev->next = NULL;
        }
    } else if (temp->next != NULL) {
        (*Q)->front = temp->next;
        temp->next->prev = NULL;
    }
    addJob(Q,temp->work,temp->description,temp->time_m,temp->time_s,prio);
    free(temp->description);
    free(temp->work);
    free(temp->status);
    free(temp);
    printf("job priority changed successfully\n");
    return 1;
}

int statusJob(struct queue *Q,char work[]){
    struct job *temp=Q->front;
    while(temp!=NULL){
        if(strcmp(temp->work,work)==0)
            break;
    temp=temp->next;
    }
    if(temp==NULL){
        printf("JOB NOT FOUND\n");
        return 0;
    }
    printf("JOB FOUND\n");
    printf("JOB STATUS->%s\n",temp->status);
    return 1;
}

void displayJobs(struct queue *Q){
    struct job *temp=Q->last;
    int count=1;
    while(temp!=NULL){
        printf("<<<%d>>>\n",count);
        printf("job->%s\n",temp->work);
        printf("description->%s\n",temp->description);
        printf("time->%d min %d sec\n",temp->time_m,temp->time_s);
        printf("importance->%d\n",temp->importance);
        printf("status->%s\n\n",temp->status);
        count++;
        temp=temp->prev;
    }
}

int deleteJob(struct queue **Q, char work[]) {
    if (*Q == NULL) {
        printf("job list is empty\n");
        return 0;
    }

    struct job* temp = (*Q)->front;

    while (temp != NULL) {
        if (strcmp(temp->work, work) == 0) {
            break;
        }
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("job not found\n");
        return 0;
    }

    char exit_choice;
    printf("Job found, if you want to proceed enter y\n");
    scanf(" %c", &exit_choice);

    if (exit_choice != 'Y' && exit_choice != 'y') {
        printf("Delete function aborted\n");
        return 0;
    }

    if (temp->prev == NULL && temp->next == NULL) {
        (*Q)->front = (*Q)->last = NULL;
    } else if (temp->prev != NULL) {
        if (temp->next != NULL) {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        } else {
            (*Q)->last = temp->prev;
            temp->prev->next = NULL;
        }
    } else if (temp->next != NULL) {
        (*Q)->front = temp->next;
        temp->next->prev = NULL;
    }

    printf("Job->%s\n", temp->work);
    printf("Description->%s\n\n", temp->description);

    free(temp->description);
    free(temp->work);
    free(temp->status);
    free(temp);

    return 1;
}

int deleteAll(struct queue **Q){
    char exit_choice;
    struct job*temp=(*Q)->front;
    while(temp!=NULL){
        printf("delete job->%s\n",temp->work);
        deleteJob(Q,temp->work);
        temp=temp->next;
    }
}

int main() {
    int ch,prio,i,t_s,t_m;
    char exit_choice;
    char job_title[20];
    char job_description[100];
    struct queue *Q = createQueue();

    while(1){
        printf("Enter choice\n");
        printf("press 0 to run tasks\n");
        printf("Press 1 to add job\n");
        printf("Press 2 to edit job details\n");
        printf("Press 3 to update a job priority\n");
        printf("Press 4 to check job status\n");
        printf("Press 5 to see job details\n");
        printf("Press 6 to cancel job\n");
        printf("press 7 to delete all jobs\n");
        printf("Press 8 to exit\n");
        scanf("%d", &ch);
        printf("**********************************************************************************************************\n");
        switch(ch){
            case 0:
                printf("OPTION 0 CHOOSEN\n");
                run(&Q);
                printf("**********************************************************************************************************\n");
                break;
            case 1:
                printf("OPTION 1 CHOOSEN\n");
                printf("Enter job title\n");
                scanf(" %[^\n]s", job_title);
                printf("Enter job description\n");
                scanf(" %[^\n]s", job_description);
                printf("enter job time minutes\n");
                scanf("%d",&t_m);
                printf("enter job time seconds\n");
                scanf("%d",&t_s);
                printf("Enter job priority\n");
                scanf("%d", &prio);
                i=addJob(&Q, job_title, job_description, t_m,t_s,prio);
                if(i==1)
                    printf("JOB CREATED SUCCESFULLY\n");
                else
                    printf("JOB CREATION FAILED\n");
                printf("**********************************************************************************************************\n");
                break;
            case 2:
                printf("OPTION 2 CHOOSEN\n");
                printf("Enter job title\n");
                scanf(" %[^\n]s", job_title);
                i=editJob(Q,job_title);
                printf("**********************************************************************************************************\n");
                break;
            case 3:
                printf("OPTION 3 CHOOSEN\n");
                printf("Enter job title\n");
                scanf(" %[^\n]s", job_title);
                printf("enter new job priority\n");
                scanf("%d",&prio);
                i=prioJob(&Q,job_title,prio);
                printf("**********************************************************************************************************\n");
                break;
            case 4:
                printf("OPTION 4 CHOOSEN\n");
                printf("enter job for which u wanna check status\n");
                scanf(" %[^\n]s",job_title);
                i=statusJob(Q,job_title);
                printf("**********************************************************************************************************\n");
                break;
            case 5:
                printf("OPTION 5 CHOOSEN\n");
                displayJobs(Q);
                printf("**********************************************************************************************************\n");
                break;
            case 6:
                printf("OPTION 6 CHOOSEN\n");
                printf("enter job to be deleted\n");
                scanf(" %[^\n]s", job_title);
                i=deleteJob(&Q,job_title);
                if(i==1)
                    printf("JOB DELETED SUCCESFULLY\n");
                else
                    printf("JOB COULDN'T BE DELETED\n");
                printf("**********************************************************************************************************\n");
                break;

            case 7:
                printf("OPTION 7 CHOOSEN\n");
                deleteAll(&Q);
                printf("**********************************************************************************************************\n");
                break;
            case 8:
                printf("OPTION 8 CHOOSEN\n");
                printf("Are you sure you want to exit?\n");
                printf("If yes, enter Y\n");
                scanf(" %c", &exit_choice);
                if(exit_choice == 'Y'||exit_choice=='y'){
                    exit(0);
                }
                printf("**********************************************************************************************************\n");
                break;
            default:
                printf("No such functionality available\n");
                printf("**********************************************************************************************************\n");
        }
    }
    return 0;
}
