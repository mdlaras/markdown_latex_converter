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
            convertBold(fileOut, pBoldEnd +2);
        }
        else{
            printf("no bold detected ");
            fprintf(fileOut, "%s", pCurrent);
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
        convertItalic(fileOut, pItalicEnd +1);
    }
    else{
        printf("no italic detected ");
        fprintf(fileOut, "%s", pCurrent);
    }
}

void closeLatex(FILE * File){
    fprintf(File, "\n\\end{document}\n");
}

int main(){
    FILE * pFileInput;
    FILE * pFileOutput;
    char * identifier[2] = {"#", "*"};
    char buffer[1024];

    pFileInput = fopen("test.md", "r");
    pFileOutput = fopen("test.tex", "w");

    initiateLatex(pFileOutput, pFileInput, buffer);

    while(fgets(buffer, 1024, pFileInput) != NULL){
        char *pHeaderOne = strstr(buffer, "#");
        char *pHeaderTwo = strstr(buffer, "##");
        char *pItalic = strstr(buffer, "*");
        // char *pBold = strstr(buffer, "**");
        // char *pCurrent = buffer;
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
            convertBold(pFileOutput, buffer);
            if (pItalic != NULL){
                convertItalic(pFileOutput, buffer);
            }
        }
    }
    closeLatex(pFileOutput);

    fclose(pFileInput);
    fclose(pFileOutput);
    return 0;
}