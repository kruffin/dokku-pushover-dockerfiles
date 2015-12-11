
#include <string.h>
#include <stdio.h>
#include "msg.h"

void printStr(char *string) {
	char tmp[MSG_SIZE];
	int i;
	for (i = 0; i < MSG_SIZE; i++) {
		if (string[i] == '\0' && i != MSG_SIZE - 1) {
			tmp[i] = '|';
		} else {
			tmp[i] = string[i];
		}
	}

	printf("string: %s\n", tmp);
}

int main(void) {
	struct Message msg;
	char str[MSG_SIZE];
	str[MSG_SIZE - 1] = '\0';

	//memcpy(string[MSG_SIZE - 1], "\0", sizeof(char));

	strcpy(msg.token, "i am a token");
	strcpy(msg.text, "some text here.");

	printf("start msg values\n");
	printf("token: %s\n", msg.token);
	printf("text: %s\n", msg.text);

	packMesageInString(&msg, str);

	printf("pack into string values\n");
	printf("token: %s\n", msg.token);
	printf("text: %s\n", msg.text);
	printStr(str);

	struct Message msg2;
	unpackMessageFromString(str, &msg2);
	printf("unpacking from string to struct\n");
	printf("token: %s\n", msg2.token);
	printf("text: %s\n", msg2.text);
	printStr(str);
}

