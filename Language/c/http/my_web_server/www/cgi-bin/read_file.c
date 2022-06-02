/*
 * read_file.c Reads a file from disk and outputs it
 */
#include "csapp.h"

int main(void) {
	char content[MAXLINE];
	char *env;
	int file_fd;
	struct stat st_buf;
	void *file_ptr = NULL;
	char file_name[MAXLINE];

	env = getenv("QUERY_STRING");

	/* Make the response body */
	sprintf(content, "Welcome to the online reader: ");
	sprintf(content, "%sThis is a text.\r\n<p>", content);

	if (env == NULL) {
		goto error;
	}
	if (sscanf(env, "file=%s\n", file_name) != 1) {
		goto error;
	}
	if (stat(file_name, &st_buf) < 0) {
		goto error;
	}
	file_fd = open(file_name, O_RDONLY, 0);
	if (file_fd < 0) {
		goto error;
	}
	file_ptr = mmap(0, st_buf.st_size, PROT_READ, MAP_PRIVATE, file_fd, 0);
	if (file_ptr == (void *)-1) {
		close(file_fd);
		goto error;
	}

	/* Generate the HTTP response */
	printf("Content-length: %u\r\n",
				 (unsigned) (strlen(content) + st_buf.st_size));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	printf("%s\r\n", (char *)file_ptr);
	fflush(stdout);

	munmap(file_ptr, st_buf.st_size);
	close(file_fd);

	return 0;

	error:
	sprintf(content, "%sError reading file!<br/>Usage read_file?file=X<br/>\r\n", content);
	printf("Content-length: %u\r\n",(unsigned) strlen(content));
	printf("Content-type: text/html\r\n\r\n");
	printf("%s", content);
	fflush(stdout);
	return 1;
}
