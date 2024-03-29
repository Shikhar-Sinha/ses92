
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
	char buf[100] = "\xff\xf0\x08\xaa\x71\x02\x01\x03\x8f\x01\x02\xcf\xcd";
	//char buf[100] = "\xff\xf0\x05\xaa\x8f\x02\x01\x11\xce\xce";
	//char buf[100] = "\xff\xf0\x01\xaa\xce\xce";
	

	HParser *by = h_uint8();
	HParser *lengthVal = h_int_range(h_uint8(), 1, 255);
	HParser *b3 = h_uint8();

	HParser *soloAddr = h_sequence(h_int_range(h_uint8(), 0, 254), h_int_range(h_uint8(), 0, 254), NULL);
	HParser *groupAddr = h_sequence(h_int_range(h_uint8(),255, 255), h_int_range(h_uint8(), 0, 254), NULL);
	HParser *globAddr = h_sequence(h_int_range(h_uint8(),255, 255), h_int_range(h_uint8(), 255, 255), NULL);

	HParser *f1 = h_bits(1, false);

	HParser *addrBits = h_xor(soloAddr, h_xor(groupAddr, globAddr));

	HParser *fcnCode = h_bits(7, false);


	HParser *len = h_length_value(h_uint8(), h_uint8());

	HParser *lengthCheck = h_and(h_sequence(len, by, by, h_end_p(), NULL));

	HParser *flags = h_sequence(f1, f1, f1, f1, f1, f1, f1, f1, NULL);
	// Within the actual message, there is a sequence of either: 1+ data sections, or no data sections
	HParser *endBit = h_and(h_int_range(h_uint8(), 128, 255));
	HParser *notEndBit = h_and(h_int_range(h_uint8(), 0, 127));
	HParser *dataLenCheck = h_and(h_int_range(h_uint8(), 1,252));
	HParser *regSection = h_sequence(notEndBit, f1, fcnCode, dataLenCheck, len, NULL);
	HParser *sections = h_many(regSection);
	HParser *endSection = h_sequence(endBit, f1, fcnCode, dataLenCheck, len, NULL);
	HParser *onePlus = h_sequence(sections, endSection, by, by, h_end_p(), NULL);

	HParser *noData = h_sequence(by, by, h_end_p(), NULL);
	HParser *dataSections = h_xor(onePlus, noData);
	HParser *RTU2M = h_sequence(addrBits, lengthCheck, lengthVal, flags, dataSections, h_end_p(), NULL);
	HParser *printable = h_sequence(h_and(RTU2M), h_uint8(), h_uint8(), h_uint8(), flags, h_many(h_uint8()), h_end_p(), NULL);
	HParseResult *result = h_parse(printable, buf, strnlen(buf, 100));
	if(result) {
		printf("yay\n");
		void *user;
		HParsedToken *fields = h_act_flatten(result, user);

		int frameLen = result->bit_length;

		printf("RTU identifier: %d\n", fields->seq->elements[0]->uint);
		printf("Group identifier: %d\n", fields->seq->elements[1]->uint);
		int restLen = fields->seq->elements[2]->uint;
		printf("Length of rest of frame (minus check codes bytes): %d\n", fields->seq->elements[2]->uint);
		printf("The FIN flag is %d\n", fields->seq->elements[3]->uint);
		printf("The ALE Flag is %d\n", fields->seq->elements[4]->uint);
		printf("The ATT Flag is %d\n", fields->seq->elements[5]->uint);
		printf("The ERR flag is %d\n", fields->seq->elements[6]->uint);
		printf("The SOE flag is %d\n", fields->seq->elements[7]->uint);
		printf("The ACC flag is %d\n", fields->seq->elements[8]->uint);
		printf("The ALG flag is %d\n", fields->seq->elements[9]->uint);
		printf("The STS flag is %d\n", fields->seq->elements[10]->uint);
		int bytes = 5;
		int pos = 11;
		while (bytes < strnlen(buf, 100) - 2){
			int dataB1 = fields->seq->elements[pos]->uint;
			printf("Endbit: %d\n", dataB1 >> 7);
			if (dataB1 > 127) {
				dataB1 -= 128;
			}
			printf("Function Code: %d\n", dataB1);
			int restOfData = fields->seq->elements[pos + 1]->uint;
			printf("Length of data section: %d\n", restOfData);
			pos += restOfData + 2;
			bytes += restOfData + 2;


		}
		printf("Check Code Byte 1: %d\n", fields->seq->elements[pos]->uint);
		printf("Check Code Byte 2: %d\n", fields->seq->elements[pos + 1]->uint);


	}
	else{
		printf("boo\n");
	}


	return 0;  
}
