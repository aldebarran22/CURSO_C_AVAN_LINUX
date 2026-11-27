#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>

int send_all(int fd, const void *buffer, size_t length)
{
    const uint8_t *cursor = buffer;
    size_t total = 0;

    while (total < length) {
        ssize_t sent = send(
            fd,
            cursor + total,
            length - total,
            0
        );

        if (sent < 0) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }

        if (sent == 0) {
            return -1;
        }

        total += (size_t)sent;
    }

    return 0;
}

int recv_all(int fd, void *buffer, size_t length)
{
    uint8_t *cursor = buffer;
    size_t total = 0;

    while (total < length) {
        ssize_t received = recv(
            fd,
            cursor + total,
            length - total,
            0
        );

        if (received < 0) {
            if (errno == EINTR) {
                continue;
            }

            return -1;
        }

        if (received == 0) {
            return 0;
        }

        total += (size_t)received;
    }

    return 1;
}
