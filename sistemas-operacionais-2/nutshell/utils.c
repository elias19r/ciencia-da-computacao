/* utils. c - some handy functions. */

#include <debug.h>
#include <nutshell.h>

/* Allocate and initialize a list. Parameter del is a pointer
   to a function to be used to release the memory used by
   a node's value.*/

list_t *new_list(void (*del) (void *))
{
	list_t *list;
	list = malloc(sizeof(list_t));
	sysfault(!list, NULL);
	list->size = 0;
	list->first = NULL;
	list->last = NULL;
	list->del = del;
	return list;
}

/* Free memory used by a list. */

void release_list(list_t * list)
{
	list_node_t *p;

	fatal(!list->del, "Don't know how to free nodes.");

	/* Free all nodes. */
	p = list->first;
	while (p) {
		list->del(p);
		p = p->next;
	}

	free(list);

}

/* Append a node at the end of the list. */

list_node_t *append_node(list_t * list)
{
	list_node_t *node;

	node = malloc(sizeof(list_node_t));
	sysfault(!node, NULL);
	node->value = NULL;

	if (list->first == NULL) {
		list->first = node;
		node->previous = NULL;
	} else {
		list->last->next = node;
		node->previous = list->last;
	}

	list->last = node;
	node->next = NULL;
	list->size++;

	return node;
}

/* Remove a node from the list. */

int del_node(list_t * list, list_node_t * node)
{
	list_node_t *p;
	/* Seek the node. */
	p = list->first;
	while ((p) && (p != node))
		p = p->next;

	/* No such node. */
	if (!p)
		return EXIT_FAILURE;

	/* Ok node found. Let's remove it. */

	if (node == list->first)
		list->first = node->next;
	else
		node->previous->next = node->next;

	if (node == list->last)
		list->last = node->previous;
	else
		node->next->previous = node->previous;

	list->del(node);
	list->size--;

	return EXIT_SUCCESS;
}

/* String dup. */

char *stringdup(const char *str)
{
	char *p;
	int n;

	n = strlen(str) + 1;
	p = malloc(n * sizeof(char));
	sysfault(!p, NULL);
	strcpy(p, str);
	return p;
}
