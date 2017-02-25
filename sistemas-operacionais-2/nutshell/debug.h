/* debug.h - Debug utilities (non-public).*/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_ERR "Something wrong"

/* 
	System errors.
*/

/* Report system error and exit. */

#define sysfatal(expression) \
	do { if ((expression)) {fprintf (stderr, "%s: %s: %d: %s: %s\n",	\
		"Fatal ", __FILE__, __LINE__,\
			__PRETTY_FUNCTION__, strerror (errno)); \
	exit (EXIT_FAILURE);}} while (0)

/* Report a system error and returns withe the specified code. */

#define sysfault(expression, return_status)						\
	do { if (expression) {fprintf (stderr, "%s: %s: %d: %s: %s\n",	\
		"Fault ", __FILE__, __LINE__,\
			__PRETTY_FUNCTION__, strerror (errno)); \
		return (return_status);}} while (0)

/* Report a system error. */

#define sysdebug(expression)						\
	do { if (expression) {fprintf (stderr, "%s: %s: %d: %s: %s\n",	\
		"Debug ", __FILE__, __LINE__,\
			__PRETTY_FUNCTION__, strerror (errno)); \
	}} while (0)

/* 
	Custom errors.
*/

/* Rerport a custom error and exit. */

#define fatal(expression, message)						\
	do { if ((expression)) {fprintf (stderr, "%s: %s: %d: %s: %s\n",	\
		"Fatal ", __FILE__, __LINE__,\
			__PRETTY_FUNCTION__, message ? message : DEFAULT_ERR ); \
	exit (EXIT_FAILURE);}} while (0)

/* Report a custom error and returns withe the specified code. */

#define fault(expression, return_status, message)				\
	do { if (expression) {fprintf (stderr, "%s: %s: %d: %s: %s\n",	\
		"Fault ", __FILE__, __LINE__,\
			__PRETTY_FUNCTION__, message ? message : DEFAULT_ERR ); \
	return (return_status);}} while (0)

/* Report a custom error. */

#define debug(expression, message)				\
	do { if (expression) {fprintf (stderr, "%s: %s: %d: %s: %s\n",	\
		"Debug ", __FILE__, __LINE__,\
		__PRETTY_FUNCTION__, message ? message : DEFAULT_ERR ); \
	}} while (0)

#endif				/* _DEBUG_H_ */
