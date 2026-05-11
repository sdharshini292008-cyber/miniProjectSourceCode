# Changes in trans.c

## Overview
The `trans.c` file implements a comprehensive banking system management program in C. It provides functionalities for managing bank accounts stored in a random-access file (`credit.dat`). The program includes features for creating, updating, deleting, viewing accounts, transferring funds, and generating reports.

## Key Features Implemented

### 1. **File Handling**
- Opens `credit.dat` in binary read/write mode for random access operations.
- Creates the file if it doesn't exist.
- Supports up to 100 accounts (defined by `MAX_ACCOUNTS`).

### 2. **Data Structure**
- `clientData` struct containing:
  - `acctNum`: Account number (unsigned int)
  - `lastName`: Last name (15 characters)
  - `firstName`: First name (10 characters)
  - `balance`: Account balance (double)

### 3. **Menu System**
- Interactive menu with 9 options:
  1. Store formatted text file for printing
  2. Update an account
  3. Add a new account
  4. Delete an account
  5. View a specific account
  6. Display all active accounts
  7. Transfer funds between accounts
  8. Show bank statistics
  9. End program

### 4. **Core Functions**

#### `textFile(FILE *readPtr)`
- Exports account data to a formatted text file (`accounts.txt`).
- Includes headers and properly formatted account information.

#### `updateRecord(FILE *fPtr)`
- Updates account balance with charges (+) or payments (-).
- Validates account existence and transaction amounts.

#### `newRecord(FILE *fPtr)`
- Creates new accounts with user-provided details.
- Validates account numbers and prevents duplicates.

#### `deleteRecord(FILE *fPtr)`
- Deletes accounts by setting them to blank records.
- Confirms account existence before deletion.

#### `viewAccount(FILE *fPtr)`
- Displays specific account information without modification.
- Safe read-only operation.

#### `displayAllAccounts(FILE *fPtr)`
- Lists all active accounts to the console.
- Handles cases with no active accounts.

#### `transferFunds(FILE *fPtr)`
- Secure fund transfers between accounts.
- Validates source funds, destination existence, and prevents self-transfers.

#### `showStatistics(FILE *fPtr)`
- Calculates and displays bank statistics:
  - Total active accounts
  - Total bank balance
  - Highest balance account

### 5. **Input Validation and Safety**
- `getValidUnsignedInt()`: Safely reads unsigned integers with error handling.
- `clearInputBuffer()`: Clears stdin buffer to prevent input issues.
- Comprehensive validation for account numbers, amounts, and user inputs.

### 6. **Error Handling**
- File opening error handling.
- Invalid input detection and user-friendly messages.
- Account existence checks before operations.

### 7. **Code Quality Improvements**
- Modular function design.
- Clear comments and documentation.
- Consistent formatting and naming conventions.
- Safe string handling with `strncpy` to prevent buffer overflows.

## Compilation and Execution
- Compiled to `trans.exe` using GCC.
- Requires standard C libraries: `<stdio.h>`, `<stdlib.h>`, `<string.h>`.
- Data persists in `credit.dat` between program runs.

## Recent Changes
- **Added executable**: `trans.exe` compiled from `trans.c`.
- Full implementation of banking system features.
- Enhanced input validation and error handling.
- Improved user interface with clear menus and messages.