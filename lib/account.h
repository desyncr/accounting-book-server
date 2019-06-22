#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "operation.h"

#define ALLOW_CREATE_ACCOUNT_ON_THE_FLAY 1

struct Account *txt_create_account(struct Operation operation);
int txt_exists_account(struct Operation operation);
struct Account *txt_get_account(struct Operation operation);
#if ALLOW_CREATE_ACCOUNT_ON_THE_FLAY == 1
struct Account *txt_get_or_create_account(struct Operation operation);
#endif
int txt_init_accounts();
int txt_view_accounts();

#endif
