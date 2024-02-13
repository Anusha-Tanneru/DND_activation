#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888

// Function prototypes
void displayMenu();
void activateDND(int clientSocket);

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of the server
    serverAddr.sin_port = htons(PORT);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    displayMenu();

    int choice;
    scanf("%d", &choice);

    // Send client's choice to the server
    send(clientSocket, &choice, sizeof(choice), 0);

    switch (choice) {
case 1:
            // Activate DND
            activateDND(clientSocket);
            break;
        case 2:
            // Handle incoming call
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }

    close(clientSocket);

    return 0;
}

void displayMenu() {
    printf("DND Service Menu:\n");
    printf("1. Activate DND\n");
    printf("2. Handle Incoming Call\n");
    printf("Enter your choice: ");
}

void activateDND(int clientSocket) {
    int choice;
    printf("Enter your choice for activation (1 or 2):\n");
    printf("1. Activate DND\n");
    printf("2. Deactivate DND\n");
    scanf("%d", &choice);

    // Send client's choice for activation to the server
    send(clientSocket, &choice, sizeof(choice), 0);

    switch (choice) {
        case 1:
            printf("You chose to activate DND.\n");
            printf("Enter your choice for DND mode (1 for global, 2 for selective):\n");
            scanf("%d", &choice);
 // Send client's choice for DND mode to the server
            send(clientSocket, &choice, sizeof(choice), 0);

            switch (choice) {
                case 1:
                    printf("Global DND activated.\n");
                    break;
                case 2:
                    printf("Selective DND activated.\n");

                    // Enter the number of numbers to block
                    int num_to_block;
                    printf("Enter the number of numbers you want to block: ");
                    scanf("%d", &num_to_block);

                    // Send the number of numbers to block to the server
                    send(clientSocket, &num_to_block, sizeof(num_to_block), 0);

                    // Enter the numbers to block
                    char buffer[15];
                    for (int i = 0; i < num_to_block; ++i) {
                        printf("Enter number to block: ");
                        scanf("%s", buffer);

                        // Send each number to block to the server
                        send(clientSocket, buffer, sizeof(buffer), 0);
                    }

                    printf("Blocked numbers sent to server.\n");
                    break;
                default:
                    printf("Invalid choice for DND mode.\n");
                    break;
            }
            break;
        case 2:
            printf("Deactivate DND.\n");
            break;
        default:
 printf("Invalid choice for activation.\n");
            break;
    }
}
