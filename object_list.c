/* File: object_list.c */

    /*******************************************************************
    * Implementation of linked-list.  Based on code from "Data         *
    * Structures and Algorithm Analysis in C, 2nd Edition", Mark Allen *
    * Weiss, Addison-Wesley, 1997.                                     *
    *******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "object_list.h"

#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

/* Declared in the interface file */
struct Node
{
  ElementType     Element;
  ListPosition    Next;
};

/* An application specific equality operator for ElementType */
int
isEqual(ElementType Obj1, ElementType Obj2)
{
  return (Obj1.object_type    == Obj2.object_type  &&
	  Obj1.bb.left_edge   == Obj2.bb.left_edge &&
	  Obj1.bb.top_edge    == Obj2.bb.top_edge  &&
	  Obj1.bb.width       == Obj2.bb.width     &&
	  Obj1.bb.height      == Obj2.bb.height    &&
	  Obj1.color          == Obj2.color        &&
	  Obj1.LineWidth      == Obj2.LineWidth    &&
	  Obj1.fill_attribute == Obj2.fill_attribute);
}


List
MakeEmpty( List L )
{
  if( L != NULL )
    DeleteList( L );
  L = malloc( sizeof( struct Node ) );
  if( L == NULL )
    FatalError( "Out of memory!" );
  L->Next = NULL;
  return L;
}


/* Return true if L is empty */
int
IsEmpty( List L )
{
  return L->Next == NULL;
}


/* Return true if P is the last position in list L */
/* Parameter L is unused in this implementation */
int IsLast( ListPosition P, List L )
{
  return P->Next == NULL;
}


/* Return ListPosition of Obj in L; NULL if not found */
ListPosition
Find( ElementType Obj, List L )
{
  ListPosition P;
  
  P = L->Next;
  while( P != NULL &&
	 ! isEqual(P->Element, Obj) )    /* P->Element != Obj */
    P = P->Next;
  
  return P;
}


/* Delete from a list */
/* Cell pointed to by P->Next is wiped out */
/* Assumes that the position is legal */
/* Assumes use of a header node */
void
Delete( ElementType Obj, List L )
{
  ListPosition P, TmpCell;
  
  P = FindPrevious( Obj, L );
  
  if( !IsLast( P, L ) )              /* Assumption of header use */
    {                                /* Obj is found; delete it  */
      TmpCell = P->Next;
      P->Next = TmpCell->Next;       /* Bypass deleted cell      */
      free( TmpCell );
    }
}


/* If Obj is not found, then Next field of returned value is NULL */
/* Assumes a header */
ListPosition
FindPrevious( ElementType Obj, List L )
{
  ListPosition P;
  
  P = L;
  while( P->Next != NULL &&
	 ! isEqual(P->Next->Element, Obj) )  /* P->Next->Element != Obj */
    P = P->Next;
  
  return P;
}


/* Insert (after legal ListPosition P) */
/* Header implementation assumed */
/* Parameter L is unused in this implementation */
void
Insert( ElementType Obj, List L, ListPosition P )
{
  ListPosition TmpCell;
  
  TmpCell = malloc( sizeof( struct Node ) );
  if( TmpCell == NULL )
    FatalError( "Out of space!!!" );
  
  TmpCell->Element = Obj;
  TmpCell->Next = P->Next;
  P->Next = TmpCell;
}


void
DeleteList( List L )
{
  ListPosition P, Tmp;
  
  P = L->Next;  /* Header assumed */
  L->Next = NULL;
  while( P != NULL )
    {
      Tmp = P->Next;

      free( P );
      P = Tmp;
    }
}


ListPosition
Header( List L )
{
  return L;
}


ListPosition
First( List L )
{
  return L->Next;
}


ListPosition
Advance( ListPosition P )
{
  return P->Next;
}


ElementType
Retrieve( ListPosition P )
{
  return P->Element;
}

/* End of File: object_list.c */
