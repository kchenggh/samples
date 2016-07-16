//  --------------------------------------------------------------
//      filename:  vga4page.c
//	setup VGA as 4 pages 
//  compiled by :  bcc
//
//  This program is written by Kaiwen Cheng
//  Copyright(C) 1993 Kaiwen Cheng. All Rights Reserved.
//  --------------------------------------------------------------

#include <dos.h>
#include <mem.h>
#include <alloc.h>
#include <conio.h>

#include "def.h"
#include "vga4page.h"
#include "pcxb.h"
#include "photo.h"

static  unsigned int ActSegment = DEF_VGA_SEGMENT;
// Current page address

void putpixel4(unsigned int x, unsigned int y, unsigned char color)
{
    //    char far *addr;
    _asm {
    pusha
    //    outport(SC_INDEX, (0x0100 << (x & 3)) | MAP_MASK);  /* Write to only 1 plane */
    mov cx,x
    and cx,03h
    mov ax,0100h
    shl ax,cl
    or  ax,MAP_MASK
    mov dx,SC_INDEX
    out dx,ax
    //    addr =(char *) MK_FP(ActSegment, y * (320/4) + x/4);
    //                                     y * 80        x >> 2
    mov bx,x
    shr bx,02h
    //  bx = x >> 2
    imul    ax,y,50h
    //  ax = 80 * y
    add bx,ax
    //  bx = y * 80 + x >> 2
    mov ax,ActSegment
    mov es,ax
    mov di,bx
    mov al,color
    stosb
    // *addr = color
    popa
    }
}

void VGA320x200x4(void)
{
    union REGS r;

    //  push the CX because some BIOS don't preserve CX
    _asm    push    cx
    r.x.ax = 0x13;
    int86(0x10, &r,&r);         //  initial mode 13h from BIOS
    _asm    pop     cx

    outport(SC_INDEX, 0x604);     //  Disable 'Chain 4' bit from Sequencer reg

    _asm    {
        mov     ax,0100h
        out     dx,ax           //  synchronous reset while setting Misc Output
                                //  for safety, even though clock unchanged
                                //  disable the operation of EGA VGA
    }

    outport(CRTC_INDEX, 0xe317);  //  CRTC -- Select "byte mode"
                                //  and then the VRAM is scaned just as
                                //  in mode 12h
    outport(CRTC_INDEX, 0x0014);  //  Turn off "double word mode"
                                //  allow CRTC to scan the VRAM beyond
                                //  64K
    _asm    {
        mov     dx,SC_INDEX
        mov     ax,0300h
        out     dx,ax           //  undo reset (restart sequencer)
                                //  restart EGA VGA
    }

    //  clear the screen
    outport(SC_INDEX, 0x0f00 | MAP_MASK);
    memset(MK_FP(DEF_VGA_SEGMENT, 0),0, 0xffff);
}

void SetVisiblePage(int page)
{
    outport(CRTC_INDEX, (page << 14) | 0x0c);
    //  set start address register
}

ImagePtr    SetActPage(int page)
{
    ActSegment = DEF_VGA_SEGMENT + (page << 10);
    return (ImagePtr)MK_FP(ActSegment, 0);
}

void        PutImageX(int x,int y,ImagePtr  pbm,int   page)
{
    int         plane;
    int         width,height;
    int         i;
    ImagePtr    scr_addr;
    ImagePtr    tmpPBM=pbm+4;
    ImagePtr    tmpSCR;

    memcpy(&width,pbm,2);
    memcpy(&height,pbm+2,2);

    tmpSCR=scr_addr=SetActPage(page)+y*SCR_WIDTH4+x/4;

    for(plane=0;plane<4;plane++)
    {
        tmpSCR=scr_addr;
        outport(SC_INDEX, (0x0100 << (plane & 3)) | MAP_MASK);
        // select one plane to write
        for(i=0;i<height;i++)
        {
            movedata(FP_SEG(tmpPBM),FP_OFF(tmpPBM),
                     FP_SEG(tmpSCR),FP_OFF(tmpSCR),width/4);
            tmpSCR+=SCR_WIDTH4;
            tmpPBM+=(width/4);
        }
    }
}

