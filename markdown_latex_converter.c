#include <stdio.h>
#include <string.h>

void initiateLatex(FILE * fileOut, FILE * fileIn, char * buffer){
    fprintf(fileOut, "\\documentclass{article}\n");
    
    fgets(buffer, 1024, fileIn);
    char *pTitle = strstr(buffer, "#");
    int titleEnd = strcspn(buffer, "\n");
    buffer[titleEnd] = '\0';
    if (pTitle != NULL){
        fprintf(fileOut, "\\title{%s}\n", pTitle + 1);
    } else {
        fprintf(fileOut, "\\title{Document}\n");
    }
    fprintf(fileOut, "\\begin{document}\n");
    fprintf(fileOut, "\\maketitle\n");
}

void convertBold(FILE * fileOut, char * buffer){
        char *pBoldStart = strstr(buffer, "**");
        char *pCurrent = buffer;
        if(pBoldStart != NULL){
            char *pBoldEnd = strstr(pBoldStart+2, "**");
            int boldLength = pBoldEnd - pBoldStart -2;
            int boldDistance = pBoldStart - pCurrent;
            char *trimmedBefore = (char *)malloc(boldDistance+1);
            strncpy(trimmedBefore, pCurrent, boldDistance);
            trimmedBefore[boldDistance] = '\0';
            fprintf(fileOut, "%s", trimmedBefore);
            free(trimmedBefore);
            printf("detected bold ");
            char *trimmedBold = (char *)malloc(boldLength + 1);
            strncpy(trimmedBold, pBoldStart+2, boldLength);
            trimmedBold[boldLength] = '\0';
            fprintf(fileOut, "\\textbf{%s}", trimmedBold);
            free(trimmedBold);
        }
}

void convertItalic(FILE * fileOut, char * buffer){
    char *pItalicStart = strstr(buffer, "*");
    char *pCurrent = buffer;
    if(pItalicStart != NULL){
        char *pItalicEnd = strstr(pItalicStart+1, "*");
        int italicLength = pItalicEnd - pItalicStart - 1;
        int italicDistance = pItalicStart - pCurrent;
        char *trimmedBefore = (char *)malloc(italicDistance + 1);
        strncpy(trimmedBefore, pCurrent, italicDistance);
        trimmedBefore[italicDistance] = '\0';
        fprintf(fileOut, "%s", trimmedBefore);
        free(trimmedBefore);
        printf("detected italic ");
        char *trimmedItalic = (char *)malloc(italicLength + 1);
        strncpy(trimmedItalic, pItalicStart+1, italicLength);
        trimmedItalic[italicLength] = '\0';
        fprintf(fileOut, "\\textit{%s}", trimmedItalic);
        free(trimmedItalic);
    }
}

void convertStyle(FILE *fileOut, char * buffer){
    char *pItalicStart = strstr(buffer, "*");
    char *pBoldStart = strstr(buffer, "**");
    char *pCurrent = buffer;
    if (pBoldStart != NULL){
        if (pItalicStart != NULL && pBoldStart < pItalicStart){
        char *pBoldEnd = strstr(pBoldStart+2, "**");
        convertBold(fileOut, pCurrent);
        convertStyle(fileOut, pBoldEnd + 2);
        } else if (pItalicStart != NULL && pBoldStart > pItalicStart){
            char *pItalicEnd = strstr(pItalicStart+1, "*");
            convertItalic(fileOut, pCurrent);
            convertStyle(fileOut, pItalicEnd +1);
        } else {
            char *pBoldEnd = strstr(pBoldStart+2, "**");
            convertBold(fileOut, pCurrent);
            convertStyle(fileOut, pBoldEnd + 2);
        }
    } else if (pItalicStart != NULL){
        char *pItalicEnd = strstr(pItalicStart+1, "*");
        convertItalic(fileOut, pCurrent);
        convertStyle(fileOut, pItalicEnd +1);
    } else {
        printf("no style detected ");
        fprintf(fileOut, "%s", pCurrent);
    }
    
}

void closeLatex(FILE * File){
    fprintf(File, "\n\\end{document}\n");
}

void generateLatex(char * inName, char * outName){

    FILE * pFileInput = fopen(inName, "r");
    FILE * pFileOutput = fopen(outName, "w");

    char buffer[1024];

    initiateLatex(pFileOutput, pFileInput, buffer);

    while(fgets(buffer, 1024, pFileInput) != NULL){
        char *pHeaderOne = strstr(buffer, "#");
        char *pHeaderTwo = strstr(buffer, "##");
        int lineEnd = strcspn(buffer, "\n");
        if(lineEnd < 1024){
            buffer[lineEnd] = '\0';
        }
        
        if (pHeaderTwo != NULL){
            fprintf(pFileOutput, "\\subsection{%s}\n", pHeaderTwo + 2);
        }
        else if (pHeaderOne != NULL){
            fprintf(pFileOutput, "\\section{%s}\n", pHeaderOne+1);
        } else {
            convertStyle(pFileOutput, buffer);
        }
    }

    closeLatex(pFileOutput);

    fclose(pFileInput);
    fclose(pFileOutput);
}

int main(){
    char * identifier[2] = {"#", "*"};
    
    printf("Enter the markdown file to convert\n");
    char fileName[256];
    scanf("%s", fileName);
    printf("Enter output name\n");
    char outName[256];
    scanf("%s", outName);
    strcat(outName, ".tex");

    generateLatex(fileName, outName);
 
    return 0;
}