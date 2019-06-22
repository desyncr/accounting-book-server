#include "deps/greatest/greatest.h"

#include "../lib/handler.h"
#include "../lib/account.h"

TEST txt_create_new_account(void) {
    struct Operation operation;
    struct Account *account;
    txt_init_accounts();

    memcpy(operation.account, "123123", sizeof(operation.account));

    account = txt_create_account(operation);

    ASSERT_EQm(
            "Must create a new account if number does not exists",
            0,
            account->id
    );

    memcpy(operation.account, "123124", sizeof(operation.account));
    account = txt_create_account(operation);

    ASSERT_EQm(
            "Must create account 1231234 with id 1",
            1,
            account->id
    );

    ASSERT_EQm(
            "Must create account with balance 0",
            0,
            account->balance
    );

    PASS();
}

TEST txt_retrieve_account(void) {
    struct Operation operation;
    struct Account *account;
    txt_init_accounts();

    memcpy(operation.account, "123123", sizeof(operation.account));

    account = txt_create_account(operation);

    ASSERT_EQm(
            "Must create a new account if number does not exists",
            0,
            account->id
    );

    account = txt_get_or_create_account(operation);
    ASSERT_EQm(
            "Must return the previously generated account",
            0,
            account->id
    );

    PASS();
}

TEST txt_update_account(void) {
    struct Operation operation;
    struct Account *account;
    txt_init_accounts();

    memcpy(operation.account, "123123", sizeof(operation.account));

    account = txt_create_account(operation);

    ASSERT_EQm(
            "Must create a new account if number does not exists",
            0,
            account->id
    );

    account->balance = 1000;

    account = txt_get_or_create_account(operation);
    ASSERT_EQm(
            "Must return the previously generated account",
            0,
            account->id
    );

    ASSERT_EQm(
            "Balance must reflect change",
            1000.000000,
            account->balance
    );
    PASS();
}

SUITE(suite) {
    RUN_TEST(txt_create_new_account);
    RUN_TEST(txt_retrieve_account);
    RUN_TEST(txt_update_account);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_SUITE(suite);

    GREATEST_MAIN_END();
}
