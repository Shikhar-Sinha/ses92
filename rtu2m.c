
/*
* SES92 for Messages from RTU to Master
*
*/
#include <hammer/hammer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main (int argc, char *argv[]) {
    //char buff[6];
    //uint  
    uint8_t buf[4];
    memset(&buf[0], 35, sizeof(uint8_t));
    memset(&buf[1], 23, sizeof(uint8_t));
    memset(&buf[2], 45, sizeof(uint8_t));
    memset(&buf[3], 254, sizeof(uint8_t));
/*
    printf("%d\n", buf[0]);
    printf("%d\n", buf[1]);
    printf("%d\n", buf[2]);
    printf("%d\n", buf[3]);
*/

    HParser *b1 = h_uint8();
    HParser *b2 = h_uint8();
    HParser *b3 = h_uint8();
    //HParser *b4 = h_uint8();
    HParser *f1 = h_bits(1, false);
    HParser *f2 = h_bits(1, false);
    HParser *f3 = h_bits(1, false);
    HParser *f4 = h_bits(1, false);
    HParser *f5 = h_bits(1, false);
    HParser *f6 = h_bits(1, false);
    HParser *f7 = h_bits(1, false);
    HParser *f8 = h_bits(1, false);

    HParser *RTU2M = h_sequence(b1, b2, b3, f1, f2, f3, f4, f5, f6, f7, f8, NULL);
    HParseResult *result = h_parse(RTU2M, buf, 4);
    //HParseResult *result = h_parse(b1, buf, 1);
    //printf("%d\n", result->ast->seq->elements[1]->uint);
    printf("The RTU Identifier Byte is %d\n", result->ast->seq->elements[0]->uint);
    printf("The RTU Group Address Byte is %d\n", result->ast->seq->elements[1]->uint);
    printf("The Frame Length is %d\n", result->ast->seq->elements[2]->uint);
    printf("The FIN flag is %d\n", result->ast->seq->elements[3]->uint);
    printf("The ALE Flag is %d\n", result->ast->seq->elements[4]->uint);
    printf("The ATT Flag is %d\n", result->ast->seq->elements[5]->uint);
    printf("The ERR flag is %d\n", result->ast->seq->elements[6]->uint);
    printf("The SOE flag is %d\n", result->ast->seq->elements[7]->uint);
    printf("The ACC flag is %d\n", result->ast->seq->elements[8]->uint);
    printf("The ALG flag is %d\n", result->ast->seq->elements[9]->uint);
    printf("The STS flag is %d\n", result->ast->seq->elements[10]->uint);


    HParser *end = h_bits(1,false);
    HParser *fcnCode = h_bits(7,false);

    return 0;  
}
