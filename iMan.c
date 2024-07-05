#include "headers.h"

#define MAX_BUFFER_SIZE 1024

void iMan(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    struct hostent *host;
    int sockfd;

    // Perform DNS resolution for man.he.net
    host = gethostbyname("man.he.net");
    if (host == NULL) {
        perror("DNS resolution failed");
        return;
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        return;
    }

    // Initialize the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port

    // Copy the resolved IP address to the server address structure
    memcpy(&server_addr.sin_addr.s_addr, host-> h_addr, host->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    // Send an HTTP GET request
    char request[MAX_BUFFER_SIZE];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", argv[1]);
    write(sockfd, request, strlen(request));
    // printf("%s",request);

    // Read and print the response
    char response[MAX_BUFFER_SIZE];
    int bytes_read;
    
    // Read until headers end (two consecutive newline characters)
    char *headers_end = NULL;
    while ((bytes_read = read(sockfd, response, sizeof(response))) > 0) {
        headers_end = strstr(response, "\r\n\r\n");
        printf("%s",response);
        if (headers_end != NULL) {
            headers_end += 4; // Skip past the newline characters
            break;
        }
    }

    // Print the rest of the response (body)
    if (headers_end != NULL) {
        printf("%s", headers_end);
        while ((bytes_read = read(sockfd, response, sizeof(response))) > 0) {
            printf("%s", response);
        }
    }

    // Check if the page exists (you can customize this part)
    if (strstr(response, "No such command") != NULL) {
        printf("ERROR\nNo such command\n");
    }

    // Close the socket
    close(sockfd);
}