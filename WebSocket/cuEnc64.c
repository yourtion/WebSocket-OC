/*
 * Copyright (c) 1998-2003 Apple Computer, Inc. All Rights Reserved.
 * 
 * The contents of this file constitute Original Code as defined in and are
 * subject to the Apple Public Source License Version 1.2 (the 'License').
 * You may not use this file except in compliance with the License. Please 
 * obtain a copy of the License at http://www.apple.com/publicsource and 
 * read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER 
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES, 
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT. 
 * Please see the License for the specific language governing rights and 
 * limitations under the License.
 *
 * cuEnc64.c - encode/decode in 64-char IA5 format, per RFC 1421
 */

#include "cuEnc64.h"

#include <stdlib.h>

/*
 * map a 6-bit binary value to a printable character.
 */
static const
unsigned char bintoasc[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * map 6 bits to a printing char
 */
#define ENC(c) (bintoasc[((c) & 0x3f)])

#define PAD		'='

/*
 * map one group of up to 3 bytes at inp to 4 bytes at outp.
 * Count is number of valid bytes in *inp; if less than 3, the
 * 1 or two extras must be zeros.
 */
static void encChunk(const unsigned char *inp,
	unsigned char *outp,
	int count)
{
	unsigned char c1, c2, c3, c4;

	c1 = *inp >> 2;
	c2 = ((inp[0] << 4) & 0x30) | ((inp[1] >> 4) & 0xf);
	c3 = ((inp[1] << 2) & 0x3c) | ((inp[2] >> 6) & 0x3);
	c4 = inp[2] & 0x3f;
	*outp++ = ENC(c1);
	*outp++ = ENC(c2);
	if (count == 1) {
	    *outp++ = PAD;
	    *outp   = PAD;
	} else {
	    *outp++ = ENC(c3);
	    if (count == 2) {
		*outp = PAD;
	    }
	    else {
		*outp = ENC(c4);
	    }
	}
}

/*
 * Given input buffer inbuf, length inlen, encode to 64-char IA5 format.
 * Result is fmalloc'd and returned; it is terminated by Microsoft-style
 * newline and NULL. Its length (including the trailing newline and NULL)
 * is returned in *outlen.
 */

unsigned char *cuEnc64(const unsigned char *inbuf,
	unsigned inlen,
	unsigned *outlen)		// RETURNED
{
	return cuEnc64WithLines(inbuf, inlen, 0, outlen);
}

unsigned char *cuEnc64WithLines(const unsigned char *inbuf,
	unsigned inlen,
	unsigned linelen,
	unsigned *outlen)
{
	unsigned		outTextLen;
	unsigned 		len;			// to malloc, liberal
	unsigned		olen = 0;		// actual output size
	unsigned char 	*outbuf;
	unsigned char 	endbuf[3];
	unsigned		i;
	unsigned char 	*outp;
	unsigned		numLines;
	unsigned		thisLine;

	outTextLen = ((inlen + 2) / 3) * 4;
	if(linelen) {
	    /*
	     * linelen must be 0 mod 4 for this to work; round up...
	     */
	    if((linelen & 0x03) != 0) {
	        linelen = (linelen + 3) & 0xfffffffc;
	    }
	    numLines = (outTextLen + linelen - 1)/ linelen;
	}
	else {
	    numLines = 1;
	}

	/*
	 * Total output size = encoded text size plus one newline per
	 * line of output, plus trailing NULL. We always generate newlines 
	 * as \n; when decoding, we tolerate \r\n (Microsoft) or \n.
	 */
	len = outTextLen + (2 * numLines) + 1;
	outbuf = (unsigned char*)malloc(len);
	outp = outbuf;
	thisLine = 0;

	while(inlen) {
	    if(inlen < 3) {
			for(i=0; i<3; i++) {
				if(i < inlen) {
					endbuf[i] = inbuf[i];
				}
				else {
					endbuf[i] = 0;
				}
			}
			encChunk(endbuf, outp, inlen);
			inlen = 0;
	    }
	    else {
			encChunk(inbuf, outp, 3);
			inlen -= 3;
			inbuf += 3;
	    }
	    outp += 4;
	    thisLine += 4;
	    olen += 4;
	    if((linelen != 0) && (thisLine >= linelen) && inlen) {
	        /*
			 * last trailing newline added below
			 * Note we don't split 4-byte output chunks over newlines
			 */
	    	*outp++ = '\n';
			olen++;
			thisLine = 0;
	    }
	}
	*outp++ = '\n';
	*outp = '\0';
	olen += 2;
	*outlen = olen;
	return outbuf;
}


