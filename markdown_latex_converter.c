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

void convertBold(FILE * fileOut, char * boldStart){
    int pBoldEnd = strcspn(boldStart+2, "**");
    boldStart[pBoldEnd+2] = '\0';
    printf("detected bold");
    fprintf(fileOut, "\\textbf{%s}", boldStart + 2); 
}

void convertItalic(FILE * fileOut, char * italicStart){
    int italicEnd = strcspn(italicStart+1, "*");
    italicStart[italicEnd+1] = '\0';
    printf("detected bold");
    fprintf(fileOut, "\\textbf{%s}", italicStart + 1); 
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
        char *pBold = strstr(buffer, "**");
        char *pCurrent = buffer;
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
            if (pBold != NULL){
                convertBold(pFileOutput, pBold);
            } 
            if (pItalic != NULL){
                convertItalic(pFileOutput, pItalic);
            }
            fprintf(pFileOutput, "%s", buffer);
        }
    }
    closeLatex(pFileOutput);

    fclose(pFileInput);
    fclose(pFileOutput);
    return 0;
}