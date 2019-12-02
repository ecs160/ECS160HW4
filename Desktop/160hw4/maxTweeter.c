#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#define N 1024
#define MAX 20000
typedef struct node node;
struct node {
  char *name;
  int number;
  struct node *next;
} ;

// Returns the last node of the list
struct node *getTail(struct node *curr) {
  while (curr != NULL && curr->next != NULL){
    curr = curr->next;
  }
  return curr;
}
// Partitions the list taking the last element as the pivot
struct node *partition(struct node *head, struct node *end,
                    struct node **newHead, struct node **newEnd){
  struct node *pivot = end;
  struct node *prev = NULL, *cur = head, *tail = pivot;

  while (cur != pivot) {
    if (cur->number > pivot->number) {
        // First node that has a value less than the pivot - becomes
        // the new head
        if ((*newHead) == NULL) (*newHead) = cur;
        prev = cur;
        cur = cur->next;
    } else {
      // Move cur node to next of tail, and change tail
      if (prev) prev->next = cur->next;
      struct node *tmp = cur->next;
      cur->next = NULL;
      tail->next = cur;
      tail = cur;
      cur = tmp;
    }
  }
  // If the pivot data is the smallest element in the current list,
  // pivot becomes the head
  if ((*newHead) == NULL)
      (*newHead) = pivot;

  // Update newEnd to the current last node
  (*newEnd) = tail;

  // Return the pivot node
  return pivot;
}
//here the sorting happens exclusive of the end node
struct node *quickSortRecur(struct node *head, struct node *end) {
    // base condition
    if (!head || head == end)  return head;
    node *newHead = NULL, *newEnd = NULL;
    // Partition the list, newHead and newEnd will be updated
    // by the partition function
    struct node *pivot = partition(head, end, &newHead, &newEnd);

    // If pivot is the smallest element - no need to recur for
    // the left part.
    if (newHead != pivot) {
      // Set the node before the pivot node as NULL
      struct node *tmp = newHead;
      while (tmp->next != pivot)
          tmp = tmp->next;
      tmp->next = NULL;
      // Recur for the list before pivot
      newHead = quickSortRecur(newHead, tmp);

      // Change next of last node of the left half to pivot
      tmp = getTail(newHead);
      tmp->next = pivot;
    }
    // Recur for the list after the pivot element
    pivot->next = quickSortRecur(pivot->next, newEnd);

    return newHead;
}

// The main function for quick sort. This is a wrapper over recursive
// function quickSortRecur()
void quickSort(struct node **headRef){
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef));
    return;
}
void displayList(struct node *tweeter){
    struct node *temp;
    if(tweeter == NULL)  {
      printf("List is empty.");
    } else {
      temp = tweeter;
      while(temp != NULL) {
        printf("%s : %d\n",temp->name ,temp->number );
        temp = temp->next;                 // Move to next node
      }
    }
}
void displayTopTen(struct node *tweeter){
  struct node *temp;
  int count = 0;
  if(tweeter == NULL)  {
    printf("List is empty.");
  } else {
    temp = tweeter;
    while(temp != NULL && count !=10) {
      printf("%s : %d\n",temp->name ,temp->number );
      temp = temp->next;                 // Move to next node
      count++;
    }
  }
}
/*There is identical tweeter in the linked list*/
bool search(struct node* front,char *newName){
  struct node* curr = front;  // Initialize current
  bool result = false;
  while (curr != NULL){
     if (strcmp (newName, curr -> name)==0){
       curr->number ++;
       result = true;
      }
     curr = curr->next;
  }
  return result;
}
char *replaceWord(char *line, char *old,char *new){
    char *result;
    int i, cnt = 0;
    int new_len = strlen(new);
    int old_len = strlen(old);
    // Counting the number of times old word
    // occur in the string
    for (i = 0; line[i] != '\0'; i++) {
      if (strstr(&line[i], old) == &line[i]) {
        cnt++;
        // Jumping to index after the old word.
        i += old_len - 1;
      }
    }
    // Making new string of enough length
    //result = (char *)malloc(i + cnt * abs(new_len - old_len) + 1);
    /*사이즈(sizeof)를 좀 더 구체적으로 설정해줘야할거같다.*/
    result = (char *)malloc(N*sizeof(char));
    i = 0;
    while (*line) {
      // compare the substring with the result
      if (strstr(line, old) == line){
        strcpy(&result[i], new);
        i += new_len;
        line += old_len;
      } else {
        result[i++] = *line++;
      }
    }
    result[i] = '\0';
  return result;
}
int main(int argc, char **argv){
  FILE *fp;
  char *line = NULL;
  char ch;
  size_t len=N;
  ssize_t read;
  int limit = 0;
  int name_limit=0;
  int tweeterNum = 0;
  int size;
  char commas[] = ",";
  char *ptr = NULL;
  struct node *tweeter = (struct node *)malloc(sizeof(struct node));
  tweeter->next = NULL;
  tweeter->name = "";
  tweeter->number = 0;

fp = fopen(argv[1], "r");
    
  if(fp){
    /* Read the first line */
    if((read = getline(&line, &len, fp)) != -1) {
      /*print out first line*/
      //printf("%s\n", line );
      size = strlen(line);  //size of line - firstline : 231
      ptr = strtok(line,commas); // split a line with commas
      while(ptr != NULL && !strstr(ptr,"name")){
        printf("'%s'\n", ptr);
		    ptr = strtok(NULL, commas);
        limit++; //Find the name's column
      }
      printf("%d\n",limit);

      /*check wheether a line's size is over 1024 or not*/
      while((read = getline(&line, &len, fp)) != -1) {
        /*Remove ,, in every sigle line*/
        if(strlen(line) <=1024){
          line = replaceWord(line,",,,",",0,0,");
          line = replaceWord(line,",,",",0,");

          ptr = strtok(line,commas); // split a line with commas
          while(ptr != NULL){
            /*Find the tweeter*/
            if(name_limit == limit){
              if(!search(tweeter,ptr)){
                /*if doesn't exist, create new one*/
                struct node * temp;
                struct node* new_node = (struct node*) malloc(sizeof(struct node));
                if(new_node == NULL){
                  /*Unable to allocate memory*/
                } else {
                  new_node -> number = 1;
                  new_node -> name = (char*)malloc(strlen(ptr)+1);
                  strcpy(new_node -> name, ptr);
                  new_node->next = NULL;
                  temp = tweeter;
                  /*Traverse to the last node*/
                  while(temp->next != NULL){
                    temp = temp->next;
                  }
                  temp->next = new_node;
                }
              }
            }
    		    ptr = strtok(NULL, commas);
            name_limit++;
          }
        } else {
            printf("Invalid Input, Out of bound 1024");
        }
        name_limit=0;
      }
    }
  } else {
    printf("File does not exist\n");
  }

  quickSort(&tweeter);
  //displayList(tweeter);
  displayTopTen(tweeter);
  if(fp){
    fclose(fp);
  }
  return 0;
}
