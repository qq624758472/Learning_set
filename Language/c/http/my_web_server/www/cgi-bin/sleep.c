/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
	char msg[MAXLINE], content[MAXLINE];
	char *env;
	int n=0;

	env = getenv("QUERY_STRING");

	if (env != NULL) {
		if (sscanf(env, "n=%d\n", &n) == 1) 
			sprintf(msg, "Sleeping for %d seconds\n", n);
		else
			sprintf(msg, "Can't parse buffer '%s'\n", env);
	}

	sleep(n);

	/* Make the response body */
	sprintf(content, "Welcome to add.com: ");
	sprintf(content, "%sTHE Internet Sleep portal.\r\n<p>", content);
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
/* $end adder */
