/*
 * g++ -Wall djvufixdpi.cpp -o djvufixdpi -s
 *
   Copyright 2003, Leon Bottou

   Permission to use, copy, modify, distribute, and sell this software and its
   documentation for any purpose is hereby granted without fee, provided that
   the above copyright notice appear in all copies and that both that
   copyright notice and this permission notice appear in supporting
   documentation.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
   THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
   IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// modified by monday2000@yandex.ru to change DPI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

//int maxversion = 22;
char *curfile = 0;

int new_dpi;

void
die(const char *s)
{
    fprintf(stderr,"djvudpi: %s", s);
    if (curfile)
        fprintf(stderr,": %s", curfile);
    if (errno > 0)
        perror(": ");
    fprintf(stderr,"\n");
    exit(10);
}

int
read8(FILE *f, int offset)
{
    unsigned char r[1];
    if (fseek(f, offset, SEEK_SET) < 0)
        die("Cannot seek");
    if (fread(r,1,1,f) != 1)
        die("Cannot read");
    return r[0];
}

void
write8(FILE *f, int offset, int val)
{
    unsigned char r[1];
    r[0] = val;
    if (fseek(f, offset, SEEK_SET) < 0)
        die("Cannot seek");
    if (fwrite(r,1,1,f) != 1)
        die("Cannot write");
}

long
read32(FILE *f, int offset)
{
    //long x;
    unsigned char r[4];
    fseek(f, offset, SEEK_SET);
    if (fread(r,1,4,f) != 4)
        die("Premature end of file");
    return ((((((r[0]<<8)|r[1])<<8)|r[2])<<8)|r[3]);
}

void processN(FILE *f, long offset, long end);

void
process1(FILE *f, long chkid, int offset, int end)
{
    if (chkid == 0x464f524d) /* FORM */
    {
        long formid = read32(f, offset);
        if  (formid == 0x444a5655 ||  /* DJVU */
                formid == 0x444a564d  )  /* DJVM */
            processN(f,offset+4, end);
    }
    else if (chkid == 0x494e464f)  /* INFO */
    {
        int dlo = read8(f, offset+6);
        int dhi = read8(f, offset+7);
        int dpi_old = (dhi<<8)|dlo;

        printf("%s: converting DPI %d to %d\n",
               curfile, dpi_old, new_dpi);

        write8(f, offset+6, new_dpi & 0xff);
        write8(f, offset+7, (new_dpi>>8) & 0xff);
    }
}

void
processN(FILE *f, long offset, long end)
{
    while (offset < end)
    {
        long chkid = read32(f,offset);
        if (chkid == 0x41542654) /* AT&T */
        {
            offset += 4;
        }
        else
        {
            long chklen = read32(f,offset+4);
            long chkend = offset+8+chklen;
            process1(f, chkid, offset+8, chkend);
            offset = chkend;
        }
        if (offset & 1)
            offset += 1;
    }
}

int
main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr,
                "Usage: %s djvufile <n>\n"
                "Change the DPI value in one bundled DjVu (single- or multyple-paged) file to <n>.\n",
                argv[0]);
        exit(10);
    }


    new_dpi = atoi(argv[2]);

    if ((new_dpi < 25) ||  (new_dpi > 2400))
    {
        printf("The new DPI value must be between 25 and 2400\n");

        return -1;
    }

    FILE *f;
    curfile = argv[1];
    new_dpi = atoi(argv[2]);
    if (! (f = fopen(curfile,"rb+")))
        die("Cannot open djvu file.");
    if (fseek(f,0,SEEK_END) < 0)
        die("Cannot seek through file.");
    processN(f, 0, ftell(f));
    fclose(f);

    return 0;
}
