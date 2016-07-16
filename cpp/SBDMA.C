//  --------------------------------------------------------------
//      filename:  sbdma.c
//	Direct Memory Access for Sound Blaster
//  compiled by :  bcc
//
//  This program is written by Kaiwen Cheng
//  Copyright(C) 1993 Kaiwen Cheng. All Rights Reserved.
//  --------------------------------------------------------------
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "sbdma.h"
#include "def.h"


int     DMAReset(int Channel)
{
    if( Channel > 3 || Channel < 0)
        return -1;

    outportb(DMA_MASK,DMA_RESET_CMD+Channel);
    return  0;
}

void    DMASetup(int Channel,char far *Buffer,unsigned Length,int Dir)
{
    unsigned long physical,tmp;
    unsigned Base;
    BYTE     Page;

    physical=((unsigned long)FP_OFF(Buffer)) + (((unsigned long)FP_SEG(Buffer)) << 4);
    printf("Buf is %p, maps to physical 0x%05l\n",Buffer,physical);
    Base=physical & 0x0000ffffL;
    tmp = ((physical & 0x000f0000L) >> 16);
    Page=(BYTE) tmp;
    printf("Base = 0x%05uX  Page = %uc\n",Base,Page);

    disable();
    outportb(0x0a,0x05);
    outportb(0x0c,0x00);
    outportb(0x0b,0x49);    //  for DAC, outport value 0x45 for ADC
    outportb(0x02,LOBYTE(Base));
    outportb(0x02,HIBYTE(Base));
    outportb(0x83,Page);
    outportb(0x03,LOBYTE(Length-1));
    outportb(0x03,HIBYTE(Length-1));
    outportb(0x0a,1);
    enable();
}

//  DMAComplete returns -1 if complete
//  DMAComplete returns  1 if error
int     DMAComplete(int Channel)
{
    unsigned    bytes;
    BYTE        hi,lo;

    if( Channel > 3 || Channel < 0)
        return 1;

    disable();
    lo=inportb(Channel*2+1);
    hi=inportb(Channel*2+1);
    bytes=(((unsigned)(hi)) << 8) + (unsigned) lo ;
    enable();

    return  bytes;
}
