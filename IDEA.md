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
0000000000012312312310120190622085100300

1234567890123456781010120190622085100300

# read
1234567890123456789010220190622085100000
1234567890123456781010220190622085100000

# transaction
0000000000003946279110320190622095100000
0002342099212808499010320190622095100000
0002342099212808499010320190622095100000
0001364941267911433010320190622095100000
0003768410188822490010320190622095100000
0002169980535163735010320190622095100000
0004608596959371829010320190622095100000
0001605661165170508010320190622095100000

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

x REST API
- Frontend

