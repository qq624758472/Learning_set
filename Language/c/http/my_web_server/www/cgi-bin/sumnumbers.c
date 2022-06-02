#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sum_internal(char *query_str, int *num1, int *num2, int *sum);
void sumnumbers(char *query_str);

int main(int argc, char **argv)
{
  sumnumbers(getenv("QUERY_STRING"));
  return 0;
}

void sumnumbers(char *query_str)
{
  int num1, num2, sum;

  printf("HTTP/1.0 200 OK\r\n");
  printf("Content-type: text/html\r\n");
  printf("\r\n");
  if(sum_internal(query_str, &num1, &num2, &sum) != 0)
  {
    printf("<h2>Invalid Numbers passed: %s</h2>", query_str);
  }
  else
  {
    printf("<h2>Number1: %d, Number2: %d, Sum: %d</h2>", num1, num2, sum);
  }
  printf("\r\n");
}

int sum_internal(char *query_str, int *num1, int *num2, int *sum)
{
  int pos;
  char *strpos; 
  char str1[32], str2[32];

  if(query_str == NULL)
    return -1;

  strpos = strchr(query_str, '&');
  if(strpos == NULL)
    return -1;

  pos = strpos - query_str;
  
  memset(str1, 0, 32);
  memset(str2, 0, 32);

  strncpy(str1, query_str, pos);
  strcpy(str2, strpos+1);

  strpos = strchr(str1, '&');
  if(strpos != NULL)
    return -1;
  
  strpos = strchr(str1, '=');
  if(strpos == NULL)
    return -1;

  *strpos = 0;
  if(strcmp(str1, "num1") != 0)
    return -1;

  *num1 = atoi(strpos+1);


  strpos = strchr(str2, '&');
  if(strpos != NULL)
    return -1;

  strpos = strchr(str2, '=');
  if(strpos == NULL)
    return -1;

  *strpos = 0;
  if(strcmp(str2, "num2") != 0)
    return -1;

  *num2 = atoi(strpos+1);

  *sum = *num1 + *num2;

  return 0;
}
