#include <stdlib.h>
#include <nutshell.h>
#include <unistd.h>
#include <string.h>
#include <debug.h>

#define BUFFER_STEP 512

/* Alocate memory for a command_t. */

buffer_t *new_command_line(void)
{
	buffer_t *command_line;
	command_line = malloc(sizeof(buffer_t));
	sysfault(!command_line, NULL);
	command_line->size = BUFFER_STEP;
	command_line->buffer = malloc(BUFFER_STEP * sizeof(char));
	if (command_line->buffer == NULL) {
		free(command_line);
		sysfault(1, NULL);
	}
	return command_line;
}

/* Release a command_t's memory. */

void release_command_line(buffer_t * command_line)
{
	free(command_line->buffer);
	free(command_line);
}

/* Read a line from standard input and store it in command_line->buffer.
   The buffer size is enlarged if needed in steps of size BUFFER_STEP.
   Return the number of bytes read or -1 on error. */

int read_command_line(buffer_t * command_line)
{
	int read_bytes, count, temp;
	int read_more = 1;
	char *offset, *p;
	int howmany;

	count = 0;
	offset = command_line->buffer;
	howmany = command_line->size;
	command_line->length = 0;
	while (read_more) {

		/* Read howmany bytes. */

		read_bytes = read(1, offset, howmany);
		count += read_bytes;

		/* This happens also if the previous read left
		   a trailing newline. */
		if (offset[0] == '\n') {
			offset[0] = '\0';
			return --count;
		}

		read_more = 0;
		if (read_bytes < howmany)
			offset[read_bytes - 1] = '\0';	/* We read less than howmany; done. */
		else if (offset[read_bytes - 1] == '\n')
			offset[read_bytes - 1] = '\0';	/* We read exactly howmany; done. */
		else {		/* There is more to read. */

			/* Enlarge buffer. */

			temp = command_line->size;
			p = realloc(command_line->buffer,
				    (temp + BUFFER_STEP) * sizeof(char));
			sysfault(!p, -1);

			/* Offest is the end of the buffer. */
			offset = command_line->buffer + count;
			command_line->size += BUFFER_STEP;
			howmany = BUFFER_STEP;

			read_more = 1;
		}

	}

	command_line->length = count;
	return count;
}

/* Allocate memory for a new pipeline. */

pipeline_t *new_pipeline(void)
{
	pipeline_t *pipeline;
	int i, j;

	pipeline = malloc(1 * sizeof(pipeline_t));
	sysfault(!pipeline, NULL);

	/* Allocate a pipeline structure. */

	pipeline->command = malloc((MAX_COMMANDS + 1) * sizeof(char **));
	if (!pipeline->command) {
		sysdebug(1);
		free(pipeline);
		return NULL;
	}

	/* Allocate memory for commands. */
	for (i = 0; i < MAX_COMMANDS + 1; i++) {
		pipeline->command[i] =
		    malloc((MAX_ARGUMENTS + 1) * sizeof(char *));
		if (!pipeline->command[i]) {
			sysdebug(1);
			for (j = 0; j < i; j++)
				free(pipeline->command[j]);
			free(pipeline->command);
			free(pipeline);
			return NULL;
		}
	}

	pipeline->ground = FOREGROUND;
	pipeline->file_in[0] = '\0';
	pipeline->file_out[0] = '\0';

	return pipeline;

}

void release_pipeline(pipeline_t * pipeline)
{
	/* int i,j;                    */
	int i;			/* Iterate through commands. */
	for (i = 0; i < MAX_COMMANDS; i++) {
		/* for (j=0; i<MAX_ARGUMENTS; j++) */
		/*        { */
		/*          free (pipeline->command[i][j]); */
		/*        } */
		free(pipeline->command[i]);
	}
}

#define isblk(c) ((c==' ') || (c=='\t') || (c=='\n')  )

enum {
	PARSER_TOO_MANY_ARGUMENTS = 1,
	PARSER_TOO_MANY_COMMANDS = 2,
	PARSER_STUFF_AFTER_AMP = 4
} parser_error_t;

