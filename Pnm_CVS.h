/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                             Pnm_CVS.h                                *
*                                                                      * 
*       Assignment: arith                                              *
*       Authors: Emily Gjertsson (egjert01) & Elise Kaplan (ekapla04)  *  
*       Date: 10/26/2020                                               *
*                                                                      *
*       Summary                                                        *
*           struct definition of Pnm_CVS, holds component video        *
*           values.                                                    *
*               Y: brightness (between 0 and 1)                        *
*               PB & PR: color-difference signals                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef struct Pnm_CVS {
    long double Y;
    long double PB;
    long double PR;
} Pnm_CVS; 