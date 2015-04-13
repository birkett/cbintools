#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define BUFFER_SIZE 80

int main(int argc, char* argv[])
{
    FILE* inputFile = NULL;
    FILE* outputFile = NULL;
    char sBuffer[BUFFER_SIZE];
    char* pch;

    if (argc != 3)
    {
        printf("%s %s %s\n", "Usage:", argv[0], "<header> <output>");
        return 1;
    }

    inputFile = fopen(argv[1], "r");

    if (inputFile == NULL)
    {
        printf("%s\n", "Unable to open input header");
        return 1;
    }

    outputFile = fopen(argv[2], "wb");

    if (outputFile == NULL)
    {
        printf("%s\n", "Unable to open output file");
        return 1;
    }

    // Skip the first 4 lines.
    for (int i = 0; i < 4; i++)
    {
        fgets(sBuffer, BUFFER_SIZE, inputFile);
    }

    // Get the contents of each line of the array.
    while (fgets(sBuffer, BUFFER_SIZE, inputFile))
    {
        // Get rid of the new line character.
        char* newline = strchr(sBuffer, '\n');
        if (newline)
        {
            *newline = 0;
        }

        // Skip this line if its the closing "};".
        char* endBracket = strchr(sBuffer, '}');
        if (endBracket)
        {
            continue;
        }

        // Write out each character.
        pch = strtok(sBuffer, ",");
        while (pch != NULL)
        {
            // Use atoi() to convert from decimal to a char.
            fprintf(outputFile, "%c", atoi(pch));
            //printf("%s\n", pch);
            pch = strtok(NULL, ",");
        }
    }

    return 0;
}