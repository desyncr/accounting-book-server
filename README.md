# Core

This is the core accouting service. It provides a socket service running on port 8080 (default).
It provides a custom text protocol.

## Running

    $ cd path-to-project
    $ make
    $ make run

## Testing

For testing we use `greatest` library. Using `clibs` for dependency management.\*

Installing dependencies:

    $ cd path-to-project/tests
    $ make deps

Running tests:

    $ make handler && ./handler
    $ make account && ./ account

 \*: You'll need to install clibs yourself using your OS package manager.

## Building

In order to compile the project you'll need gcc and build-tools.

## Container images

You'll may also build container images. You can build them as follow:

    $ make container-build
    $ make container-run

## Protocol

We use a custom text-based protocol.

Note: In these examples we use brackets to denote fields in the request and responses;
these are not part of the protocol.

### Request

The request structure looks as follow:

    [12345678901234567890][001][123123123][123123]
    ^ account/transaction ^ op
                               ^ timestamp
                                          ^ amount

* account/transaction: 20 chars, account or transaction id.
* op: 3 chars, operation code, see below.
* timestamp: 14 chars, timestamp in format: yyyymmddhhmmss
* amount: 6 chars: nnnnnn

### Response

The response looks as the follow:

                                v balance
    v account             v status
    [89618263481836485756][0000][0000850.00]                   < account balance as the header
    [00000005044306110431][0000500.00][0000][0001][1561273704] < processed transaction (if successful)
    ^ transaction id      ^ amount    ^ status
                                            ^ type
                                                  ^ timestamp

### Operations

The following are the operation codes currently supported:

* 100: Debit
* 101: Credit
* 102: Read balance
* 103: Read transaction history

Note: No account creation operations are avaiable as by default accounts are created on-the-fly
if it's not available upon operation.

### Status code

Operations may return one of the following statuses:

* 1000: Unknown account/account not found (unused)
* 1001: Insufficient funds
* 1002: Unknown transaction/transaction not found

* 2000: Invalid amount provided
* 2001: Invalid account number provided
* 2002: Invalid operation code provided
* 2003: Invalid timestamp provided (unused)

## Examples

    echo 8961826348183648575610220190623071137 | nc 127.0.0.1 8080
    8961826348183648575600000000850.00
    000000050443061104310000500.00000000011561273704
    000000060547026503120001000.00000000011561273709
    000000070454084102610000050.00000000011561273712
    000000080511056103030000700.00000000001561273719

