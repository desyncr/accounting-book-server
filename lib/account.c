#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "handler.h"
#include "account.h"

struct Account accounts[MAX_ACCOUNTS];
int accounts_idx = 0;

struct Account *txt_create_account(struct Operation operation) {
    struct Account *account = malloc(sizeof(struct Account));

    memcpy(account->number, operation.account, ACCOUNT_IDENTIFIER_SIZE);
    account->active = 1;
    account->balance = 0;
    account->id = accounts_idx;

    accounts[accounts_idx] = *account;
    accounts_idx++;

    return &accounts[account->id];
}

int txt_exists_account(struct Operation operation) {
    // Silly way to retrieve an element from an array
    // (no sorting, not binary search, no nothing)
    for (int i = 0; i <= accounts_idx; i++) {
        if (memcmp(accounts[i].number, operation.account, ACCOUNT_IDENTIFIER_SIZE) == 0) {
            return i;
        }
    }

    return -1;
}

struct Account *txt_get_account(struct Operation operation) {
    int idx;
    if ((idx = txt_exists_account(operation))) {
        return &accounts[idx];
    }

    return 0;
}

#if ALLOW_CREATE_ACCOUNT_ON_THE_FLY == 1
struct Account *txt_get_or_create_account(struct Operation operation) {
    int idx;
    if ((idx = txt_exists_account(operation)) >= 0) {
        return &accounts[idx];
    }

    return txt_create_account(operation);
}
#endif

int txt_view_accounts() {
    for (int i = 0; i <= accounts_idx-1; i++) {
        printf("account #%s\n", accounts[i].number);
        printf("     id     %d\n", accounts[i].id);
        printf("     amount %f\n", accounts[i].balance);
        printf("\n");
    }

    return 0;
}

int txt_init_accounts() {
    memset(accounts, 0, sizeof(accounts));
    accounts_idx = 0;

    return 0;
}
