/*****************************************************************************
 *                                                                           *
 *   ELWBIO.C  - Biorhythm charting program       Feb 25, 1988 / D. Arnel    *
 *                                                                           *
 *   Copyright (c) 1988-90  Logicom, inc.         All Rights Reserved        *
 *   Copyright (c) 2004-24  Elwynor Technologies  All Rights Reserved        *
 *                                                                           *
 *   Feb 9, 2006: Conversion to WG 3.2 - R. Hadsall                          *
 *   Jun 1, 2024: Conversion to MBBS10 - R. Hadsall                          *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      * 
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/

#include "gcomm.h"
#include "majorbbs.h"
#include "elwbio.h"

#define VERSION "2.00"

struct biousr *bioptr;        /* ((struct biousr *)vdaptr)                 */

#define BIOOUT  -1            /* biorhythm outputting state                */

#define LOW      19           /* Left (low) side of chart                  */
#define HIGH     66           /* Right (high) side of chart                */
#define CRITICAL 44           /* Middle (critical) of chart                */
#define PHYSC    23           /* Physical cycle                            */
#define SENSC    28           /* Sensativity cycle                         */
#define COGNC    33           /* Cognitive cycle                           */
#define NUMDAYS  30           /* Number of days to output                  */

VOID EXPORT init__elwbio(VOID);
GBOOL biorth(VOID);
VOID bosthn(VOID);
VOID clsbio(VOID);

static VOID errmsg(INT msgn);
static VOID errmsgc(INT msgn, CHAR parm);
VOID biodat(VOID);
VOID bgnbio(VOID);
VOID prfbio(VOID);
INT calcgp(INT v);
LONG dysbth(INT m,INT d,INT y);
INT curwdy(VOID);
LONG longabs(LONG num);

static INT biostt;            /* biorhythm state                           */
struct module biorhythm={     /* module interface block                    */
     "",                      /*    name used to refer to this module      */
     NULL,                    /*    user logon supplemental routine        */
     biorth,                  /*    input routine if selected              */
     bosthn,                  /*    status-input routine if selected       */
     NULL,                    /*    "injoth" routine for this module       */
     NULL,                    /*    user logoff supplemental routine       */
     NULL,                    /*    hangup (lost carrier) routine          */
     NULL,                    /*    midnight cleanup routine               */
     NULL,                    /*    delete-account routine                 */
     clsbio                   /*    finish-up (sys shutdown) routine       */
};

//static
struct biousr {
     INT bday;                /* Day user was born                         */
     INT bmon;                /* Month user was born                       */
     INT byear;               /* Year user was born                        */
     INT biod;                /* Day bio is on                             */
     INT biom;                /* Month bio is on                           */
     INT bioy;                /* Year bio is on                            */
     LONG dsb;                /* Days since birth at start of chart        */
     INT linctr;              /* Number of biorhythm lines output so far   */
};                            /* Pointer to current biorhythm users data   */

static INT newstt;            /* new sub-state for this state              */

static INT accday[12]={       /* accumulative number of days in year       */
     0,31,59,90,120,151,
     181,212,243,273,304,334
};

static INT daysmo[12]={       /* number of days in each month              */
     31,28,31,30,31,30,
     31,31,30,31,30,31
};

static CHAR *daynam[]={
     "Tuesday","Wednesday","Thursday","Friday",
     "Saturday","Sunday","Monday"
};

static CHAR *monnam[]={
     "January","February","March","April","May","June",
     "July","August","September","October","November","December"
};

static
HMCVFILE biomb;               /* Biorhythm message file block pointer      */

struct bgreg {
  char userid[UIDSIZ];
  char modnam[MNMSIZ];
  int  expires;
};

VOID EXPORT init__elwbio()    /* Module initialization routine             */
{

     biomb=opnmsg("elwbio.mcv");

     stzcpy(biorhythm.descrp,gmdnam("ELWBIO.MDF"),MNMSIZ);
     biostt=register_module(&biorhythm);

     shocst(spr("ELW Biorhythm v%s",VERSION),"(C) Copyright 2024 Elwynor Technologies - www.elwynor.com");
}

GBOOL biorth(VOID)            /* Module input routine                      */
{    
     setmbk(biomb);     
     if ((newstt=usrptr->substt) == BIOOUT) {
          return(1);
     }
     else if (usrptr->flags&INJOIP) {
          prfmsg(newstt);
     }
     else if (margc == 1 && sameas(margv[0],"x")) {
          return(0);
     }
     else {
          switch (newstt) {
          case 0:
               prfmsg(newstt=BIOMNU);
               break;
          case BIOMNU:
               if (margc == 1 && margv[0][1] == '\0') {
                    switch (toupper(*margv[0])) {
                    case '\0':
                         prfmsg(BIOMNU);
                         break;
                    case 'I':
                         errmsg(BIOINS);
                         break;
                    case 'C':
                         if (!hasmkey(BIOKEY)) {
                              errmsg(LIVONL);
                         }
                         else {
                              prfmsg(newstt=BRTHDY);
                         }
                         break;
                    default:
                         errmsgc(CNOTIL,*margv[0]);
                    }
               }
               else {
                    errmsg(JSTONE);
               }
               break;
          case BRTHDY:
               if (margc == 0 && !(usrptr->flags&INJOIP)) {
                    prfmsg(newstt=BIOMNU);
               }
               else {
                    biodat();
               }
               break;
          case PRPRBF:
               bgnbio();
               break;
          }
     }
     usrptr->substt=newstt;
     outprf(usrnum);
     return(1);
}

static VOID errmsg(INT msgn)
{
     prfmsg(msgn);
     prfmsg(newstt);
}

