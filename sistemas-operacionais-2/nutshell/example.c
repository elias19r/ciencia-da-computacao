/* example.c - Example of list functions. */

#include <stdlib.h>
#include <stdio.h>

#include <nutshell.h>		/* List api is here. */

/* Function used to free a node's value. */
void delstring(void *str)
{
	free(str);
}

int main(int argc, char **argv)
{
	list_t *list;
	list_node_t *p, *p1, *p2, *p3, *p4;

	/* Get a new empty list. */

	list = new_list(delstring);

	/* Append some nodes. */

	p1 = append_node(list);
	p1->value = stringdup("Node 1");

	p2 = append_node(list);
	p2->value = stringdup("Node 2");

	p3 = append_node(list);
	p3->value = stringdup("Node 3");

	/* Show list. */

	p = list->first;
	while (p) {
		printf("> %s\n", (char *)p->value);
		p = p->next;
	}

	/* Delete a given node (p2). */

	printf("Del\n");
	del_node(list, p2);

	/* Show the list again. */

	p = list->first;
	while (p) {
		printf("> %s\n", (char *)p->value);
		p = p->next;
	}

	printf("Add\n");

	/* Append another node. */

	p4 = append_node(list);
	p4->value = stringdup("Node 4");

	/* Show the list once more. */

	p = list->first;
	while (p) {
		printf("> %s\n", (char *)p->value);
		p = p->next;
	}

	return EXIT_SUCCESS;
}
