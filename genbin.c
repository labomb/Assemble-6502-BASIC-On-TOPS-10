/*

 genbin - Generate an 8-bit binary from a DEC Macro-10 36-bit .REL file
 
 Use the M6502.UNV universal file with your MACRO-10 source code to ensure that the
 assembly .REL file is formatted properly for use with this utility. To do so, simply
 add the following line to the MACRO-10 source:
 
 SEARCH	M6502
 
 
 Note:
 
 If transferring the REL file to a Windows or Linux host using Kermit, you must set
 the byte size to 36-bit on the PDP side, and 8-bit on the PC side. For example, if
 using TOPS-10 KERMIT version 3(136), enter 'set file byte-size 36-bit' prior to
 initiating the transfer.
 
 
 Scott LaBombard
 labomb@rochester.rr.com
 01/06/2014
 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{

  printf("\n *******************************************************\n");
  printf("       DEC MACRO-10 .REL to 8-Bit Binary Converter\n");
  printf("             Scott LaBombard - Jan. 6, 2014\n");
  printf(" *******************************************************\n\n");

  if(argc < 2) {
    printf("\nYou must supply the name of a MACRO-10 .REL binary file to process.\n");
    return 1;
  }

  FILE *in_file, *out_file;
  char buf[64] = "";
  char * tmp;
  const char srchChars[] = {0x00,0x20,0x00,0x00};
  size_t srchLen = 4;
  int c = 0, gBin = 0;

  in_file = fopen(argv[1],"rb");
  if(!in_file) {
    printf("\nProblem opening input file %s !\n",argv[1]);
    return 1;
  }

  tmp = strrchr(argv[1],'.');
  strncat(buf,argv[1],tmp-argv[1]);
  strcat(buf,".bin");
  out_file = fopen(buf,"wb");

  if(!out_file) {
    printf("\nProblem opening output file %s !\n",buf);
    return 1;
  }
  
  printf("Writing binary file... ");

  while( (fread(tmp, 1, srchLen, in_file)) == srchLen ) {	/* get first 4 bytes of a record */
    for(c=0; c < srchLen; c++) {
      if(tmp[c] != srchChars[c])				/* compare to check if it's valid */
        break;
    }
    if(c == srchLen) {							/* valid if all signature bytes found */
      printf("found signature at offset $%04X... ",ftell(in_file)-srchLen);
      while( (c = fgetc(in_file)) != EOF ) {	/* file pointer now at byte to process */
        if(c & 0x80)							/* rotate left 1-bit (with carry if required) */
          fputc(((c << 1) | 0x01),out_file);	/* and write the result to the output file */
        else
          fputc((c << 1),out_file);
        fseek(in_file, 4, SEEK_CUR);		/* next byte (process every 5th byte from now on) */
      }
      gBin=1;								/* set signature found flag */
      break;
    }
    else									/* not a valid signature ... so ... */
      fseek(in_file,1,SEEK_CUR);			/* skip 5th byte of record and point to next */
  }

  if(!gBin)									/* REL file doesn't meet requirements */
    printf("signature not found...");

  fclose(in_file);
  fclose(out_file);
  printf("done!\n");
  return 0;

}
