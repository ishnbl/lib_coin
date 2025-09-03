#include <iostream>
#include <string>
#include <cstdint>
#include <sstream> // For std::stringstream
#include "transaction.h"
#include "wallet.h"
#include "block.h"
#include "picosha2.h" // Include picosha2 header

// Define global variables declared in transaction.h
std::unordered_map<std::string, uint64_t> balances;
std::vector<Transaction> transactions;

// Helper function to compute the message for signing
// This ensures consistency in what data is signed
std::string computeSigningMessage(const std::string& sender_pk, const std::string& receiver_pk, uint64_t amount) {
    std::stringstream ss;
    ss << sender_pk << receiver_pk << amount;
    return picosha2::hash256_hex_string(ss.str());
}

// Transaction constructor implementation
Transaction::Transaction(std::string spk,std::string rpk,std::string s_priv_k,uint64_t amt) {
    uint64_t timestamp = std::time(nullptr);
    sender_public_key = spk;
    reciever_public_key = rpk;
    amount = amt;
    // Compute the message that was signed
    message = computeSigningMessage(sender_public_key, reciever_public_key, amount);
    signature = sign_generator(message,s_priv_k);
    // Calculate and store the full transaction hash
    full_transaction_hash = calculateTransactionHash(*this);
}

/**
 * @brief Calculates the cryptographic hash of a given Transaction object using SHA-256.
 *
 * This function concatenates key fields of the transaction (sender, recipient, timestamp, message, signature, amount)
 * and then applies the picosha2 SHA-256 hash function to the resulting string.
 *
 * @param tx The Transaction object to hash.
 * @return A std::string representing the SHA-256 cryptographic hash of the transaction in hexadecimal format.
 */
std::string calculateTransactionHash(const Transaction& tx) {
    std::stringstream ss;
    ss << tx.sender_public_key
       << tx.reciever_public_key
       << tx.timestamp // Convert timestamp to a numerical value
       << tx.message // Now the message is computed internally
       << tx.signature
       << tx.amount;

    std::string data_to_hash = ss.str();

    std::string hash_hex_str;
    picosha2::hash256_hex_string(data_to_hash, hash_hex_str);

    return hash_hex_str;
}

void make_transaction(std::string spk,std::string rpk,std::string sig_transac,uint64_t amt){
    mem_pool.emplace_back(spk,rpk,sig_transac,amt);
}

// void verify_transaction(const Transaction& transaction_verify){
//         // The message for verification is now tx.message, which was computed during construction
//         if(sign_verifier(transaction_verify.message,transaction_verify.signature,transaction_verify.sender_public_key)){
//             if (balances[transaction_verify.sender_public_key] >= transaction_verify.amount) {
//                 balances[transaction_verify.reciever_public_key] += transaction_verify.amount;
//                 balances[transaction_verify.sender_public_key] -= transaction_verify.amount;
//                 mem_pool.push_back(transaction_verify);
//             } else {
//                 std::cout << "Insufficient balance" << std::endl;
//             }
//         }

// }

