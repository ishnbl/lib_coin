#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <sstream> // For std::stringstream
#include <algorithm> // For std::sort
#include <map>       // For std::map
#include "transaction.h"
#include "wallet.h"
#include "blockchain.h"
#include "picosha2.h" // Include picosha2 header

std::vector<Transaction> mem_pool;

std::string merkle_hash(std::vector<std::string> transac_hash){
    if (transac_hash.empty()) return "";
    std::vector<std::string> merkle_tree;
    merkle_tree = transac_hash;
    while (merkle_tree.size() > 1) {
    if (merkle_tree.size() % 2 != 0) {
        merkle_tree.push_back(merkle_tree.back());
    }

    std::vector<std::string> next_level;

    for (size_t i = 0; i < merkle_tree.size(); i += 2) {
        next_level.push_back(picosha2::hash256_hex_string(merkle_tree[i] + merkle_tree[i+1]));
    }

    merkle_tree = next_level;
    }
    return merkle_tree[0];
}

/**
 * @brief Calculates a Merkle root for the current state of all account balances.
 *
 * This function creates a deterministic, cryptographic commitment to the state of the balances map.
 * It does this by:
 * 1. Converting the unordered_map of balances to a sorted vector of pairs to ensure consistent ordering.
 * 2. Serializing each key-value pair into a "key:value" string.
 * 3. Using these strings as leaves to build a Merkle tree.
 *
 * @param balances The map of public keys to their balances.
 * @return A std::string representing the Merkle root hash of the current state.
 */
std::string calculateStateMerkleRoot(const std::unordered_map<std::string, uint64_t>& balances) {
    // Use std::map to automatically sort the keys, ensuring a deterministic order.
    std::map<std::string, uint64_t> sorted_balances(balances.begin(), balances.end());

    std::vector<std::string> balance_hashes;
    for (const auto& pair : sorted_balances) {
        // Create a consistent string representation for each balance entry
        std::string balance_data = pair.first + ":" + std::to_string(pair.second);
        balance_hashes.push_back(picosha2::hash256_hex_string(balance_data));
    }

    // If there are no balances, return a fixed empty hash
    if (balance_hashes.empty()) {
        return picosha2::hash256_hex_string(std::string(""));
    }

    return merkle_hash(balance_hashes);
}


std::string build_header_string(
    uint32_t index,
    const std::string& hash_previous,
    const std::string& pkminer,
    const std::string& merkel_root, // Transaction root
    const std::string& state_root,  // State root
    uint64_t miner_reward,
    uint64_t timestamp
) {
    std::ostringstream oss;
    oss << index << "|"
        << hash_previous << "|"
        << pkminer << "|"
        << merkel_root << "|"
        << state_root << "|"
        << miner_reward << "|"
        << timestamp;
    return oss.str();
}
std::pair<uint64_t, std::string> pow(std::string header) {
    int zeroes = 5;  // 
    uint64_t nonce = 0;
    std::string to_hash;
    std::string hash;
    std::string target_prefix(zeroes, '0');

    while (nonce < 1000000000) { // Stop after 10 million hashes
        to_hash = header + "|" + std::to_string(nonce);
        hash = picosha2::hash256_hex_string(to_hash);  // your existing sha256 function
        if (hash.substr(0, zeroes) == target_prefix) {
            std::cout << "Found nonce: " << nonce << std::endl;
            std::cout << "Hash: " << hash << std::endl;
            break;
        }

        ++nonce;
        if (nonce % 1000000 == 0) {
            std::cout << "Tried: " << nonce << " hashes so far...\r";
            std::cout.flush();
        }
    }
    return std::make_pair(nonce,hash);
}
void begin_mining(std::string pk_miner){
    if(block_chain.empty()){
        Block* genesis = new Block(
            0, // index
            "0", // hash_previous
            {},
            pk_miner,
            "GENESIS_MERKLE_ROOT",
            calculateStateMerkleRoot(balances),
            50, // miner_reward
            0, // nonce
            std::time(nullptr),
            "0" // hash_current
        );
        addBlock(genesis);

    }else{
        uint32_t index = block_chain.size();
        std::string hash_previous = block_chain.back()->hash_current;
        std::string pkminer = pk_miner;

        // --- ROBUST TRANSACTION SELECTION AND STATE CALCULATION ---
        std::unordered_map<std::string, uint64_t> temp_balances = balances;
        std::vector<Transaction> valid_txs_for_block;

        for (const auto& tx : mem_pool) {
            if (temp_balances.count(tx.sender_public_key) && temp_balances.at(tx.sender_public_key) >= tx.amount) {
                temp_balances.at(tx.sender_public_key) -= tx.amount;
                temp_balances[tx.reciever_public_key] += tx.amount; 
                valid_txs_for_block.push_back(tx);
            } else {
                std::cout << "Miner ignoring invalid transaction from: " << tx.sender_public_key << std::endl;
            }
        }
        uint64_t mine_rew = 50;
        temp_balances[pk_miner] += mine_rew;
        std::vector<std::string> hashes_transacs = {};
        for (const auto& tx : valid_txs_for_block) {
            hashes_transacs.push_back(tx.full_transaction_hash);
        }
        std::string merkel_root = merkle_hash(hashes_transacs);

        std::string state_root = calculateStateMerkleRoot(temp_balances);

        uint64_t miner_reward = 50;
        uint64_t timestamp = std::time(nullptr);
        std::string header_str = build_header_string(
            index,
            hash_previous,
            pkminer,
            merkel_root,
            state_root, 
            miner_reward,
            timestamp
        );
        std::pair<uint64_t, std::string> pow_result = pow(header_str);
        uint64_t nonce = pow_result.first;
        std::string hash_current = pow_result.second;
        Block* new_block = new Block(
            index,
            hash_previous,
            valid_txs_for_block,
            pkminer,
            merkel_root,
            state_root,
            miner_reward,
            nonce,
            timestamp,
            hash_current
        );
        addBlock(new_block);
        mem_pool.clear(); // Clear mem_pool after block is mined
    }
}

bool check_pow(std::string block_header, uint64_t nonce, std::string given_hash){
    int zeroes = 5;  // 
    std::string to_hash;
    std::string hash;
    std::string target_prefix(zeroes, '0');
    to_hash = block_header + "|" + std::to_string(nonce);
    hash = picosha2::hash256_hex_string(to_hash);  // your existing sha256 function

if (hash.substr(0, zeroes) == target_prefix&&hash==given_hash) {
    return true;
}else return false;

}