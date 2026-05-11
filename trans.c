/**
 * Bank-account program: Reads a random-access file sequentially,
 * updates data already written to the file, creates new data to
 * be placed in the file, and deletes data previously in the file.
 * Includes advanced functionalities like View Account, Display All,
 * Transfers, and Bank Statistics.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define LAST_NAME_LEN 15
#define FIRST_NAME_LEN 10

// clientData structure definition
struct clientData {
    unsigned int acctNum;           // account number
    char lastName[LAST_NAME_LEN];   // account last name
    char firstName[FIRST_NAME_LEN]; // account first name
    double balance;                 // account balance
};

// Prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void viewAccount(FILE *fPtr);
void displayAllAccounts(FILE *fPtr);
void transferFunds(FILE *fPtr);
void showStatistics(FILE *fPtr);

void clearInputBuffer(void);
unsigned int getValidUnsignedInt(const char *prompt);

int main(int argc, char *argv[]) {
    FILE *cfPtr; // credit.dat file pointer
    unsigned int choice; // user's choice

    // fopen opens the file; if it does not exist, create it
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        if ((cfPtr = fopen("credit.dat", "wb+")) == NULL) {
            printf("%s: File could not be opened.\n", argv[0]);
            exit(-1);
        }
    }

    // Enable user to specify action
    while ((choice = enterChoice()) != 9) {
        switch (choice) {
            case 1:
                textFile(cfPtr);
                break;
            case 2:
                updateRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            case 5:
                viewAccount(cfPtr);
                break;
            case 6:
                displayAllAccounts(cfPtr);
                break;
            case 7:
                transferFunds(cfPtr);
                break;
            case 8:
                showStatistics(cfPtr);
                break;
            default:
                puts("\n!!! Incorrect choice. Please try again. !!!\n");
                break;
        } // end switch
    } // end while

    fclose(cfPtr); // fclose closes the file
    puts("\nProgram ended successfully.");
    return 0;
} // end main

/**
 * Helper to safely clear stdin buffer
 */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Helper to safely get an unsigned int from the user
 */
unsigned int getValidUnsignedInt(const char *prompt) {
    unsigned int value;
    char buffer[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%u", &value) == 1) {
                return value;
            }
        } else {
            clearerr(stdin);
        }
        printf("Invalid input. Please enter a valid positive number.\n");
    }
}

/**
 * Create formatted text file for printing
 */
void textFile(FILE *readPtr) {
    FILE *writePtr; // accounts.txt file pointer
    int result;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        puts("File could not be opened.");
    } else {
        rewind(readPtr); // sets pointer to beginning of file
        fprintf(writePtr, "%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");

        while (!feof(readPtr)) {
            result = fread(&client, sizeof(struct clientData), 1, readPtr);
            if (result != 0 && client.acctNum != 0) {
                fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
            }
        }
        fclose(writePtr);
        puts("\n--- Successfully exported to 'accounts.txt' ---\n");
    }
}

/**
 * Update balance in record
 */
void updateRecord(FILE *fPtr) {
    unsigned int account;
    double transaction;
    char buffer[100];
    struct clientData client = {0, "", "", 0.0};

    account = getValidUnsignedInt("Enter account to update ( 1 - 100 ): ");
    if (account < 1 || account > MAX_ACCOUNTS) {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (account - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account #%d has no information.\n\n", account);
    } else {
        printf("\n%-6d%-16s%-11s%10.2f\n\n", client.acctNum, client.lastName, client.firstName, client.balance);

        while (1) {
            printf("Enter charge ( + ) or payment ( - ): ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%lf", &transaction) == 1) {
                    break;
                }
            }
            printf("Invalid transaction amount.\n");
        }

        client.balance += transaction;

        printf("\nUpdated Record:\n%-6d%-16s%-11s%10.2f\n\n", client.acctNum, client.lastName, client.firstName, client.balance);

        fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    }
}

/**
 * Delete an existing record
 */
void deleteRecord(FILE *fPtr) {
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};
    unsigned int accountNum;

    accountNum = getValidUnsignedInt("Enter account number to delete ( 1 - 100 ): ");
    if (accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account %d does not exist.\n\n", accountNum);
    } else {
        fseek(fPtr, (accountNum - 1) * (long)sizeof(struct clientData), SEEK_SET);
        fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
        printf("\n--- Account #%d successfully deleted. ---\n\n", accountNum);
    }
}

/**
 * Create and insert record
 */
void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;
    char buffer[100];

    accountNum = getValidUnsignedInt("Enter new account number ( 1 - 100 ): ");
    if (accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf("Invalid account number. Must be between 1 and %d.\n", MAX_ACCOUNTS);
        return;
    }

    fseek(fPtr, (accountNum - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account #%d already contains information.\n\n", client.acctNum);
    } else {
        printf("Enter lastname, firstname, balance (e.g. Doe John 150.50)\n? ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            char last[LAST_NAME_LEN];
            char first[FIRST_NAME_LEN];
            double bal;
            
            if (sscanf(buffer, "%14s %9s %lf", last, first, &bal) == 3) {
                strncpy(client.lastName, last, LAST_NAME_LEN - 1);
                client.lastName[LAST_NAME_LEN - 1] = '\0';
                
                strncpy(client.firstName, first, FIRST_NAME_LEN - 1);
                client.firstName[FIRST_NAME_LEN - 1] = '\0';
                
                client.balance = bal;
                client.acctNum = accountNum;

                fseek(fPtr, (client.acctNum - 1) * (long)sizeof(struct clientData), SEEK_SET);
                fwrite(&client, sizeof(struct clientData), 1, fPtr);
                printf("\n--- Account #%d successfully created. ---\n\n", accountNum);
            } else {
                printf("Error parsing input. Please ensure you provide: Lastname Firstname Balance\n\n");
            }
        }
    }
}

