#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "handler.h"
#include "operation.h"
#include "account.h"

struct TransactionLog transaction_log[MAX_TRANSACTION_LOGS];
int transaction_idx = 0;

int txt_get_operation_code(struct Operation operation) {
    char op_str[OPERATION_CODE_SIZE+1];

    memset(op_str, 0, OPERATION_CODE_SIZE+1);
    memcpy(op_str, operation.operation, OPERATION_CODE_SIZE);

    return atoi(op_str);
}

int txt_validate_account_identifier(struct Operation operation) {
    if (strlen(operation.account) == 0)
    {
        return TXT_ERROR_INVALID_ACCOUNT_NUMBER;
    }

#if ALLOW_CREATE_ACCOUNT_ON_THE_FLAY == 0
    if (txt_exists_account(operation) == 0)
    {
        return TXT_ERROR_UNKNOWN_ACCOUNT;
    }
#endif

    return TXT_VALIDATION_CONTINUE;
}

int txt_validate_operation(struct Operation operation) {
    int op = txt_get_operation_code(operation);

    if (op != TXT_OPERATION_DEBIT
            && op != TXT_OPERATION_CREDIT
            && op != TXT_OPERATION_READ
    )
    {
        return TXT_ERROR_INVALID_OPERATION;
    }

    return TXT_VALIDATION_CONTINUE;
}

int txt_validate_amount(struct Operation operation) {
    int amount;
    char amount_str[AMOUNT_SIZE+1];

    if (txt_get_operation_code(operation) == TXT_OPERATION_READ)
    {
        return TXT_VALIDATION_CONTINUE;
    }

    memset(amount_str, 0, AMOUNT_SIZE+1);
    memcpy(amount_str, operation.amount, AMOUNT_SIZE);
    amount = atoi(amount_str);
    if (amount <= MIN_OPERATION_AMOUNT || amount >= MAX_OPERATION_AMOUNT)
    {
        return TXT_ERROR_INVALID_AMOUNT;
    }

    return TXT_VALIDATION_CONTINUE;
}

int txt_add_transation_log(struct Operation operation, struct Account account, struct OperationResult result) {
    struct TransactionLog *log = malloc(sizeof(struct TransactionLog));

    memcpy(log->account, operation.account, ACCOUNT_IDENTIFIER_SIZE);
    log->amount = atoi(operation.amount);
    switch (result.status)
    {
        case TXT_OPERATION_SUCCESS:
            log->status = Accepted;
            break;
        default:
            log->status = Rejected;
            break;
    }

    transaction_log[transaction_idx] = *log;
    transaction_idx++;

    return 0;
}

int txt_handle(struct Operation operation, struct OperationResult *result) {

    result->status = txt_validate_account_identifier(operation);
    if (result->status != TXT_VALIDATION_CONTINUE)
    {
        return result->status;
    }

    result->status = txt_validate_operation(operation);
    if (result->status != TXT_VALIDATION_CONTINUE)
    {
        return result->status;
    }

    result->status = txt_validate_amount(operation);
    if (result->status != TXT_VALIDATION_CONTINUE)
    {
        return result->status;
    }

#if ALLOW_CREATE_ACCOUNT_ON_THE_FLAY == 1
    struct Account *account = txt_get_or_create_account(operation);
#else
    struct Account *account = txt_get_account(operation);
#endif

    switch (txt_get_operation_code(operation))
    {
        case TXT_OPERATION_DEBIT:
            txt_process_debit_for_account(operation, account, result);
            break;
        case TXT_OPERATION_CREDIT:
            txt_process_credit_for_account(operation, account, result);
            break;
        case TXT_OPERATION_READ:
            txt_process_read_for_account(operation, account, result);
            break;
    }

    txt_add_transation_log(operation, *account, *result);

    result->account = *account;

    return result->status;
}

int txt_init() {
    txt_init_accounts();

    return TXT_SERVICE_OK;
}
