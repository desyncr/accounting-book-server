# Base architecture

x Server

c based socket server. Listens on port x and processes DEBIT and CREDIT:
    - ACCOUNT #
    - OPERATION (DEBIT, CREDIT)
    - AMOUNT
    - TIMESTAMP (Avoid duplication)

(It doesn't handle TRANSFER FROM / TO)

x packet:
    [123123123][001][123123123][123123]
    ^ account  ^ op cod
                    ^ timestamp
                               ^ amount

# debit
1234567890123456789010020190622085100100

# credit
1234567890123456789010120190622085100300

* account: fixed length (lets say, 20 chars)
* operation cod: 3 bytes
* timestamp: fixed length: yyyymmddhhmmss
* amount: fixed length (6): nnnnnn

Operation operation {
    account: char[20],
    operation: char[3],
    timestamp: char[14],
    amount: char[6]
}

`trx_handle(operation op)`

x Core

Transaction handler:
    - Recieves an operation and processes it:
    - it may return success or an error:
        - ERROR_UNKNOWN_ACCOUNT
        - ERROR_INSUFFICIENT_FUNDS

        - ERROR_INVALID_AMOUNT
        - ERROR_INVALID_ACCOUNT_NUMBER
        - ERROR_INVALID_OPERATION
        - ERROR_INVALID_TIMESTAMP


- How to retrieve historic for a given account? / for all accounts?

# API REST

Use any rest api server (sinatra?)

# Frontend

Use bootstrap

## TOOD

x Multi-threading server
x Locking/mutex mechanism

- REST API
- Frontend

