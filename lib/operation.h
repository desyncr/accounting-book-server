#ifndef OPERATION_H
#define OPERATION_H

int txt_process_debit_for_account(struct Operation operation, struct Account *account, struct OperationResult *result);
int txt_process_credit_for_account(struct Operation operation, struct Account *account, struct OperationResult *result);
int txt_process_read_for_account(struct Operation operation, struct Account *account, struct OperationResult *result);
int txt_process_read_for_transaction(struct Operation operation, struct Account *account, struct OperationResult *result);

#endif
