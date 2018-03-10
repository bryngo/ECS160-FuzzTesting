//
// Created by Bryan Ngo on 3/6/18.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

typedef struct  {
  char name[50]; // max username length can be no longer than 50 characters
  int tweetCount; // number of tweets this user has tweeted
} User;

unsigned int hash(unsigned char *str) {
  unsigned int hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash % 20000;
} // returns hash value of string, 20,000 slots

char* getfield(char* line, int num ) {
  char* tok;
  for (tok = strtok(line, ",");tok && *tok;tok = strtok(NULL, ",\n"))
  {
    if (!num--)
      return tok;
  }
  return NULL;
} // returns the num-th column of the line

int getNameCol(char *header) {
  int colNum = 0;


  char target[50], target2[50];
  strcpy(target, "\"name\"");
  strcpy(target2, "name");

  const char* tok;

  for (colNum = 0, tok = strtok(header, ",");tok && *tok;tok = strtok(NULL, ",\n"), colNum += 1)
  {

    // we found the name column
    if (strcmp(tok, target) == 0 || strcmp(tok, target2) == 0)
      return colNum;
  }
  return -1;
} // returns the column number for the header

int comp(const void *user1, const void *user2) {

  User* tweeter1 = (User*)user1;
  User* tweeter2 = (User*)user2;

  if(tweeter1->tweetCount < tweeter2->tweetCount) return 1;
  if(tweeter1->tweetCount > tweeter2->tweetCount) return -1;
  return 0;
} // sorts users based on tweet count

int getColCount(char *line) {
  int num = 0;
  char* tok;

  char linecpy[500];
  strcpy(linecpy, line);


  for (tok = strtok(linecpy, ",");tok && *tok;tok = strtok(NULL, ",\n"), num+=1) { }

  return num;
} // returns the number of columns in the line

void init(User* userArray) {

  int i;

  for(i = 0; i < 20000; i += 1) {
    strcpy(userArray[i].name, "");
    userArray[i].tweetCount = 0;
  }
} // zero out all the values in our user array

void error() {
  printf("Invalid Input Format\n");
  exit(1);
} // prints error messages and exits the program

void insert(char * user, User* userArray) {

  // compute the hash of the user
  unsigned int index = hash((unsigned char*)user);

  // we have a hit
  if(strcmp(userArray[index].name, user) == 0) {
    userArray[index].tweetCount += 1;

    // else we have an empty bucket
  } else if(userArray[index].tweetCount == 0) {
    strcpy(userArray[index].name, user);
    userArray[index].tweetCount += 1;

    // else, we need to start probing
  } else if(userArray[index].tweetCount != 0) {

    // linear probing
    while(userArray[index].tweetCount != 0) {
      index += 1;

      if(index >= 20000) index = 0;
    }

    userArray[index].tweetCount += 1;
    strcpy(userArray[index].name, user);
  }

} // increments a user's tweet count

int main(int argc, char**argv) {

  // holds array of users
  User userArray[20000];
  init(userArray);

  // variables used to read file lines
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int nameColumn, headerCols, numCols, numRows = 1;

  // for temp use
  char *filepath = "./afl/in/";

  char* name_with_extension;
  name_with_extension = malloc(strlen(filepath)+1+strlen(argv[1]));
  strcpy(name_with_extension, filepath); /* copy name into the new var */
  strcat(name_with_extension, argv[1]); /* add the extension */

  // open the file
  FILE *fp;
  printf("Opening file: %s\n", argv[1]);
  fp=fopen(argv[1], "r");

  // if we got a bad file
  if(fp == NULL) { error(); }

  // get the header line, and find out where name column is
  getline(&line, &len, fp);

  headerCols = getColCount(line);
  nameColumn = getNameCol(line);

  // if the header column is found larger than the number of columns, or no `name` column found
  if(nameColumn > headerCols || nameColumn == -1) { error(); }

  // while there are lines in the file
  while ((read = getline(&line, &len, fp)) != -1) {
    numRows += 1;

    if(numRows >= 20000) { error(); }

    numCols = getColCount(line);

    // if the number of columns found in the line
    if(numCols != headerCols) { error(); }
    insert(getfield(line, nameColumn), userArray);
  }

  // sort the users in decreasing order, based on tweet count
  qsort(userArray, sizeof(userArray)/sizeof(*userArray), sizeof(*userArray), comp);

  int printIndex = 0;
  for(; printIndex < 10; printIndex += 1) {
    printf("%s : %d\n", userArray[printIndex].name, userArray[printIndex].tweetCount);
  }
  // close the file
  fclose(fp);

} // main f(x)