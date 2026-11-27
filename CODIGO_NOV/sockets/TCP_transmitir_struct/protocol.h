#ifndef PROTOCOL_H
#define PROTOCOL_H

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

static inline void serialize_header(
    uint8_t buffer[HEADER_SIZE],
    const message_header_t *header)
{
    uint16_t length = htons(header->length);
    uint32_t identifier = htonl(header->identifier);

    buffer[0] = header->version;
    buffer[1] = header->type;

    memcpy(buffer + 2, &length, sizeof(length));
    memcpy(buffer + 4, &identifier, sizeof(identifier));
}

static inline void deserialize_header(
    message_header_t *header,
    const uint8_t buffer[HEADER_SIZE])
{
    uint16_t length;
    uint32_t identifier;

    header->version = buffer[0];
    header->type = buffer[1];

    memcpy(&length, buffer + 2, sizeof(length));
    memcpy(&identifier, buffer + 4, sizeof(identifier));

    header->length = ntohs(length);
    header->identifier = ntohl(identifier);
}

#endif
