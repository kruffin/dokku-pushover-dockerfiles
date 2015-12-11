#ifndef HEADER_MSG_32303290324
#define HEADER_MSG_32303290324

#include <string.h>

#define MSG_SIZE 			4096 + 1
#define MSG_TOKEN_LENGTH	1024
#define MSG_TEXT_LENGTH		3072

struct Message {
	char token[MSG_TOKEN_LENGTH];
	char text[MSG_TEXT_LENGTH];
};

void packMesageInString(struct Message *msg, char *string) {
	memcpy(&string[0], &(msg->token[0]), sizeof(char) * MSG_TOKEN_LENGTH);
	memcpy(&string[MSG_TOKEN_LENGTH], &(msg->text[0]), sizeof(char) * MSG_TEXT_LENGTH);
}

void unpackMessageFromString(char *string, struct Message *msg) {
	memcpy(&(msg->token[0]), &string[0], sizeof(char) * MSG_TOKEN_LENGTH);
	memcpy(&(msg->text[0]), &string[MSG_TOKEN_LENGTH], sizeof(char) * MSG_TEXT_LENGTH);
}

#endif