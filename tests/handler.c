#include "deps/greatest/greatest.h"

#include "../lib/handler.h"

TEST txt_reject_invalid_parameters(void) {
    struct Operation operation;
    struct OperationResult result;
    txt_init();

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.account, "", 1);
    txt_handle(operation, &result);
    ASSERT_EQm(
            "Operation must be rejected with invalid account number",
            TXT_ERROR_INVALID_ACCOUNT_NUMBER,
            result.status
    );

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.operation, "INVALID", sizeof(operation.operation));
    memcpy(operation.account, "123123123123", sizeof(operation.account));
    txt_handle(operation, &result);
    ASSERT_EQm(
            "Operation must be rejected with invalid operation error",
            TXT_ERROR_INVALID_OPERATION,
            result.status
    );

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.operation, "100", 1);
    memcpy(operation.account, "123123123123", sizeof(operation.account));
    txt_handle(operation, &result);
    ASSERT_EQm(
            "Operation must be rejected with invalid amount error",
            TXT_ERROR_INVALID_OPERATION,
            result.status
    );

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.operation, TXT_OPERATION_DEBIT_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", sizeof(operation.account));
    memcpy(operation.amount, "1000", AMOUNT_SIZE);
    txt_handle(operation, &result);

    ASSERT_EQm(
            "Operation must be rejected with insufficient funds error",
            TXT_ERROR_INSUFFICIENT_FUNDS,
            result.status
    );

    PASS();
}

TEST txt_handle_debit(void) {
    struct Operation operation;
    struct OperationResult result;
    txt_init();

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.operation, TXT_OPERATION_DEBIT_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", sizeof(operation.account));
    memcpy(operation.amount, "1000", AMOUNT_SIZE);

    txt_handle(operation, &result);
    ASSERT_EQm(
            "Operation must be rejected with insufficient funds error",
            TXT_ERROR_INSUFFICIENT_FUNDS,
            result.status
    );

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.operation, TXT_OPERATION_DEBIT_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", sizeof(operation.account));
    memcpy(operation.amount, "0", AMOUNT_SIZE);

    txt_handle(operation, &result);
    ASSERT_EQm(
            "Operation must be rejected with invalid amount error",
            TXT_ERROR_INVALID_AMOUNT,
            result.status
    );

    PASS();
}

TEST txt_handle_credit(void) {
    struct Operation operation;
    struct OperationResult result;

    txt_init();

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.operation, TXT_OPERATION_CREDIT_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", sizeof(operation.account));
    memcpy(operation.amount, "0", AMOUNT_SIZE);

    txt_handle(operation, &result);

    ASSERT_EQm(
            "Operation must be rejected with invalid amount error",
            TXT_ERROR_INVALID_AMOUNT,
            result.status
    );

    memset(&operation, 0, OPERATION_SIZE);
    memcpy(operation.operation, TXT_OPERATION_CREDIT_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", sizeof(operation.account));
    memcpy(operation.amount, "10", AMOUNT_SIZE);

    memset(&result, 0, sizeof(result));
    txt_handle(operation, &result);

    ASSERT_EQm(
            "Operation must be accepted",
            TXT_OPERATION_SUCCESS,
            result.status
    );

    PASS();
}

TEST txt_handle_credit_and_debit(void) {
    struct Operation operation;
    struct OperationResult result;
    txt_init();

    memset(&operation, 0, OPERATION_SIZE);
    memset(&result, 0, sizeof(result));

    memcpy(operation.operation, TXT_OPERATION_CREDIT_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", ACCOUNT_IDENTIFIER_SIZE);
    memcpy(operation.amount, "1000", AMOUNT_SIZE);

    txt_handle(operation, &result);

    ASSERT_EQm(
            "Credit operation must be accepted",
            TXT_OPERATION_SUCCESS,
            result.status
    );

    ASSERT_EQm(
            "Amount must be credited",
            1000.000000,
            result.account.balance
    );

    memset(&operation, 0, OPERATION_SIZE);
    memset(&result, 0, sizeof(result));

    memcpy(operation.operation, TXT_OPERATION_DEBIT_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", ACCOUNT_IDENTIFIER_SIZE);
    memcpy(operation.amount, "342", AMOUNT_SIZE);

    txt_handle(operation, &result);

    ASSERT_EQm(
            "Debit operation must be accepted",
            TXT_OPERATION_SUCCESS,
            result.status
    );

    ASSERT_EQm(
            "Balance must be updated",
            658.000000,
            result.account.balance
    );


    memset(&operation, 0, OPERATION_SIZE);
    memset(&result, 0, sizeof(result));

    memcpy(operation.operation, TXT_OPERATION_READ_STR, OPERATION_CODE_SIZE);
    memcpy(operation.account, "123123123123", ACCOUNT_IDENTIFIER_SIZE);

    txt_handle(operation, &result);

    ASSERT_EQm(
            "Read operation must be accepted",
            TXT_OPERATION_SUCCESS,
            result.status
    );

    PASS();
}
SUITE(suite) {
    RUN_TEST(txt_reject_invalid_parameters);
    RUN_TEST(txt_handle_credit);
    RUN_TEST(txt_handle_debit);
    RUN_TEST(txt_handle_credit_and_debit);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_SUITE(suite);

    GREATEST_MAIN_END();
}
