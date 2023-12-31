/* File: vertex.c */

    /*************************************************************
    * Implementation of vertex linked-list.  Based on code from  *
    * "Computational geometry in C", Joseph O'Rourke, Cambridge  *
    * University Press, 1994.                                    *
    *************************************************************/

#include "vertex_list.h"

/*----------------------------------------------------------------
MakeVertex: Makes a vertex, nulls out fields.
----------------------------------------------------------------*/ 
tVertex MakeVertex( tVertex vertices )
{
  tVertex v;

  NEW( v, tsVertex );
  ADD( vertices, v );

  return v;
}

/* End of File: vertex_list.c */