int find_modifiers(buffer_t * command_line, pipeline_t * pipeline)
{
	int amp;		/* '&' position */
	int lt, gt;
	int len, truncated;
	int i, j, k;

	pipeline->ground = FOREGROUND;
	pipeline->file_in[0] = '\0';
	pipeline->file_out[0] = '\0';

	truncated = 0;
	len = command_line->length;

	/* Find &, if any. */
	amp = strcspn(command_line->buffer, "&");

	if (amp != (len - 1))
		pipeline->ground = BACKGROUND;

	for (i = amp + 1; i < len - 1; i++)
		if (!isblk(command_line->buffer[i]))
			truncated |= PARSER_STUFF_AFTER_AMP;
	command_line->buffer[amp] = '\0';
	command_line->length = amp;
	len = amp;

	/* Find output redirect. */

	lt = strcspn(command_line->buffer, "<");
	if (lt != len) {
		for (i = lt + 1; isblk(command_line->buffer[i]); i++) ;
		/* printf ("lt found (%s)\n", command_line->buffer+i); */
		j = 0;
		while ((i < len) && (i < MAX_FILENAME) &&
		       (!isblk(command_line->buffer[i]))
		       && (command_line->buffer[i] != '<'))
			pipeline->file_in[j++] = command_line->buffer[i++];
		pipeline->file_in[j] = '\0';
	}
	/* printf ("lt now (%s)\n", pipeline->file_in); */

	gt = strcspn(command_line->buffer, ">");
	if (gt != len) {
		if (command_line->buffer[gt + 1] == '>') {
			pipeline->redirect = APPEND;
			gt++;
		} else {
			pipeline->redirect = WRITE;
		}
		for (i = gt + 1; isblk(command_line->buffer[i]); i++) ;
		/* printf ("gt found (%s)\n", command_line->buffer+i); */
		j = 0;
		while ((i < len) && (i < MAX_FILENAME) &&
		       (!isblk(command_line->buffer[i]))
		       && (command_line->buffer[i] != '<'))
			pipeline->file_out[j++] = command_line->buffer[i++];
		pipeline->file_out[j] = '\0';
	}
	/* printf ("gt now (%s)\n", pipeline->file_out); */

	k = lt < gt ? lt : gt;
	k = k < len ? k : k + 1;
	command_line->buffer[k - 1] = '\0';
	command_line->length = k;

	debug(truncated && PARSER_STUFF_AFTER_AMP, "Nothing allowed after '&'");

	return truncated;

}

/* Parse command_line into pipeline. Return 0 on success, 1 if too many arguments,
   2 if too many commands in a pipeline; 1+2 if both errors occur.*/

int parse_command_line(buffer_t * command_line, pipeline_t * pipeline)
{
	int i, j, truncated;
	char *token, *token2, *bkstring;

	truncated = 0;

	truncated |= find_modifiers(command_line, pipeline);

	i = 0;
	while ((i < MAX_COMMANDS) &&
	       (token =
		strtok_r(i == 0 ? command_line->buffer : NULL, "|",
			 &bkstring))) {
		while ((token[0] == ' ') || (token[0] == '\t'))
			token++;

		j = 0;
		while ((j < MAX_ARGUMENTS) &&
		       (token2 = strtok(j == 0 ? token : NULL, " \t"))) {
			pipeline->command[i][j] = token2;
			j++;
		}

		pipeline->command[i][j] = NULL;
		i++;
		truncated |=
		    strtok(NULL, " \t") ? PARSER_TOO_MANY_ARGUMENTS : 0;

		pipeline->narguments[i - 1] = j;

	}
	pipeline->command[i][0] = NULL;
	truncated |=
	    strtok_r(NULL, " \t", &bkstring) ? PARSER_TOO_MANY_COMMANDS : 0;

	pipeline->ncommands = i;

	debug(truncated & PARSER_TOO_MANY_ARGUMENTS,
	      "Too many arguments in a command");
	debug(truncated & PARSER_TOO_MANY_COMMANDS,
	      "Too many commands in a pipeline");

	return truncated;
}
