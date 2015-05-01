/*$Header: /usr8/web/src/RCS/mircol.c,v 1.5 1995/08/21 15:51:59 mladjadj Exp $*/
/*************************************************************************
*									 *
* Transfered from mircol.for by Jing Su 8/30/93	
*
* WARNING: THIS is not suitable for general matrix inversion due to
*          assignment of array dimension 4x5 in parameters!!!!!!!!!!!
*									 *
*************************************************************************/

#include "std.h"

/*** willsq.c: if ((ier = mircol(4,1,5,sqa,eps,r)) != NULL ) call!! */

/******************************** mircol ******************************/

int mircol(int n, int m, int mm, float a[4][5], float eps, float x[])
{
int   i,ii,iii,j,jjj,k,kkk;
int   wurz[6];
float epsq,s;

epsq = eps * eps;

for (i = 1; i <= n; i++)
    {
    wurz[i-1] = TRUE;
    s = a[i-1][i-1];
    if (i != 1)
       {
       iii = i -1;
       for (j = 1; j <= iii; j++)
           {
           if (!wurz[j-1])
	      s = s + a[j-1][i-1] * a[j-1][i-1];
	   else
	      s = s - a[j-1][i-1] * a[j-1][i-1];
           }
       }
     if (s <= 0)
        {
        s = -s;
	wurz[i-1] = FALSE;
        }
     if (s < epsq)
     	return -1;
     
     a[i-1][i-1] = sqrt(s);
     iii         = i+1;

     for (k = iii; k <= mm; k++)
         { 
	 s   = a[i-1][k-1];
	 jjj = i-1;

	 if (jjj >= 1)
	       {
	       for (j = 1; j <= jjj; j++)
	           {
		   if (!wurz[j-1])
		      s = (s + a[j-1][i-1] * a[j-1][k-1]);
		   else
		      s = (s - a[j-1][i-1] * a[j-1][k-1]);
	           }
	       }

	 if (!wurz[i-1])
	    s = -s;

	 a[i-1][k-1] = s / a[i-1][i-1];
         } 
    }

for (k = 1; k <= m; k++)
    { 
    for ( ii = 1; ii <= n; ii++)
        {
	i   = n - ii + 1;
	s   = a[i-1][n+k-1];
	kkk = i+1;
	if ( kkk <= n)
	   {
	   for (j = kkk; j <= n; j++)
		s = s - x[j-1] * a[i-1][j-1];   
	   }
	x[i-1] = s / a[i-1][i-1];
        } 
    }
return 0;
}
