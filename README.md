# lib_coin

A simple, local blockchain implementation created for educational purposes to demonstrate the fundamental concepts of blockchain technology. This project is built from scratch with no networking capabilities... yet!

## Core Features

*   **Proof of Work:** Utilizes a PoW consensus mechanism.
*   **ECDSA Signing:** Ensures transaction integrity and authenticity through Elliptic Curve Digital Signature Algorithm.
*   **Merkle Trees:** Employs Merkle trees to ensure the integrity of the balance model.
*   **Ethereum-style Balance Model:** Implements an account-based balance model, similar to Ethereum, rather than Bitcoin's UTXO model.

## Roadmap

The next major step for this project is to evolve it into a distributed blockchain. The plan is to add networking capabilities using **Boost.Asio** to allow nodes to communicate and maintain a decentralized ledger.

## How to Build

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/ishnbl/lib_coin.git
    cd mybtc
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Run CMake and build:**
    ```bash
    cmake ..
    make
    ```

4.  **Run the executable:**
    The executable, named `mybtc`, will be located in the `build` directory.
    ```bash
    ./mybtc
    ```