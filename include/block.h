#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <vector>
#include <cstdint>
#include "transaction.h"
class Block{
public:
    uint32_t index;
    std::string hash_previous;
    std::vector<Transaction> transactions_block;
    std::string pk_miner;
    std::string merkel_root; // Transaction Merkle Root
    std::string state_root;  // State Merkle Root
    uint64_t miner_reward;
    uint64_t nonce;
    uint64_t timestamp;
    std::string hash_current;
    Block() = default;
    // Constructor
    Block(
        uint32_t index,
        const std::string& hash_previous,
        const std::vector<Transaction>& transactions_block,
        const std::string& pk_miner,
        const std::string& merkel_root,
        const std::string& state_root,
        uint64_t miner_reward,
        uint64_t nonce,
        uint64_t timestamp,
        const std::string& hash_current
    ) : index(index),
        hash_previous(hash_previous),
        transactions_block(transactions_block),
        pk_miner(pk_miner),
        merkel_root(merkel_root),
        state_root(state_root),
        miner_reward(miner_reward),
        nonce(nonce),
        timestamp(timestamp),
        hash_current(hash_current) {}
};

class Block_headers{
public:
    uint32_t index;
    std::string hash_previous;
    std::string pk_miner;
    std::string merkel_root;
    uint64_t miner_reward;
    uint64_t timestamp;   

};

#include <utility> // For std::pair

void begin_mining(std::string pk_miner);
bool check_pow(std::string block_header, uint64_t nonce, std::string given_hash);
std::pair<uint64_t, std::string> find_pow(std::string header);
std::string build_header_string(
    uint32_t index,
    const std::string& hash_previous,
    const std::string& pkminer,
    const std::string& merkel_root, // Transaction root
    const std::string& state_root,  // State root
    uint64_t miner_reward,
    uint64_t timestamp
);
std::string merkle_hash(std::vector<std::string> transac_hash);
std::string calculateStateMerkleRoot(const std::unordered_map<std::string, uint64_t>& balances);
extern std::vector<Transaction> mem_pool;
#endif