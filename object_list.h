/* File: object_list.h */

    /*******************************************************************
    * Declarations for linked-list.  Based on code from "Data          *
    * Structures and Algorithm Analysis in C, 2nd Edition", Mark Allen *
    * Weiss, Addison-Wesley, 1997.                                     *
    *******************************************************************/

#ifndef _Object_List_H_
#define _Object_List_H_

#include "vertex_list.h"

typedef enum ObjectType
{line, rectangle, ellipse, circle, polyline, text} ObjectType;

typedef struct BoundingBox
{
  short left_edge, top_edge;
  short width, height;
} BoundingBox;

typedef struct Draw_Object
{
  ObjectType  object_type;
  BoundingBox bb;
  tVertex     v;
  int         npts;
  int         color;
  int         LineWidth;
  int         fill_attribute;
  char       *txt;
} Draw_Object;

typedef Draw_Object ElementType;

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode ListPosition;

/* Return an empty list. Typical usage: List L = MakeEmpty( NULL ); */
List MakeEmpty( List L );

/* Return true if L is empty, false otherwise. */
int IsEmpty( List L );

/* Return true if P is the last position in list L. */
int IsLast( ListPosition P, List L );

/* Return position of Obj in L; NULL if not found. */
ListPosition Find( ElementType Obj, List L );

/* Return position of element previous to Obj.  If Obj is not found, the
   Next field of returned value is NULL.  FindPrevious() is used by
   Delete(). */
ListPosition FindPrevious( ElementType Obj, List L );

/* Delete Obj from the list. */
void Delete( ElementType Obj, List L );

/* Insert Obj after legal position P. */
void Insert( ElementType Obj, List L, ListPosition P );

/* Delete the entire list. */
void DeleteList( List L );

/* Return position of Header node. */
ListPosition Header( List L );

/* Return position of first node following Header. */
ListPosition First( List L );

/* Return the position of the next node. */
ListPosition Advance( ListPosition P );

/* Retrieve element at position pointed to by P.  Return value is
   undefined if current position is NULL. */
ElementType Retrieve( ListPosition P );

#endif    /* _Object_List_H_ */

/* End of File: object_list.h */
