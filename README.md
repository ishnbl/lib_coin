# mybtc: A Simplified Blockchain Implementation in C++

`mybtc` is a foundational command-line interface (CLI) application built in C++ that demonstrates the core concepts of a blockchain. It provides a hands-on understanding of how blocks are structured, transactions are processed, and the chain's integrity is maintained through cryptographic principles like Proof of Work and Merkle Trees.

## ✨ Features

`mybtc` encapsulates several fundamental blockchain functionalities:

*   **Block Structure:**
    *   Each block contains an `index`, `previous_hash`, `timestamp`, `nonce`, `miner_public_key`, `miner_reward`, `transaction_merkle_root`, and `state_merkle_root`.
    *   Blocks are immutably linked via cryptographic hashes, forming a secure chain.
*   **Transactions:**
    *   Support for basic peer-to-peer value transfers (`sender`, `receiver`, `amount`).
    *   Transactions are signed using ECDSA (secp256k1 curve) for authenticity and integrity.
    *   Transactions are collected in a `mem_pool` before being included in a block.
*   **Proof of Work (PoW):**
    *   A simplified PoW mechanism (`find_pow`) is implemented to secure block creation, requiring computational effort to find a valid `nonce` that results in a hash meeting a specific difficulty target (leading zeroes).
*   **Merkle Trees:**
    *   **Transaction Merkle Root:** All transactions within a block are hashed and combined into a single `merkel_root`, ensuring the integrity of the transaction set.
    *   **State Merkle Root:** A `state_merkle_root` is calculated from the current balances of all accounts, providing a cryptographic snapshot of the entire system's state at the time of block creation. This is crucial for validating the global balance sheet.
*   **Blockchain Management:**
    *   **`addBlock` Function:** Implements rigorous validation checks for new blocks, including:
        *   Chain connection (correct index and previous hash).
        *   Proof of Work verification.
        *   Transaction Merkle Root validation.
        *   Transaction signature verification and sufficient funds check.
        *   State Merkle Root validation (ensuring balances are correctly updated).
    *   **Genesis Block:** Automatic creation of the first block in the chain.
*   **Basic Wallet Functionality:**
    *   Generation of new secp256k1 private and public key pairs.
    *   Signing messages/transactions with a private key.
    *   Verifying signatures with a public key.
*   **CLI Interface:**
    *   **Generate Wallet:** Create a new private/public key pair and save the private key to `wallet.dat` (basic persistence).
    *   **Make Transaction:** Create and sign a new transaction, adding it to the mempool.
    *   **Show Balance:** Display the current balance for all known public keys.
    *   **Mine Block:** Initiate the mining process to create a new block, including valid transactions from the mempool and a miner's reward.
    *   **Show Chain:** Beautifully prints the entire blockchain, detailing each block's information and its contained transactions.

## 🚀 Getting Started

To build and run `mybtc`, you'll need `cmake` and a C++ compiler (like `g++`).

1.  **Clone the repository:**
    ```bash
    git clone <repository_url>
    cd mybtc
    ```
2.  **Build the project:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
3.  **Run the application:**
    ```bash
    ./mybtc
    ```

## 🎮 Usage

Once running, `mybtc` will present a simple menu:

```
Enter: 1 for generating a new wallet; 2 for signing a transaction; 3: To show balance, 4 to mint a coin, 5 to show the chain
```

*   **1: Generate Wallet:** Creates a new private key (saved to `wallet.dat`) and displays the corresponding public key.
*   **2: Sign a Transaction:** Prompts for sender/receiver public keys, amount, and uses the private key from `wallet.dat` to sign the transaction.
*   **3: Show Balance:** Lists all known public keys and their current balances.
*   **4: Mint a Coin (Mine Block):** Initiates the mining process. It will attempt to create a new block, include valid transactions from the mempool, and add a miner's reward.
*   **5: Show the Chain:** Prints a detailed, formatted view of every block in the blockchain, including all its transactions.

## 🏗️ Architecture Overview

The project is structured around key blockchain components:

*   **`Block`:** Defines the structure of a block, including its header information and a list of transactions.
*   **`Transaction`:** Represents a value transfer, containing sender/receiver public keys, amount, and cryptographic signatures.
*   **`Blockchain`:** Manages the chain of blocks, including the `addBlock` validation logic and the global `block_chain` and `blockIndex`.
*   **`Wallet`:** Provides utilities for key generation, signing, and verification.
*   **`picosha2`:** A third-party library used for SHA-256 hashing.

## 🔒 Security Considerations & Current Scope

This project is a **simplified educational implementation** and is **not intended for real-world use or production environments.** Key security and robustness features are intentionally omitted or simplified for clarity.

Current limitations include:

*   **No Networking:** This is a single-node, local simulation. There is no peer-to-peer communication.
*   **No Persistence (beyond `wallet.dat`):** The blockchain data (blocks, balances) is stored in memory and is lost when the application closes.
*   **Basic Wallet Security:** Private keys are stored in plaintext in `wallet.dat`. In a real application, these would be encrypted and secured.
*   **No Transaction Fees:** While a miner reward exists, transactions do not currently include fees, which are crucial for incentivizing miners and preventing spam in real networks.
*   **No Transaction Nonce:** Replay attacks are possible as transactions lack a nonce.
*   **Simplified Difficulty:** The Proof of Work difficulty is static.
*   **Limited Error Handling:** Error messages are basic and do not include comprehensive logging.

## 💡 Future Enhancements

Future development could focus on transforming `mybtc` into a more complete and robust blockchain prototype by adding:

*   **Blockchain Persistence:** Saving and loading the entire chain to/from disk.
*   **Networking Layer:** Implementing peer-to-peer communication for block and transaction propagation.
*   **Difficulty Adjustment Algorithm:** Dynamic adjustment of PoW difficulty.
*   **Transaction Fees:** Incorporating transaction fees into the mining reward.
*   **Transaction Nonce:** Adding nonces to transactions to prevent replay attacks.
*   **Improved Wallet Security:** Encrypting private keys and providing more secure storage.
*   **Comprehensive Logging:** Implementing a robust logging system.
*   **UTXO Model:** Exploring an alternative transaction model.

---

Feel free to explore, modify, and learn from `mybtc`!
# lib_coin
