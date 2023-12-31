/* File: vertex_list.h */

    /**************************************************************
    *  Linked-list vertex structures and macros to maintain them. *
    *  Based on code from "Computational geometry in C", Joseph   *
    *  O'Rourke, Cambridge University Press, 1994.                *
    **************************************************************/

#ifndef _VERTEX_LIST_H_
#define _VERTEX_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include "libsx.h"           /* For definition of XPoint */

#define X 0
#define Y 1

#define ABS(x)     (((x) < 0) ? -(x) : (x))

#define NEW(p,type)	if ((p=(type *) malloc (sizeof(type))) == NULL) {\
				fprintf (stderr,"Out of Memory!\n");\
				exit(0);\
			}

#define FREE(p)		if (p) { free ((char *) p); p = NULL; }


#define ADD( tail, p )  if ( tail )  { \
				p->next = tail; \
				p->prev = tail->prev; \
				tail->prev = p; \
				p->prev->next = p; \
			} \
			else { \
				tail = p; \
				tail->next = tail->prev = p; \
			}

#define DELETE( tail, p )   if ( tail )  { \
				if ( tail == tail->prev ) \
					tail = NULL;  \
				else if ( p == tail ) \
					tail = tail->prev; \
				p->prev->next = p->next;  \
				p->next->prev = p->prev;  \
				FREE( p ); \
			} 

/* Define structure for polyline vertices */
typedef struct tVertexStructure tsVertex;    /* used to allocate storage */
typedef tsVertex *tVertex;                   /* pointer into vertex list */

struct tVertexStructure {
  XPoint  xp;
  int     vnum;
  tVertex next, prev;
};

/* Function declarations */
tVertex MakeVertex( tVertex vertices );

#endif /* _VERTEX_LIST_H_ */

/* End of File: vertex_list.h */
