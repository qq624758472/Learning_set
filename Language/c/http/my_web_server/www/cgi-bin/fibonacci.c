/*
 * fibonacci.c - a minimal CGI program that calculates the fibonacci value.
 */
/* $begin fibonacci */
#include "csapp.h"

int fibonacci(int n) {
	if (n <= 0) {
		return 0;
	}
	if ((n == 1) || (n == 2)) {
		return 1;
	}
	return fibonacci(n-1) + fibonacci(n-2);
}

int main(void) {
	char msg[MAXLINE], content[MAXLINE];
	char *env;
	int n=0;

	env = getenv("QUERY_STRING");

	/* Extract the two arguments */
	if (env != NULL) {
		if (sscanf(env, "n=%d\n", &n) == 1) 
			sprintf(msg, "fib(%d) -> %d\n", n, fibonacci(n));
		else
			sprintf(msg, "Can't parse buffer '%s'\r\n<br/>Usage fibonacci?n=x<br/>\r\n", env);
	}
	/* Make the response body */
	sprintf(content, "Welcome to add.com: ");
	sprintf(content, "%sTHE Internet fibonacci portal.\r\n<p>", content);
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
/* $end fibonacci */
