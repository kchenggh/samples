//  --------------------------------------------------------------
//      filename:  palette.c
//  compiled by :
//                 bcc -mc -c palette.c
//                 cl  /AC /c palette.c
//
//  This program is written by Kaiwen Cheng
//  Copyright(C) 1993 Kaiwen Cheng. All Rights Reserved.
//  --------------------------------------------------------------

#include <dos.h>
#ifndef _MSC_VER
    #include <alloc.h>
#else
    #include <malloc.h>
#endif
#include <mem.h>
#include <stdio.h>
#include <conio.h>
#include "palette.h"

#ifdef  _MSC_VER
    #include "bc2msc.h"
#endif

unsigned char * read_PCX_palette(char  *filename)
{
    FILE *fp;
    unsigned j;
    unsigned char * color;

    color=(char *)malloc(PALETTE_SIZE);

    while((fp=fopen(filename,"rb")) == NULL)
        return NULL;

    if(!fseek(fp,-769L,SEEK_END)) {
        if(!(fgetc(fp) == 0x0c && fread(color,1,PALETTE_SIZE,fp) == PALETTE_SIZE))
            return NULL;
    }
    for(j=0;j<PALETTE_SIZE;j++)
        color[j]>>=2;
    fclose(fp);
    return color;
}

void set_palette(unsigned char *PalBufferAddress) {

    #ifdef  NORMAL
    // BIOS CALL version
    union REGS r;
    struct SREGS sr;

    WaitVert();
    r.x.dx=FP_OFF(PalBufferAddress);
    sr.es=FP_SEG(PalBufferAddress);

    r.x.ax=0x1012;
    r.x.bx=0;
    r.x.cx=256;
    int86x(0x10,&r,&r,&sr);
    #endif

    #ifdef  FAST
    int register i;
    WaitVert();
    outportb(PEL_ADD,0x00);
    for(i=0;i<768;i++) 
        outportb(PEL_DATA,*(PalBufferAddress++));
    #endif

    #ifdef      FASTEST
    WaitVert();

    asm    {
            push    ds

            mov     al,00h
            mov     dx,PEL_ADD
            out     dx,al

            mov     bx,WORD PTR [PalBufferAddress]
            mov     ax,WORD PTR [PalBufferAddress+2]

            mov     ds,ax

            mov     cx,0300h
            mov     dx,PEL_DATA
    }
    pal_loop:
    asm {
            mov     ax,[bx]
            out     dx,al
            inc     bx
            loop    pal_loop
        }
    asm     pop     ds
    #endif
}

void set_palette_sets(unsigned char start,int NoOfColRegs
                     ,unsigned char *PalBufferAddress)
{

    #ifdef  NORMAL
    //  BIOS CALL version
    union REGS r;
    struct SREGS sr;

    WaitVert();
    r.x.dx=FP_OFF(PalBufferAddress);
    sr.es=FP_SEG(PalBufferAddress);

    r.x.ax=0x1012;
    r.x.bx=start;
    r.x.cx=NoOfColRegs/3;
    int86x(0x10,&r,&r,&sr);
    #endif

    #ifdef  FAST
    int register i;
    WaitVert();
    outportb(PEL_ADD,start);
    for(i=0;i<NoOfColRegs;i++)
        outportb(PEL_DATA,*(PalBufferAddress++));
    #endif

    #ifdef      FASTEST
    WaitVert();

    asm    {
        push    ds

        mov     al,start
        mov     dx,PEL_ADD
        out     dx,al
        //      outportb(PEL_ADD,start);

        mov     bx,WORD PTR [PalBufferAddress]
        mov     ax,WORD PTR [PalBufferAddress+2]

        mov     ds,ax
        mov     cx,NoOfColRegs
        mov     dx,PEL_DATA
    }
    pal_loop:
    asm {
            mov     ax,[bx]
            out     dx,al
            inc     bx
            loop    pal_loop
        }
    asm     pop     ds
    #endif
}

//      direction =  1 light up 
//      direction = -1 fade out 
void fade_out(char *colors,int direction)
{
    int register i,j;
    unsigned char *tmp_pal;
    PalBuf  *range;
    PalBuf  *interval;

    tmp_pal=(unsigned char *) malloc(PALETTE_SIZE);
    range=(PalBuf *) malloc(sizeof(PalBuf)*PALETTE_SIZE);
    interval=(PalBuf *) malloc(sizeof(PalBuf)*PALETTE_SIZE);

    movedata(FP_SEG(colors),FP_OFF(colors),FP_SEG(tmp_pal),
             FP_OFF(tmp_pal),PALETTE_SIZE);

    if( direction == -1 )
    {
        for(j=0;j<PALETTE_SIZE;j++)
        {
            range[j].ch[1]=*(tmp_pal+j);
            range[j].ch[0]=0;
            interval[j].pal=direction*range[j].pal/FADEOUT_STEPS;
        }
    }   else
    {
        for(j=0;j<PALETTE_SIZE;j++)
        {
            range[j].ch[1]=(char)(63)-*(tmp_pal+j);
            range[j].ch[0]=0;
            interval[j].pal=direction*range[j].pal/FADEOUT_STEPS;
            range[j].ch[1]=tmp_pal[j];
        }
    }
    //  -------------------------------------------------

    for(j=0;j<FADEOUT_STEPS;j++) {
        for(i=0;i<PALETTE_SIZE;i++)
        {
            range[i].pal+=interval[i].pal;
            tmp_pal[i]=range[i].ch[1];
        }
        set_palette(tmp_pal);
    }

    if( direction == -1 )
        memset(tmp_pal,0,PALETTE_SIZE);
    else
        memset(tmp_pal,63,PALETTE_SIZE);
    set_palette(tmp_pal);

    free(range);
    free(interval);
    free(tmp_pal);
}

//      direction =  1 forward
//      direction = -1 backward
void cycle_palette(unsigned char start,int NoOfSets,
                    char *colors,int direction )
{
    unsigned char *RGBSets;
    unsigned char RGB1[3];
    int size;
    unsigned char *start_col_add=colors+start*3;

    size=(NoOfSets-1)*3;

    RGBSets=(unsigned char *) malloc(size);
    
    if( direction == -1)    // backward
    {
        // get origin color arrangment
        memcpy(RGBSets,start_col_add,size);
        memcpy(RGB1,start_col_add+size,3);
        // rearrangment colors
        memcpy(start_col_add+3,RGBSets,size);
        memcpy(start_col_add,RGB1,3);
    }   else    // forward
    {
        // get origin color arrangment
        memcpy(RGBSets,start_col_add+3,size);
        memcpy(RGB1,start_col_add,3);
        // rearrangment colors
        memcpy(start_col_add,RGBSets,size);
        memcpy(start_col_add+size,RGB1,3);
    }
    set_palette_sets(start,size+3,start_col_add);

    free(RGBSets);
}

void WaitVert(void) 
{
    #ifdef      FAST

    while (( inportb(M6845_STA) & VERT_RETR ) == 1);

    while (( inportb(M6845_STA) & VERT_RETR ) == 0);
                                                    
    #endif

    #ifdef      FASTEST
    asm     mov     dx,M6845_STA
    wvrt1:
            asm {
                 in      al,dx
                 test    al,VERT_RETR
                 jnz     wvrt1
            }
    wvrt2:
            asm {
                 in      al,dx
                 test    al,VERT_RETR
                 jz      wvrt2
            }
    #endif
}

