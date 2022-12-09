#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Supported commands
// ADD, SUB, AND, OR, XOR, ADDI, ANDI, ORI, SLL, SRL
/*
    cmd  opcode  f3  f7

    tipo R
    ADD  0110011 000 0000000
    SUB  0110011 000 0100000
    AND  0110011 111 0000000
    OR   0110011 110 0000000
    XOR  0110011 100 0000000
    SLL  0110011 001 0000000
    SRL  0110011 101 0000000

    tipo I
    ADDI 0010011 000
    ANDI 0010011 111
    ORI  0010011 110

*/

/*
    formato R
    funct7  rs2   rs1   f3  rd    opcode
    0000000 00000 00000 000 00000 0000000

    formato I
    immediate     rs1   f3  rd    opcode
    000000000000  00000 000 00000 0000000
*/

const char * intToBin(int num, int n) {
    int x, y, z;
    x = num;
    char* result = (char*) malloc(n+1);
    for(z = n-1; z >= 0; z--) {
        y = x >> z;
        if (z == n-1)
        {
            if(y & 1) {
                strcpy(result, "1");
            } else {
                strcpy(result, "0");
            }
        } else {
            if(y & 1) {
                strcat(result, "1");
            } else {
                strcat(result, "0");
            }
        }
    }
    return result;
}

void tipoR(char* f7, char* f3, char* p1, char* p2, char* p3, char ** bin) {
    char result[33];
    strcat(result, f7);
    strcat(result, p3);
    strcat(result, p2);
    strcat(result, f3);
    strcat(result, p1);
    strcat(result, "0110011");
    strcpy(*bin, result);
}

void tipoI(char* f3, char* p1, char* p2, char* p3, char ** bin) {
    char result[33];
    strcat(result, p3);
    strcat(result, p2);
    strcat(result, f3);
    strcat(result, p1);
    strcat(result, "0010011");
    strcpy(*bin, result);
}

void montadorBinario(char* cmd, char* p1, char* p2, char* p3, char** bin) {
    int p1n, p2n, p3n;
    p1n = (int) strtol(p1, (char **)NULL, 10);
    p2n = (int) strtol(p2, (char **)NULL, 10);
    p3n = (int) strtol(p3, (char **)NULL, 10);

    if (!strcmp(cmd, "add")) {
        tipoR("0000000", "000", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 5), bin); 
        return;
    }
    if (!strcmp(cmd, "sub")) {
        tipoR("0100000", "000", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 5), bin); 
        return;
    }
    if (!strcmp(cmd, "and")) {
        tipoR("0000000", "111", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 5), bin); 
        return;
    }
    if (!strcmp(cmd, "or")) {
        tipoR("0000000", "110", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 5), bin); 
        return;
    }
    if (!strcmp(cmd, "xor")) {
        tipoR("0000000", "100", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 5), bin); 
        return;
    }
    if (!strcmp(cmd, "sll")) {
        tipoR("0000000", "001", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 5), bin); 
        return;
    }
    if (!strcmp(cmd, "srl")) {
        tipoR("0000000", "101", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 5), bin); 
        return;
    }
    if (!strcmp(cmd, "addi")) {
        tipoI("000", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 12), bin); 
        return;
    }
    if (!strcmp(cmd, "andi")) {
        tipoI("111", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 12), bin); 
        return;
    }
    if (!strcmp(cmd, "ori")) {
        tipoI("110", intToBin(p1n, 5), intToBin(p2n, 5), intToBin(p3n, 12), bin); 
        return;
    }

}

char* formatRegs(char * str) {
    if (str[strlen(str)-1] == ',')
    {
        str[strlen(str)-1] = '\0';
    }
    if (str[0] == 'x')
    {
        memmove(str, str+1, strlen(str));
    }
    return str;
}

void montarBinario(char * str, char **bin) {
    char* cmds;
    char *cmd, *p1, *p2, *p3;
    int i = 0;
    cmds = strtok(str, " ");
    while (cmds != NULL) {
        if (i == 0) {
            cmd = cmds;
        }
        if (i == 1) {
            p1 = formatRegs(cmds);
        }
        if (i == 2) {
            p2 = formatRegs(cmds);
        }
        if (i == 3) {
            p3 = formatRegs(cmds);        
        }
        i += 1;
        cmds = strtok (NULL, " ");
    }
    montadorBinario(cmd, p1, p2, p3, bin);
}

int main(){
    FILE *in_ptr, *out_ptr;
    char str[50];
    char * bin = (char *) malloc(33);
    char inputfile[50];
    printf("Insira o nome do arquivo: ");
    scanf("%s", inputfile);
    in_ptr = fopen(inputfile, "a+");
    out_ptr = fopen("output.rsm", "w");
    
    if (NULL == in_ptr) {
        printf("input file can't be opened \n");
    }
    if (NULL == out_ptr) {
        printf("output file can't be opened \n");
    }
    while (fgets(str, 50, in_ptr) != NULL) {
        montarBinario(str, &bin);
        fprintf(out_ptr, "%s\n", bin);
    }
    fclose(in_ptr);
    fclose(out_ptr);
    return 0;
}