static VOID errmsgc(INT msgn,CHAR parm)
{
     prfmsg(msgn,parm);
     prfmsg(newstt);
}

VOID biodat(VOID)
{
     INT mon=0,day=0,year=0,d;

     bioptr=(struct biousr *)alczer(sizeof(struct biousr));
     
     bioptr->biod=ddday(d=today());
     bioptr->biom=ddmon(d);
     bioptr->bioy=ddyear(d);
     if (sscanf(margv[0],"%d/%d/%d",&mon,&day,&year) != 3) { /*(CNTLIT ? 5 : 3)) {*/
          errmsg(INVDAT);
          return;
     }
     year=(year < 100 && year >= bioptr->bioy%100 ? 1800+year : year);
     year=(year < 100 ? 1900+year : year);
     if (bioptr->bioy-year < 1 || bioptr->bioy-year > 99) {
          errmsg(YOUTRG);
     }
     else if (mon < 1 || mon > 12) {
          errmsg(MOUTRG);
     }
     else if (day < 1 || day > daysmo[mon-1]+(mon == 2 && year%4 == 0)) {
          errmsg(DOUTRG);
     }
     else {
          bioptr->bday=day;
          bioptr->bmon=mon;
          bioptr->byear=year;
          prfmsg(newstt=PRPRBF);
     }
}

LONG longabs(LONG num)
{
  if (num < 0)
    return (num*(-1));
  else
    return(num);
}

VOID bgnbio(VOID)
{
     INT wd;
     LONG d,x;

     usrptr->flags|=NOINJO;
     bioptr->linctr=-1;
     d=dysbth(bioptr->biom,bioptr->biod,bioptr->bioy);
     x=dysbth(bioptr->bmon,bioptr->bday,bioptr->byear);
     d=longabs(x-d);
     bioptr->dsb=d;
     wd=(INT)(x-7*(x/7))+1;
     if (wd > 6) {
          wd=0;
     }
     prfmsg(BIOHDR,monnam[bioptr->bmon-1],bioptr->bday,bioptr->byear);
     prfmsg(LNGAGO,daynam[wd],ltoa(d),spr("%ld.%02d",d/365,((d%365)*100)/365));
     prfmsg(HDRPT2);
     btuoes(usrnum,1);
     newstt=BIOOUT;
}

VOID prfbio(VOID)
{
     INT leap,point,phys,sens,cogn;

     leap=(bioptr->biom == 2 && bioptr->bioy%4 == 0);
     if (bioptr->biod > daysmo[bioptr->biom-1]+leap) {
          bioptr->biom=(bioptr->biom+1 > 12 ? 1 : bioptr->biom+1);
          bioptr->biod=1;
          if (bioptr->biom == 1) {
               prfmsg(HPYNWY);
               bioptr->bioy+=1;
          }
     }
     if (curwdy() == 3 && bioptr->biod == 13) {
          prfmsg(FRID13);
     }
     if (bioptr->biom == bioptr->bmon && bioptr->biod == bioptr->bday) {
          prfmsg(HPPYBD,bioptr->bioy-bioptr->byear);
     }
     prf("[33m%.3s  [0;32m%.3s %02d %d[1m    ",daynam[curwdy()],monnam[bioptr->biom-1],
         bioptr->biod,bioptr->bioy);
     phys=calcgp(PHYSC);
     sens=calcgp(SENSC);
     cogn=calcgp(COGNC);
     for (point=LOW ; point < HIGH ; point++) {
          if (point != phys) {
               if (point != sens) {
                    if (point != cogn) {
                         if (point != CRITICAL) {
                              prf(" ");
                         }
                         else {
                              prf("[0;37mI[1m");
                         }
                    }
                    else {
                         prf("[33mC[32m");
                    }
               }
               else {
                    prf("[31mS[32m");
               }
          }
          else {
               prf("[34mP[32m");
          }
     }
     prf("\r");
     bioptr->biod+=1;
}

INT calcgp(INT v)  
/* Calculate point in chart for phys, sens, and cogn cycles */
{
     DOUBLE val,ndays,sin();

     ndays=bioptr->dsb+bioptr->linctr;
     val=ndays/v-(INT)(ndays/v);
     return((INT)(sin(val*6.28318)*18.0)+CRITICAL);
}

LONG dysbth(INT m,INT d,INT y)
{
     INT yr,x1,x2,x3,ly;
     LONG dsb;

     yr=y-1800;
     x1=yr/4;
     x2=x1/25;
     x3=(yr+200)/400;
     ly=1;
     if (x1*4 != yr || x2*100 != yr || x3*400-200 != yr) {
          ly=0;
     }
     dsb=365L*yr+x1-x2+x3-ly;
     dsb+=accday[m-1]+d-1;
     if (m > 2) {
          dsb+=ly;
     }
     if (yr%4 == 0 && m < 3) {
          dsb-=1;
     }
     return(dsb);
}

INT curwdy(VOID)
{
     return(cofdat(dddate(bioptr->biom,bioptr->biod,bioptr->bioy))%7);
}

VOID bosthn(VOID)             /* Module status-input routine               */
{
     setmbk(biomb);
     if (status == OUTMT && usrptr->substt == BIOOUT) {
          if (++(bioptr->linctr) > NUMDAYS-1) {
               usrptr->flags&=~NOINJO;
               btuoes(usrnum,0);
               prfmsg(usrptr->substt=BIOMNU);
          }
          else {
               prfbio();
          }
          outprf(usrnum);
     }
     else {
          dfsthn();
     }
}

VOID clsbio(VOID)             /* BBS shutdown routine                      */
{
     clsmsg(biomb);
}
