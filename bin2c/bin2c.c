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

#define H_OUTPUT_FORMAT_STR "0x%0.2X"
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

int usage(char * exe)
{
    printf("%s %s %s\n", "Usage:", exe, "[-h] <source_bynary_file> <dest_header_file> <array_name>");
    printf("%s\n", "\t-h                  : [Optional] Output in hexadecimal format. The default format is decimal.");
    printf("%s\n", "\t<source_bynary_file>: Path to the binary file to convert to C char array.");
    printf("%s\n", "\t<dest_header_file>  : Path and name of the C header output file to generate.");
    printf("%s\n", "\t<array_name>        : Name of the C array to generate from source_bynary_file.");
    return 1;
}

unsigned char * fprintList(FILE *outputFile, unsigned char *bufptr, unsigned char len, const char* formatStr)
{
	unsigned char j;
	for (j = 0; j < len; j++)
	{
		fprintf(outputFile, formatStr, *(bufptr++));
		fprintf(outputFile, ",");
	}
	fprintf(outputFile, "\n");
	return bufptr;
}

int main(int argc, char *argv[])
{
    const char      *outputFormatStr = D_OUTPUT_FORMAT_STR;
    FILE            *inputFile = NULL;
    FILE            *outputFile = NULL;
    void            *buffer;
    unsigned int    i = 0;
    unsigned int    major;
    unsigned int    minor;
    unsigned int    fileLen;
    unsigned char   *bufptr;
    unsigned char   *arrayName;
    unsigned char   *inputFilePath;
    unsigned char   *outputFilePath;

    if(argc < 4 || argc > 5) 
    {
        return usage(argv[0]);
    }

    if (argc==5)
    {
        outputFormatStr = H_OUTPUT_FORMAT_STR;
        if(strcmp(argv[1], "-h") == 0)
        {
            i = 1;
        }
        else if(strcmp(argv[4], "-h") != 0)
        {
             return usage(argv[0]);
        }
    }

    inputFilePath   = argv[i+1];
    outputFilePath  = argv[i+2];
    arrayName       = argv[i+3];

    inputFile = fopen(inputFilePath, "rb");
    if(inputFile == NULL)
    {
        printf("%s %s\n", "Unable to open input file:", inputFilePath);
        return 1;
    }

    fseek(inputFile, 0, SEEK_END);
    fileLen = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    buffer = calloc(fileLen, 1);

    if (fread(buffer, 1, fileLen, inputFile) != fileLen)
    {
        fclose(inputFile);
        free(buffer);
        printf("%s %s\n", "Unable to read input file:", argv[1]);
        return 1;
    }

    fclose(inputFile);

    outputFile = fopen(outputFilePath, "wb");
    if(outputFile == NULL)
    {
        printf("%s %s\n", "Unable to open output file:", outputFilePath);
        free(buffer);
        return 1;
    }

    _strupr(arrayName);

    fprintf(outputFile, "#define %s%s %u\n\n\n", arrayName, "_SIZE", fileLen);
    fprintf(outputFile, "unsigned char %s[] = {\n", arrayName);

    major = fileLen / 16;
    minor = fileLen % 16;

    bufptr = buffer;

    for (i = 0; i < major; i++)
    {
        bufptr = fprintList(outputFile, bufptr, 16, outputFormatStr);
    }

    if (minor)
    {
        bufptr = fprintList(outputFile, bufptr, minor, outputFormatStr);
    }
    fprintf(outputFile, "};\n");
    fclose(outputFile);
    free(buffer);

    return 0;
}
