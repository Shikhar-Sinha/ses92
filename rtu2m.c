
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
    uint8_t buf[100];
    memset(&buf[0], 99, sizeof(uint8_t));
    memset(&buf[1], 23, sizeof(uint8_t));
    memset(&buf[2], 97, sizeof(uint8_t));
    memset(&buf[3], 254, sizeof(uint8_t));
    memset(&buf[4], 254, sizeof(uint8_t));
    memset(&buf[5], 254, sizeof(uint8_t));
    memset(&buf[6], 254, sizeof(uint8_t));
    memset(&buf[7], 254, sizeof(uint8_t));
    memset(&buf[8], 2, sizeof(uint8_t));
    //memset(&buf[9], 9, sizeof(uint8_t));
    
/*
    printf("%d\n", buf[0]);
    printf("%d\n", buf[1]);
    printf("%d\n", buf[2]);
    printf("%d\n", buf[3]);
*/

    HParser *by = h_uint8();
    HParser *lengthVal = h_uint8();
    HParser *b3 = h_uint8();
    //HParser *b4 = h_uint8();
    HParser *f1 = h_bits(1, false);
    
    HParser *fcnCode = h_bits(7, false);
    /*
    HParser *f3 = h_bits(1, false);
    HParser *f4 = h_bits(1, false);
    HParser *f5 = h_bits(1, false);
    HParser *f6 = h_bits(1, false);
    HParser *f7 = h_bits(1, false);
    HParser *f8 = h_bits(1, false);
    */

    HParser *len = h_length_value(h_uint8(), h_uint8());

    HParser *lengthCheck = h_and(h_sequence(len,by, by, h_end_p(), NULL));
    
    HParser *flags = h_sequence(f1, f1, f1, f1, f1, f1, f1, f1, NULL);
    // Within the actual message, there is a sequence of either: 1+ data sections, or no data sections
    HParser *endBit = h_and(h_ch_range(128, 255));
    HParser *notEndBit = h_and(h_ch_range(0,127));
    HParser *regSection = h_sequence( notEndBit, f1, fcnCode, len, NULL);
    HParser *sections = h_many(regSection);
    HParser *endSection = h_sequence(endBit, fcnCode, len, NULL);
    HParser *onePlus = h_sequence(sections, endSection, by, by, NULL);

    HParser *noData = h_sequence(by, by, NULL);
    HParser * dataSections = h_xor(onePlus, noData);
    HParser *RTU2M = h_sequence(by, by, lengthCheck, lengthVal, flags, dataSections,  NULL);
    HParseResult *result = h_parse(RTU2M, buf, 100);
    if(result) {
        printf("yay\n");
    }
    else{
        printf("boo\n");
    }















    //printf("%d\n", result->ast->seq->elements.length);
    //HParseResult *result = h_parse(b1, buf, 1);
    //printf("%d\n", result->ast->seq->elements[1]->uint);
    /*
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
    */
    return 0;  
}