/**
 * View a specific account safely without modifying it
 */
void viewAccount(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    accountNum = getValidUnsignedInt("Enter account number to view ( 1 - 100 ): ");
    if (accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr, (accountNum - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account #%d has no information.\n\n", accountNum);
    } else {
        printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
        printf("%-6d%-16s%-11s%10.2f\n\n", client.acctNum, client.lastName, client.firstName, client.balance);
    }
}

/**
 * Display all accounts directly to the console
 */
void displayAllAccounts(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    int foundAny = 0;

    rewind(fPtr);
    printf("\n%-6s%-16s%-11s%10s\n", "Acct", "Last Name", "First Name", "Balance");
    printf("--------------------------------------------------\n");

    while (!feof(fPtr)) {
        if (fread(&client, sizeof(struct clientData), 1, fPtr) != 0 && client.acctNum != 0) {
            printf("%-6d%-16s%-11s%10.2f\n", client.acctNum, client.lastName, client.firstName, client.balance);
            foundAny = 1;
        }
    }

    if (!foundAny) {
        printf("No active accounts found in the database.\n");
    }
    printf("\n");
}

/**
 * Transfer funds securely between two accounts
 */
void transferFunds(FILE *fPtr) {
    struct clientData srcClient = {0, "", "", 0.0};
    struct clientData destClient = {0, "", "", 0.0};
    unsigned int srcAccount, destAccount;
    double amount;
    char buffer[100];

    printf("\n--- Funds Transfer ---\n");
    srcAccount = getValidUnsignedInt("Enter SOURCE account number ( 1 - 100 ): ");
    if (srcAccount < 1 || srcAccount > MAX_ACCOUNTS) {
        printf("Invalid source account number.\n");
        return;
    }

    fseek(fPtr, (srcAccount - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fread(&srcClient, sizeof(struct clientData), 1, fPtr);

    if (srcClient.acctNum == 0) {
        printf("Source account #%d does not exist.\n\n", srcAccount);
        return;
    }

    destAccount = getValidUnsignedInt("Enter DESTINATION account number ( 1 - 100 ): ");
    if (destAccount < 1 || destAccount > MAX_ACCOUNTS || destAccount == srcAccount) {
        printf("Invalid destination account number (cannot be the same as source).\n");
        return;
    }

    fseek(fPtr, (destAccount - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fread(&destClient, sizeof(struct clientData), 1, fPtr);

    if (destClient.acctNum == 0) {
        printf("Destination account #%d does not exist.\n\n", destAccount);
        return;
    }

    while (1) {
        printf("Enter amount to transfer: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%lf", &amount) == 1 && amount > 0) {
                break;
            }
        }
        printf("Invalid amount. Must be greater than 0.\n");
    }

    if (srcClient.balance < amount) {
        printf("Error: Source account has insufficient funds (Balance: %.2f).\n\n", srcClient.balance);
        return;
    }

    // Deduct from source
    srcClient.balance -= amount;
    fseek(fPtr, (srcAccount - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fwrite(&srcClient, sizeof(struct clientData), 1, fPtr);

    // Add to destination
    destClient.balance += amount;
    fseek(fPtr, (destAccount - 1) * (long)sizeof(struct clientData), SEEK_SET);
    fwrite(&destClient, sizeof(struct clientData), 1, fPtr);

    printf("\n--- Transfer Successful! ---\n");
    printf("Transferred %.2f from Acct #%d to Acct #%d.\n\n", amount, srcAccount, destAccount);
}

/**
 * Calculate bank statistics
 */
void showStatistics(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    double totalBankBalance = 0.0;
    double highestBalance = -999999999.0;
    unsigned int highestAccount = 0;
    int activeAccountsCount = 0;

    rewind(fPtr);

    while (!feof(fPtr)) {
        if (fread(&client, sizeof(struct clientData), 1, fPtr) != 0 && client.acctNum != 0) {
            totalBankBalance += client.balance;
            activeAccountsCount++;

            if (client.balance > highestBalance) {
                highestBalance = client.balance;
                highestAccount = client.acctNum;
            }
        }
    }

    printf("\n============================================\n");
    printf("              BANK STATISTICS                 \n");
    printf("============================================\n");
    printf("Total Active Accounts : %d\n", activeAccountsCount);
    printf("Total Bank Balance    : %.2f\n", totalBankBalance);
    
    if (activeAccountsCount > 0) {
        printf("Highest Balance Acct  : #%d (%.2f)\n", highestAccount, highestBalance);
    }
    printf("============================================\n\n");
}

/**
 * Enable user to input menu choice safely
 */
unsigned int enterChoice(void) {
    unsigned int menuChoice;
    printf("\n============================================\n");
    printf("              BANKING SYSTEM MENU             \n");
    printf("============================================\n");
    printf("1 - Store formatted text file for printing\n");
    printf("2 - Update an account\n");
    printf("3 - Add a new account\n");
    printf("4 - Delete an account\n");
    printf("5 - View a specific account\n");
    printf("6 - Display all active accounts\n");
    printf("7 - Transfer funds between accounts\n");
    printf("8 - Show bank statistics\n");
    printf("9 - End program\n");
    printf("============================================\n");
    
    menuChoice = getValidUnsignedInt("Enter your choice: ");
    return menuChoice;
}