#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_NUMBERS 10
#define MAX_GROUPS 5
#define PORT 8888

typedef struct {
    char numbers[MAX_NUMBERS][15];
    int num_numbers;
} DNDList;

typedef struct {
    char groups[MAX_GROUPS][20];
    int num_groups;
} DNDGroups;

typedef struct {
    char username[50];
    char password[50];
    char blocked_calls_filename[60];
} UserInfo;

// Function prototypes
void handleClient(int clientSocket, UserInfo *userInfo, DNDList *blockedList, DNDGroups *blockedGroups);

int main() {
    UserInfo userInfo;
    DNDList blockedList = {.num_numbers = 0};
    DNDGroups blockedGroups = {.num_groups = 0};

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Create socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
 }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept incoming connection
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // Handle client requests
        handleClient(clientSocket, &userInfo, &blockedList, &blockedGroups);

        close(clientSocket);
        printf("Connection closed.\n");
    }

    close(serverSocket);

    return 0;
}

void handleClient(int clientSocket, UserInfo *userInfo, DNDList *blockedList, DNDGroups *blockedGroups) {
 char buffer[1024];
    int choice;

    // Send welcome message to the client
    snprintf(buffer, sizeof(buffer), "Welcome to the DND Service!\n");
    send(clientSocket, buffer, strlen(buffer), 0);

    // Receive client's choice
    recv(clientSocket, &choice, sizeof(choice), 0);

    switch (choice) {
        case 1:
            // Activate DND
            printf("Client activated DND service.\n");
            printf("Enter your choice for DND mode (1 for global, 2 for selective):\n");
            recv(clientSocket, &choice, sizeof(choice), 0);
            switch (choice) {
                case 1:
                    printf("Global DND activated. Client will not receive any calls.\n");
                    break;
                case 2:
                    printf("Selective DND activated. Calls from specified numbers/groups will be blocked.\n");
                    printf("Enter the number of numbers you want to block: ");
                    int num_to_block;
                    recv(clientSocket, &num_to_block, sizeof(num_to_block), 0);
                    for (int i = 0; i < num_to_block; ++i) {
                        recv(clientSocket, buffer, sizeof(buffer), 0);
                        strcpy(blockedList->numbers[blockedList->num_numbers], buffer);
                        blockedList->num_numbers++;
                    }
                    printf("Blocked %d numbers.\n", num_to_block);
                    break;
                default:
                    printf("Invalid choice for DND mode.\n");
                    break;
            }
            break;
        default:
            printf("Invalid choice for activation.\n");
            break;
    }
}

