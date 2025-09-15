/*

 genbinl - Generate an 8-bit binary from a DEC Macro-10 Output Listing
 
 Use the M6502L.UNV universal file with your MACRO-10 source code to ensure that the
 assembly .LST file is formatted properly for use with this utility. To do so, simply
 add the following line to the MACRO-10 source:
 
 SEARCH	M6502L
 
 
 Note:
 
 If transferring the LST file to a Windows or Linux host using Kermit, you must set
 the byte size to 36-bit on the PDP side, and 8-bit on the PC side. For example, if
 using TOPS-10 KERMIT version 3(136), enter 'set file byte-size 36-bit' prior to
 initiating the transfer.
 
 
 Scott LaBombard
 labomb@rochester.rr.com
 12/19/2013
 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0L
#endif /* NULL */

int
main(int argc, char **argv)
{

  int
  valid_line(char * inpbuf) {
    int i;
    for( i=1; i < 7; i++ )
      if( (inpbuf[i] < 0x30) || (inpbuf[i] > 0x37) )
        return 0;
    for( i=8; i < 14; i++ )
      if( inpbuf[i] != 0x30 )
        if( (inpbuf[i] != 0x31) && (i == 10) )
          return 0;
    for( i=15; i < 21; i++ )
      if( (inpbuf[i] < 0x30) || (inpbuf[i] > 0x37) )
        return 0;
    return 1;
  }

  printf("\n ********************************************\n");
  printf("       DEC MACRO-10 Listing Binary Generator\n");
  printf("          Scott LaBombard - Dec. 19, 2013\n");
  printf(" ********************************************\n\n");

  if(argc < 2) {
    printf("\nYou must supply the name of a MACRO-10 .LST file to process.\n");
    return 1;
  }

  FILE *in_file, *out_file;
  char buf[512] = "";
  char * tmp;
  int gotromln=0;
  unsigned long ul;

  in_file = fopen(argv[1],"r");
  if (!in_file) {
    printf("\nProblem opening input file %s !\n",argv[1]);
    return 1;
  }

  tmp = strrchr(argv[1],'.');
  strncat(buf,argv[1],tmp-argv[1]);
  strcat(buf,".bin");
  out_file = fopen(buf,"wb");

  if (!out_file) {
    printf("\nProblem opening output file %s !\n",buf);
    return 1;
  }
  
  printf("Writing binary file... ");

  while( fgets(buf,512,in_file) != NULL ) {
    if(!gotromln) {		  /* wait for actual code */
      if( strstr(buf,"	ORG	ROMLOC") != NULL )
        gotromln=1;
	}
    else {
      if( valid_line((char *)buf) ) {
        tmp = buf+18;        /* point to octal opcode */
        ul = strtoul(tmp, NULL, 8);
        fputc(ul, out_file);
      }
      else {
        if( (tmp = strstr(buf,"BLOCK")) != NULL ) {
          if( (buf[1] != 0x09) && (buf[8] == 0x09) && (buf[9] == 0x09) ) {
            tmp = tmp+6;
            if( strncmp(tmp,"^",1) == 0 ) {
              tmp = tmp+2;       /* skip '^O' prefix */
              ul = strtoul(tmp, NULL, 8);
            }
            else {
              ul = strtoul(tmp, NULL, 10);
            }
            while(ul-- != 0)
              fputc(0x00, out_file);
          }
        }
      }
    }
  }

  fclose(in_file);
  fclose(out_file);
  printf("done!\n");
  return 0;
  
}
