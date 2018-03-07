//
// Created by Bryan Ngo on 3/6/18.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

/// TODO: Write a function to count commas in a given line
/// TODO: Write a function to determine a valid header
/// TODO: Check that our program doesn't crash in empty csv files

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

  char target[50];
  strcpy(target, "\"name\"");

  const char* tok;

  for (colNum = 0, tok = strtok(header, ",");tok && *tok;tok = strtok(NULL, ",\n"), colNum += 1)
  {

    // we found the name column
    if (strcmp(tok, target) == 0)
      return colNum;
  }
  return -1;
} // returns the column number for the header

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

int comp(const void *user1, const void *user2) {

  User* tweeter1 = (User*)user1;
  User* tweeter2 = (User*)user2;

  if(tweeter1->tweetCount < tweeter2->tweetCount) return 1;
  if(tweeter1->tweetCount > tweeter2->tweetCount) return -1;
  return 0;
} // sorts users based on tweet count

void init(User* userArray) {

  int i;

  for(i = 0; i < 20000; i += 1) {
    strcpy(userArray[i].name, "");
    userArray[i].tweetCount = 0;
  }
}

int main(int argc, char**argv) {

  // holds array of users
  User userArray[20000];

  init(userArray);

  // variables used to read file lines
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int nameColumn;

  // for temp use
  char filepath[100];
  strcpy(filepath, "/Users/Bryan1/Desktop/160/projects/p4/cl-tweets-short.csv");

  // open the file
  FILE *fp;
  printf("Opening file: %s\n", argv[1]);
  fp=fopen(filepath, "r");

  // if we got a bad file
  if(fp == NULL) {
    printf("Invalid Input Format\n");
    exit(1);
  }

  /// TODO: Check that we actually got a csv file

  // get the header line, and find out where name column is
  getline(&line, &len, fp);
  nameColumn = getNameCol(line); /// TODO: if this number is larger than the number of columns, invalid input

  if(nameColumn == -1) {
    printf("Invalid Input Format\n");
    exit(1);
  }

  userArray[0].tweetCount = 5;

  while ((read = getline(&line, &len, fp)) != -1) {
    insert(getfield(line, nameColumn), userArray);
  }

  qsort(userArray, sizeof(userArray)/sizeof(*userArray), sizeof(*userArray), comp);

  int printIndex = 0;
  for(; printIndex < 10; printIndex += 1) {
    printf("%s : %d\n", userArray[printIndex].name, userArray[printIndex].tweetCount);
  }
  // close the file
  fclose(fp);

}