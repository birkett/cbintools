/*
* The MIT License (MIT)
*
* Copyright (c) 2014-2015 Anthony Birkett
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
* Turns binaries into C char arrays.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define H_OUTPUT_FORMAT_STR	"0x%0.2X"
#define D_OUTPUT_FORMAT_STR "%u"

#ifndef WIN32
    #include <ctype.h>
    
    char *_strupr(char *x)
    {
        char *y = x;
        while(*y)
        {
            *y = toupper(*y);
            y++;
        }
        return x;
    }
#endif

	unsigned char * fprintList(FILE *outputFile, unsigned char *bufptr, unsigned char len, const char* formatStr)
	{
		for (unsigned char j = 0; j < len; j++)
		{
			fprintf(outputFile, formatStr, *(bufptr++));
			fprintf(outputFile, ",");
		}
		fprintf(outputFile, "\n");
		return bufptr;
	}

int main(int argc, char *argv[])
{
    FILE          *inputFile = NULL;
    FILE          *outputFile = NULL;
    void          *buffer;
    unsigned int  fileLen, i, major, minor;
    unsigned char *bufptr;
	unsigned char useHex = 0;

    if(argc < 4 || argc > 5) {
        printf("%s %s %s\n", "Usage:", argv[0], "<binary> <destination file> <array name> [options]");
		printf("%s\n", "options:\n\t-h output numbers in hex format");
        return 1;
    }

	useHex = strstr(argv[4], "-h") != NULL;

    inputFile = fopen(argv[1], "rb");
    if(inputFile == NULL)
    {
        printf("%s %s\n", "Unable to open input file:", argv[1]);
        return 1;
    }

    fseek(inputFile, 0, SEEK_END);
    fileLen = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    buffer = calloc(fileLen, 1);

    if (fread(buffer, 1, fileLen, inputFile) != fileLen)
    {
        fclose(inputFile);
        printf("%s %s\n", "Unable to read input file:", argv[1]);
        return 1;
    }

    fclose(inputFile);

    outputFile = fopen(argv[2], "wb");
    if(outputFile == NULL)
    {
        printf("%s %s\n", "Unable to open output file:", argv[2]);
        free(buffer);
        return 1;
    }

    _strupr(argv[3]);

    fprintf(outputFile, "#define %s%s %u\n\n\n", argv[3], "_SIZE", fileLen);
    fprintf(outputFile, "unsigned char %s[] = {\n", argv[3]);

    major = fileLen / 16;
    minor = fileLen % 16;

    bufptr = buffer;
	
    for (i = 0; i < major; i++)
    {
		bufptr = fprintList(outputFile, bufptr, 16, useHex? H_OUTPUT_FORMAT_STR: D_OUTPUT_FORMAT_STR);
    }

    if (minor)
    {
		bufptr = fprintList(outputFile, bufptr, minor, useHex? H_OUTPUT_FORMAT_STR : D_OUTPUT_FORMAT_STR);
    }
    fprintf(outputFile, "};\n");
    fclose(outputFile);
    free(buffer);

    return 0;
}
