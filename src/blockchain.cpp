#include <unordered_map>
#include <iostream>
#include <string>
#include "block.h"
#include "transaction.h"
#include "wallet.h"
std::unordered_map<std::string, Block*> blockIndex;
std::vector<Block*> block_chain;

void addBlock(Block* block_to_add){
    // In a real implementation, you would have a more robust way to get the previous block
    if (block_chain.empty() && block_to_add->index == 0) { // Genesis block case
        // Genesis block might have different validation rules
        // For now, we accept it as is.
        balances[block_to_add->pk_miner] += block_to_add->miner_reward;
        block_chain.push_back(block_to_add);
        blockIndex[block_to_add->hash_current] = block_to_add;
        return;
    }

    // Get the previous block for validation
    const Block* previous_block = block_chain.back();

    // === VALIDATION CHECKS ===

    // 1. Check connection to the chain
    if (block_to_add->index != previous_block->index + 1) {
        std::cout << "Validation Failed: Invalid block index." << std::endl;
        return;
    }
    if (block_to_add->hash_previous != previous_block->hash_current) {
        std::cout << "Validation Failed: Previous block hash does not match." << std::endl;
        return;
    }

    // 2. Reconstruct the header string to verify the Proof of Work
    std::string header_to_check = build_header_string(
        block_to_add->index,
        block_to_add->hash_previous,
        block_to_add->pk_miner,
        block_to_add->merkel_root,
        block_to_add->state_root,
        block_to_add->miner_reward,
        block_to_add->timestamp
    );
    // 3. Check Proof of Work
    // We need a check_pow function here. Assuming it exists and works as discussed.
    if (!check_pow(header_to_check, block_to_add->nonce, block_to_add->hash_current)) {
        std::cout << "Validation Failed: Invalid Proof of Work." << std::endl;
        return;
    }

    // 4. Validate Transaction Merkle Root
    std::vector<std::string> tx_hashes;
    for (const auto& tx : block_to_add->transactions_block) {
        tx_hashes.push_back(tx.full_transaction_hash);
    }
    std::string calculated_merkle_root = merkle_hash(tx_hashes);

    if (calculated_merkle_root != block_to_add->merkel_root) {
        std::cout << "Validation Failed: Transaction Merkle Root does not match." << std::endl;
        return;
    }

    std::unordered_map<std::string, uint64_t> temp_balances = balances;
    std::vector<Transaction> valid_txs_for_block;
    for (size_t i = 0; i < block_to_add->transactions_block.size(); i++)
    {
        if(!sign_verifier(block_to_add->transactions_block[i].message,block_to_add->transactions_block[i].signature,block_to_add->transactions_block[i].sender_public_key)){
            std::cout << "Transaction verif failed" << std::endl;
            // If signature verification fails, the transaction is invalid.
            // In a real system, this would likely lead to the block being rejected or the transaction being excluded.
            // For now, we'll just skip it and ensure the block doesn't contain invalid transactions.
            continue;
        };
        if (temp_balances.count(block_to_add->transactions_block[i].sender_public_key) && temp_balances.at(block_to_add->transactions_block[i].sender_public_key) >= block_to_add->transactions_block[i].amount) {
                temp_balances.at(block_to_add->transactions_block[i].sender_public_key) -= block_to_add->transactions_block[i].amount;
                temp_balances[block_to_add->transactions_block[i].reciever_public_key] += block_to_add->transactions_block[i].amount;
                valid_txs_for_block.push_back(block_to_add->transactions_block[i]);
        } else {
                std::cout << "Miner ignoring invalid transaction from: " << block_to_add->transactions_block[i].sender_public_key << std::endl;
                // If balance is insufficient, the transaction is invalid.
                continue;
        }
    }

    // 5. Validate all transactions in the block were valid
    if (valid_txs_for_block.size() != block_to_add->transactions_block.size()) {
        std::cout << "Validation Failed: Block contains invalid transactions." << std::endl;
        return;
    }

    // Now that all transactions are valid, let's add the miner's reward to the temporary balances
    temp_balances[block_to_add->pk_miner] += block_to_add->miner_reward;

    // 6. Validate State Root (after applying all transactions and miner reward)
    if (calculateStateMerkleRoot(temp_balances) != block_to_add->state_root) {
        std::cout << "Validation Failed: State Merkle Root does not match." << std::endl;
        return;
    }

    // === ALL VALIDATION PASSED ===
    // Update the global state with the validated temporary balances
    balances = temp_balances;

    // Add the block to the chain
    block_chain.push_back(block_to_add);
    blockIndex[block_to_add->hash_current] = block_to_add;

    std::cout << "Block successfully added." << std::endl;
}