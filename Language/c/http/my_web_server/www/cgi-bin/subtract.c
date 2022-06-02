/*
 * subtract.c - a minimal CGI program that sbutracts two numbers together
 */
/* $begin subtract */
#include "csapp.h"

int main(void) {
	char msg[MAXLINE], content[MAXLINE];
	char *env;
	int n1=0, n2=0;

	env = getenv("QUERY_STRING");

	/* Extract the two arguments */
	if (env != NULL) {
		if (sscanf(env, "n1=%d&n2=%d\n", &n1, &n2) == 2) 
			sprintf(msg, "%d - %d -> %d\n", n1, n2, n1 - n2);
		else
			sprintf(msg, "Can't parse buffer '%s'\n", env);
	}
	/* Make the response body */
	sprintf(content, "Welcome to add.com: ");
	sprintf(content, "%sTHE Internet subtraction portal.\r\n<p>", content);
	sprintf(content, "%sThe answer is: %s\r\n<p>", 
	content, msg);
	sprintf(content, "%sThanks for visiting!\r\n", content);

	/* Generate the HTTP response */
	printf("Content-length: %u\r\n", (unsigned) strlen(content));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	fflush(stdout);
	return 0;
}
