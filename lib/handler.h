#ifndef HANDLER_H
#define HANDLER_H

#include <time.h>

#define ACCOUNT_IDENTIFIER_SIZE 20
#define TRANSACTION_IDENTIFIER_SIZE 20

struct Account {
    int  id         ;
    char number     [ACCOUNT_IDENTIFIER_SIZE];
    int  active     ;
    float balance   ;
};

#define OPERATION_RESULT_MESSAGE_LENGTH 256

#define OPERATION_CODE_SIZE     3
#define TIMESTAMP_SIZE          14
#define AMOUNT_SIZE             6
#define MAX_THREADS             100
#define MAX_OPERATION_LIST      10

struct OperationResult {
    int status;
    char message        [OPERATION_RESULT_MESSAGE_LENGTH];
    struct Account account;
    int transactions_cnt;
    struct TransactionLog *transactions[MAX_OPERATION_LIST];
};

struct Operation {
   char account     [ACCOUNT_IDENTIFIER_SIZE];
   char operation   [OPERATION_CODE_SIZE];
   char timestamp   [TIMESTAMP_SIZE];
   char amount      [AMOUNT_SIZE];
};


enum TransactionStatus {
    Accepted,
    Rejected,
    Invalid
};

struct TransactionLog {
    char identifier     [TRANSACTION_IDENTIFIER_SIZE];
    char account        [ACCOUNT_IDENTIFIER_SIZE];
    float amount        ;
    enum TransactionStatus   status;
    time_t time         ;
};

#define OPERATION_SIZE                  43

#define TXT_OPERATION_SUCCESS           0
#define TXT_VALIDATION_CONTINUE         10

#define MIN_OPERATION_AMOUNT            0
#define MAX_OPERATION_AMOUNT            999999

#define MAX_ACCOUNTS                    100
#define MAX_TRANSACTION_LOGS            10000

#define TXT_OPERATION_DEBIT             100
#define TXT_OPERATION_CREDIT            101
#define TXT_OPERATION_READ_BALANCE      102
#define TXT_OPERATION_READ_TRANSACTION  103

#define TXT_OPERATION_DEBIT_STR             "100"
#define TXT_OPERATION_CREDIT_STR            "101"
#define TXT_OPERATION_READ_BALANCE_STR      "102"
#define TXT_OPERATION_READ_TRANSACTION_STR  "103"

#define TXT_ERROR_UNKNOWN_ACCOUNT       1000
#define TXT_ERROR_INSUFFICIENT_FUNDS    1001
#define TXT_ERROR_UNKNOWN_TRANSACTION   1002

#define TXT_ERROR_INVALID_AMOUNT         2000
#define TXT_ERROR_INVALID_ACCOUNT_NUMBER 2001
#define TXT_ERROR_INVALID_OPERATION      2002
#define TXT_ERROR_INVALID_TIMESTAMP      2003

#define TXT_SERVICE_OK                  3000;
#define TXT_SERVICE_ERROR               3001;

int txt_handle(struct Operation operation, struct OperationResult *result);
int txt_init();
#endif
