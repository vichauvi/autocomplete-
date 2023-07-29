#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct term{
    char term[200]; 
    double weight;
} term;
/*
tasks:
! read in file 
! allocate memory for all terms in filename w/ pointers *terms 
! number of items in file stored in *pnterms 
! qsort in ascending lexicogrpahic order 
! place sorted terms in the block allocated *terms  
*/
char *term_only(char str[200]) 
{
  int stringLength = strlen(str);
  char *finalResult= (char *)malloc(sizeof(char)*stringLength);
  char temporaryResult[200];
  int j = 0;
  int k = 0;

  while (j <strlen(str) && ((isspace(str[j]) == 0) ^ (isdigit(str[j]) ==0)))
  {
    j++;
  }

  while (k<(stringLength-j))//removed +1 -->29-15+1 = 15
  {
    temporaryResult[k] = str[j+k]; //removed -1, result[0] = str[15+0-1]
    k++;
  }
  temporaryResult[k-1] ='\0'; //! k-1 //returns just the string, like Shanghai, China 
  strcpy(finalResult, temporaryResult);
  
  return finalResult; 
}

int comp_test_term(const void *term1, const void *term2) 
{
  return strcmp(((term *)term1)->term, ((term *)term2)->term);
}

void read_in_terms(term **terms, int *pnterms, char *filename)
{
  // * reads in file 
  FILE *fp = fopen(filename, "r"); 
  if (fp == NULL)
  { 
    printf("Error opening file %s\n",filename);
    return;
  }
  char fileLine[200];
  fgets(fileLine, sizeof(fileLine), fp);  
    // *number of items in file stored in *pnterms  

  *pnterms = atoi(fileLine); //set the # of terms to the place for pnterms 
  // *allocate memory for all terms
  //assigns to the pointer to the pointer to terms, the number of terms * the size of each term (max 200 char)
  *terms = (term *)malloc(*pnterms * sizeof(term)); 
  // *place terms in the block allocated  
  // note: now starts at the next line to be read since first line already read 
  for (int i = 0; i < *pnterms; i++)
  {   // using code sol'n from the lab 3 
    fgets(fileLine, sizeof(fileLine), fp); 
    double weight_only = atof(fileLine);
        // printf("term weight: %f\n",weight_only);
    char *term_alone = term_only(fileLine);
        // printf("term: %s\n",term_alone);
    (*terms)[i].weight = weight_only; //like why does it just stop at the 90230th term 
    strcpy((*terms)[i].term, term_alone);
  
  }
  
  qsort(*terms, *pnterms, sizeof(term), comp_test_term);
    // for (int p = 0; p<*pnterms; p++)
  // {
  //   printf("term sorted: %s\n",(*terms)[p].term);
  //   printf("term weight: %f\n",(*terms)[p].weight);
  // } 
  fclose(fp);
}
/*
task:
! use binary search
! return first index of term that matches the substring i.e shan --> shanghai 
! index in the *terms struct 
*/
int lowest_match(term *terms, int nterms, char *substr)
{
    // from index 1 to index nterms 
  // check the middle index
  // move the bottom or top to middle 
  // repeat 
  int low = 0;
  int high = nterms; 
  int substr_length = strlen(substr);
  while (low != high) //* use binary search mayeb change to != high
  {
    int mid = (low + high)/2;

    if (strncmp(terms[low].term, substr, substr_length) == 0)
    {
      return low;
    }
    else if (strncmp(terms[mid].term, substr, substr_length) < 0)
    {
      low = mid + 1;
    }
    else
    {
      high = mid;
    }

  }
  return low;
}


int highest_match(struct term *terms, int nterms, char *substr)
{
  int low = 0;
  int high = nterms;
  int substr_length = strlen(substr);
  while (low != high)
  {
    int mid = (low + high)/2;
    if (strncmp(terms[high].term, substr, substr_length) == 0)
    {
      return high;
    }
    else if (strncmp(terms[mid].term, substr, substr_length) > 0)
    {
      high = mid;
    }
    else
    {
      low = mid + 1;
    }
  }
  return high - 1; 
}

int comp_test_weight(const void *term1, const void *term2)
{
  int weightTerm1 = ((term *)term1)->weight;
  int weightTerm2 = ((term *)term2)->weight;
  return (weightTerm2 - weightTerm1);
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
  
  int highest_ans = highest_match(terms, nterms, substr);
  int lowest_ans = lowest_match(terms, nterms, substr);

  *n_answer = highest_ans - lowest_ans + 1; // high = 9, low = 6 = 6,7,8,9 4 indices


  *answer = (term *)malloc(sizeof(term)* (*n_answer));
  for (int q = 0; q < *n_answer; q++)
  {
    (*answer)[q] = terms[lowest_ans+q]; //answer[0] = terms[...] answer[1] = terms[...+1]

  }
  qsort(*answer, *n_answer, sizeof(term), comp_test_weight); //sort new struct answer with the 
  //added terms by weight 
    /*
  The function takes terms (assume it is sorted lexicographically in increasing order), the number
  of terms nterms, and the query string substr, and places the answers in answer, with *n_answer
  being the number of answers. The answers should be sorted by weight in non-increasing order.
  !place answers (all indices between lowest and highest) in term **answer
  !plcae num of answers in *n_answers
  !sort answers in weight (descending?)
  */
}

int main(void)
{
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "wiktionary.txt");
    struct term *answer;
    int n_answer;
    // printf("first term: %s",terms[0].term);
    autocomplete(&answer, &n_answer, terms, nterms, "pike");
    printf("number of answers: %d\n",n_answer);
    for (int c = 0; c < n_answer; c++)
    {
      printf("term: %s\n",answer[c].term);
    }
    printf("after sorting autocomplete gives the term: %s\n", answer[0].term);
    printf("with a weight of: %d\n", answer[0].weight);
    // free allocated blocks here -- not required for the project, but good practice
    free (terms);
    return 0;
}