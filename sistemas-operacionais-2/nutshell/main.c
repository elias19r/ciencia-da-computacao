#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <nutshell.h>
#include <debug.h>

#define PROMPT "\\m/:"

int go_on = 1;			/* This variable controls the main loop. */

/*
	Built in function: print help info
*/
void help()
{
	/* TODO write something useful, indeed */
	printf("Insert useful help information here.\n");
}

/*
	Built in function: print a ASCII-style scarab
*/
void escaravelho()
{
	printf("\
	            _\n\
	        _,=(_)=,_\n\
	     ,;`         `;,\n\
	    \\\\    (\\^/)    //\n\
	     \\\\   <( )>   //\n\
	      <`-'`\"\"\"`'-`>\n\
	     _/           \\_\n\
	   _(_\\           /_)_\n\
	  /|` |`----.----`| `|\\\n\
	  |/  |     |     |  \\|\n\
	 />   |     |     |   <\\\n\
	     _;     |     ;_\n\
	   <`_\\     |     /_`>\n\
	   |\\  `._  |  _.'  /|\n\
	   \\|     `\"\"\"`    |/\n\
	    |\\            /|\n\
	     \\\\          //\n\
	     /_>        <_\\\n\
");
}

/*
	Execute a built-in command.
	Return 1 if command is executed as a builtin function, 0 otherwise
*/
int exec_builtin(char *cmd, char **cmdargv)
{
	if (strcmp(cmd, "help") == 0) {
		help();
		return 1;
	}
	if (strcmp(cmd, "escaravelho") == 0) {
		escaravelho();
		return 1;
	}
	if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
		printf("nutshell says: GG\n");	/* Good game, bro (y) */
		sleep(1);
		go_on = 0;
		return 1;
	}
	if (strcmp(cmd, "cd") == 0) {
		char *cwd;

		if (chdir(cmdargv[1]) != 0) {
			printf("%s: could not change directory\n", cmd);
			printf("nutshell says: Did you mistype it?\n");
		}

		cwd = getcwd(NULL, 0);
		setenv("PWD", cwd, 1);
		free(cwd);

		return 1;
	}

	return 0;
}

/*
	Execute a pipeline of commands
*/
void execute_pipeline(pipeline_t * pipeline, int pipeA[2], int index)
{
	int pid, pipeB[2], ret;

	if (index < 0)
		return;

	if (index > 0) {
		ret = pipe(pipeB);
		fatal(ret < 0, "Could not create pipe.");
	}

	pid = fork();
	fatal(pid < 0, "Could not create process");

	/* Parent: exec */
	if (pid > 0) {

		/* Not last, redirect input */
		if (index > 0) {
			close(0);
			dup(pipeB[0]);
			close(pipeB[0]);
		}

		/* Not first, redirect output */
		if (index < pipeline->ncommands - 1) {
			close(1);
			dup(pipeA[1]);
			close(pipeA[1]);
		}

		/* Not used. */
		if (index > 0)
			close(pipeB[1]);

		/* Run command */
		execvp(pipeline->command[index][0], pipeline->command[index]);

		/* Child: recurse */
	} else {
		execute_pipeline(pipeline, pipeB, index - 1);
		return;
	}
}

/*
	Main function
*/
int main(int argc, char **argv)
{
	buffer_t *command_line;
	pipeline_t *pipeline;
	int pid, status, tmp, fd, ret;

	command_line = new_command_line();
	pipeline = new_pipeline();

	/* Welcome message */
	printf("nutshell says: Welcome!\n");

	while (go_on) {
		printf("%s ", PROMPT);
		fflush(stdout);
		ret = read_command_line(command_line);
		fatal(ret < 0, NULL);

		/* Parse command line. */
		if (parse_command_line(command_line, pipeline) == 0) {

			if (pipeline->ncommands < 1)
				continue;

			/* Try to execute built in command */
			if (exec_builtin
			    (pipeline->command[0][0], pipeline->command[0]))
				continue;

			/* Running group of process in foreground */
			if (RUN_FOREGROUND(pipeline)) {

				/* More than one command */
				if (pipeline->ncommands > 1) {

					/* Create another process and check for fatal error */
					pid = fork();
					fatal(pid < 0,
					      "Could not create a new process.");

					/* If parent  */
					if (pid > 0) {
						/* Just wait for child to end */
						wait(&status);

						/* If child */
					} else {
						execute_pipeline(pipeline, NULL,
								 pipeline->
								 ncommands - 1);
						return EXIT_SUCCESS;
					}

					/* Only one command was read */
				} else {

					/* Create another process and check for fatal error */
					pid = fork();
					fatal(pid < 0,
					      "Could not create a new process.");

					/* If parent  */
					if (pid > 0) {
						/* Just wait for child to end */
						wait(&status);

						/* If child */
					} else {

						/* Redirecting input */
						if (REDIRECT_STDIN(pipeline)) {
							tmp =
							    open(pipeline->
								 file_in,
								 O_RDONLY);
							fatal(tmp < 0,
							      "Could not redirect input: file not found");

							/* Close file descriptor 0 which is 'stdin' */
							close(0);

							/* Make a copy of file descriptor opened for input,
							   thus now it is associated with file descriptor 0 that
							   has been just released */
							fd = dup(tmp);
							fatal(fd < 0,
							      "Could not redirect input.");
							close(tmp);
						}

						/* Redirecting output */
						if (REDIRECT_STDOUT(pipeline)) {
							/* '>' Overwrite if file exists, or create a new one */
							if (REDIRECT_STDOUT_WRITE(pipeline)) {
								tmp =
								    open
								    (pipeline->
								     file_out,
								     O_CREAT |
								     O_TRUNC |
								     O_RDWR,
								     S_IRUSR |
								     S_IWUSR);
								fatal(tmp < 0,
								      "Could not redirect output: failed to create a file");

								/* '>>' Append to existing file */
							} else
							    if
							    (REDIRECT_STDOUT_APPEND
							     (pipeline)) {
								tmp =
								    open
								    (pipeline->
								     file_out,
								     O_CREAT |
								     O_APPEND |
								     O_RDWR,
								     S_IRUSR |
								     S_IWUSR);
								fatal(tmp < 0,
								      "Could not redirect output: failed to append to file");
							}

							/* Close file descriptor 1 which is 'stdout' */
							close(1);

							/* Make a copy of file descriptor opened for output,
							   thus now it is associated with file descriptor 1 that
							   has been just released */
							fd = dup(tmp);
							fatal(fd < 0,
							      "Could not redirect input.");
							close(tmp);
						}

						/* Execute a program */
						execvp(pipeline->command[0][0],
						       pipeline->command[0]);

						/* If program not found */
						printf
						    ("%s: command not found\n",
						     pipeline->command[0][0]);
						printf("nutshell says: QQ\n");

						return EXIT_FAILURE;
					}
				}

				/* Running group of process in background */
			} else {
				if (REDIRECT_STDIN(pipeline)) {
					/* TODO */
				}
				if (REDIRECT_STDOUT(pipeline)) {
					/* TODO */
				}
			}
		}
	}

	release_command_line(command_line);
	release_pipeline(pipeline);

	return EXIT_SUCCESS;
}
