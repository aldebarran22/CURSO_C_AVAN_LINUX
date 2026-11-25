#define _DEFAULT_SOURCE

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

static void print_hostent(
    const struct hostent *host)
{
    printf(
        "Nombre oficial: %s\n",
        host->h_name
    );

    puts("Alias:");

    if (host->h_aliases[0] == NULL) {
        puts("  Ninguno");
    } else {
        for (size_t i = 0;
             host->h_aliases[i] != NULL;
             i++) {
            printf(
                "  %s\n",
                host->h_aliases[i]
            );
        }
    }

    printf(
        "Longitud de dirección: %d bytes\n",
        host->h_length
    );

    const char *family_name;
    socklen_t text_buffer_size;

    switch (host->h_addrtype) {
        case AF_INET:
            family_name = "AF_INET, IPv4";
            text_buffer_size = INET_ADDRSTRLEN;
            break;

        case AF_INET6:
            family_name = "AF_INET6, IPv6";
            text_buffer_size = INET6_ADDRSTRLEN;
            break;

        default:
            family_name = "Familia desconocida";
            text_buffer_size = INET6_ADDRSTRLEN;
            break;
    }

    printf(
        "Familia: %s (%d)\n",
        family_name,
        host->h_addrtype
    );

    puts("Direcciones:");

    if (host->h_addr_list[0] == NULL) {
        puts("  Ninguna");
        return;
    }

    for (size_t i = 0;
         host->h_addr_list[i] != NULL;
         i++) {
        char address_text[INET6_ADDRSTRLEN];

        const char *result = inet_ntop(
            host->h_addrtype,
            host->h_addr_list[i],
            address_text,
            text_buffer_size
        );

        if (result == NULL) {
            perror("inet_ntop");
            continue;
        }

        printf(
            "  [%zu] %s\n",
            i,
            address_text
        );
    }
}

int main(int argc, char *argv[])
{
   
    const char *host_name = "www.google.com";

    struct hostent *host =
        gethostbyname(host_name);

    if (host == NULL) {
        fprintf(
            stderr,
            "No se pudo resolver '%s': %s\n",
            host_name,
            hstrerror(h_errno)
        );

        return EXIT_FAILURE;
    }

    print_hostent(host);

    return EXIT_SUCCESS;
}