#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "handler.h"
#include "operation.h"

int txt_process_debit_for_account(struct Operation operation, struct Account *account, struct OperationResult *result) {
    if (account->balance < atof(operation.amount))
    {
        result->status = TXT_ERROR_INSUFFICIENT_FUNDS;

        return result->status;
    }

    account->balance -= atof(operation.amount);

    result->status = TXT_OPERATION_SUCCESS;

    return result->status;
}

int txt_process_credit_for_account(struct Operation operation, struct Account *account, struct OperationResult *result) {
    result->status = TXT_OPERATION_SUCCESS;

    account->balance += atof(operation.amount);

    return result->status;
}

int txt_process_read_for_account(struct Operation operation, struct Account *account, struct OperationResult *result) {
    result->status = TXT_OPERATION_SUCCESS;

    return result->status;
}

int txt_process_read_for_transaction(struct Operation operation, struct Account *account, struct OperationResult *result) {
    result->status = TXT_OPERATION_SUCCESS;

    return result->status;
}
