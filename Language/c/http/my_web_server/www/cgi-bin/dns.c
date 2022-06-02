#include "csapp.h"

int main(void) {
	char content[MAXLINE], *env, msg[MAXLINE];
	char domain[MAXLINE], error[MAXLINE];
	char **pp;
	struct in_addr addr;
	struct hostent *hostp;

	env = getenv("QUERY_STRING");

	/* Make the response body */
	sprintf(content, "Welcome to the online dns resolver:<br/>\r\n");

	if (env == NULL) {
		sprintf(error, "Can't scan buffer!<br/>Usage: dns?domain=X<br/>\r\n");
		goto error;
	}
	if (sscanf(env, "domain=%s\n", domain) != 1) {
		sprintf(error, "Can't scan buffer!<br/>Usage: dns?domain=X<br/>\r\n");
	}

	sprintf(msg, "The answer to your query \'%s\':<br/>\r\n", domain);

	/* Resolve domain name */
	if ((hostp = gethostbyname(domain)) == NULL) {
		sprintf(error, "Invalid domain name: %s<br/>", domain);
		goto error;
	}

	/* Get offical host name */
	sprintf(msg, "%sOfficial hostname: %s<br/>\r\n", msg, hostp->h_name);

	/* Get aliases */
	for (pp = hostp->h_aliases; *pp != NULL; pp++) {
		sprintf(msg, "%s Alias: %s<br/>\r\n", msg, *pp);	
	}

	/* Get addresses */
	for (pp = hostp->h_addr_list; *pp != NULL; pp++) {
		addr.s_addr = ((struct in_addr *)*pp)->s_addr;
		sprintf(msg, "%s Address: %s<br/>\r\n", msg, inet_ntoa(addr));
	}

	/* Generate the HTTP response */
	printf("Content-length: %u\r\n",
				 (unsigned) (strlen(content) + strlen(msg)));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	printf("%s", msg);
	fflush(stdout);
	return 0;

	error:
	printf("Content-length: %u\r\n",
					(unsigned) (strlen(content)+ strlen(error)));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	printf("%s", error);
	fflush(stdout);
	return 1;
}
