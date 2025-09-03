#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <string>
#include <chrono>
#include <cstdint>
#include <vector>
#include <unordered_map>

class Transaction{
public:
    std::string sender_public_key;
    std::string reciever_public_key;
    uint64_t timestamp;
    std::string message; // This will be computed internally
    std::string signature;
    uint64_t amount;
    std::string full_transaction_hash;
    Transaction(std::string spk,std::string rpk,std::string s_priv_k,uint64_t amt);
    Transaction() = default;
};

extern std::unordered_map<std::string, uint64_t> balances;
extern std::vector<Transaction> transactions;

void make_transaction(std::string spk,std::string rpk,std::string sig_transac,uint64_t amt);
void verify_transaction(const Transaction& transaction_verify);
std::string calculateTransactionHash(const Transaction& tx);
#endif