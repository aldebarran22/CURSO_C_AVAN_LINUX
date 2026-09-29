#include <arpa/inet.h>
#include <stdint.h>
#include <string.h>

#define HEADER_SIZE 8

typedef struct {
    uint8_t version;
    uint8_t type;
    uint16_t length;
    uint32_t identifier;
} message_header_t;


void serialize_header( uint8_t buffer[HEADER_SIZE],  const message_header_t *header) {
    uint16_t length = htons(header->length);
    uint32_t identifier = htonl(header->identifier);
    buffer[0] = header->version;
    buffer[1] = header->type;
    memcpy(buffer + 2, &length, sizeof(length));
    memcpy(buffer + 4,&identifier, sizeof(identifier));
}

void deserialize_header(message_header_t *header,  const uint8_t buffer[HEAD*R_SIZE]){
    uint16_t length;
    uint32_t identifier;

    header->version = buffer[0];
    header->type = buffer[1];

    memcpy(&length, buffer + 2, sizeof(length));
    memcpy(&identifier, buffer + 4, sizeof(identifier));

    header->length = ntohs(length);
    header->identifier = ntohl(identifier);
}

void print_message_header(const message_header_t* mh){
	printf("\nmessage_header_t");
	printf("\nVersion: %u", mh->version);
	printf("\nType: %u", mh->type);
	printf("\nLength: %d", mh->length);
	printf("\nidentifier: %d", mh->identifier);
}

int main(){
	message_header_t mh = {
		.version = 1,
		.type = 2,
		.length = htons(128),
		.identifier = htonl(1000)
	};
	message_header_t mh2; 
	uint8_t buffer[HEADER_SIZE];
	
	printf("\nSizeof message_header_t: %zu\n", sizeof(message_header_t));
	print_message_header(&mh);
	serialize_header(buffer, &mh);
	
	deserialize_header(&mh2, buffer);
	print_message_header(&mh2);
	return 0;
}


