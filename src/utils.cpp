#include <string>
#include "transaction.h"
#include "block.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
std::string time_point_to_string(const std::chrono::system_clock::time_point& tp) {
    // convert to time_t (seconds since epoch)
    std::time_t t = std::chrono::system_clock::to_time_t(tp);

    // convert to tm structure
    std::tm tm = *std::localtime(&t);

    // format using stringstream
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

std::string serializerTransaction(Transaction* transaction_to_serialize){
    std::string serialized_string_transac = "";
    serialized_string_transac += transaction_to_serialize->sender_public_key;
    serialized_string_transac += "+";
    serialized_string_transac += transaction_to_serialize->reciever_public_key;
    serialized_string_transac += "+";
    serialized_string_transac += std::to_string(transaction_to_serialize->timestamp);
    serialized_string_transac += "+";
    serialized_string_transac += transaction_to_serialize->message;
    serialized_string_transac += "+";
    serialized_string_transac += transaction_to_serialize->signature;
    serialized_string_transac += "+";
    serialized_string_transac += transaction_to_serialize->full_transaction_hash;
    serialized_string_transac += "+";
    serialized_string_transac += std::to_string(transaction_to_serialize->amount);
    return serialized_string_transac;
}

std::string serializerTransaction_vector(std::vector<Transaction> transactions){
    std::string ser_vec_transac = "@"+std::to_string(transactions.size());
    if (transactions.size()==0)
    {   
        return "";
    }
    
    for (size_t i = 0; i < transactions.size(); i++)
    {
        ser_vec_transac += serializerTransaction(&transactions[i]);
        if (i==transactions.size()-1)
        {
            ser_vec_transac += "@";
            return ser_vec_transac;
        }
        
        ser_vec_transac += "#";
    }
}

std::string serializerBlock(Block* block_to_serialize) {
    std::string s;

    s += std::to_string(block_to_serialize->index);
    s += "|";

    s += block_to_serialize->hash_previous;
    s += "|";

    s += serializerTransaction_vector(block_to_serialize->transactions_block);
    s += "|";

    s += block_to_serialize->pk_miner;
    s += "|";

    s += block_to_serialize->merkel_root;
    s += "|";

    s += block_to_serialize->state_root;
    s += "|";

    s += std::to_string(block_to_serialize->miner_reward);
    s += "|";

    s += std::to_string(block_to_serialize->nonce);
    s += "|";

    s += std::to_string(block_to_serialize->timestamp);
    s += "|";

    s += block_to_serialize->hash_current;

    return s;
}

std::string serializer_chain(std::vector<Block*> chain_serialize){
    std::string final_str = "*^"+std::to_string(chain_serialize.size())+"^";
    if (chain_serialize.size()==0)
    {   
        return "";
    }
    
    for (size_t i = 0; i < chain_serialize.size(); i++)
    {
        final_str += serializerBlock(chain_serialize[i]);
        if (i==chain_serialize.size()-1)
        {
            final_str += "*";
            return final_str;
        }
        
        final_str += "!";
    }
}


void print_blockchain(const std::vector<Block*>& chain_rec) {
    std::cout << "\n=== BLOCKCHAIN START ===\n" << std::endl;
    for (const auto& block : chain_rec) {
        if (!block) continue; // Safety check for null pointers

        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        std::cout << "Block Index: " << block->index << std::endl;
        std::cout << "Current Hash: " << block->hash_current << std::endl;
        std::cout << "Previous Hash: " << block->hash_previous << std::endl;
        std::cout << "Miner Public Key: " << block->pk_miner << std::endl;
        
        // FIX: Use std::dec to ensure the output is in decimal format.
        std::cout << "Miner Reward: " << std::dec << block->miner_reward << std::endl;
        
        // FIX: Use std::dec to ensure the output is in decimal format.
        std::cout << "Nonce: " << std::dec << block->nonce << std::endl;
        
        std::cout << "Merkle Root (Transactions): " << block->merkel_root << std::endl;
        std::cout << "State Root (Balances): " << block->state_root << std::endl;
        
        // Convert timestamp to human-readable format
        std::time_t timestamp_t = block->timestamp;
        std::tm* ltm = std::localtime(&timestamp_t);
        std::cout << "Timestamp: " << std::put_time(ltm, "%Y-%m-%d %H:%M:%S") << std::endl;

        std::cout << "Transactions in Block (" << block->transactions_block.size() << "):" << std::endl;
        if (block->transactions_block.empty()) {
            std::cout << "  (No transactions)" << std::endl;
        } else {
            for (const auto& tx : block->transactions_block) {
                std::cout << "    --------------------" << std::endl;
                std::cout << "    Sender: " << tx.sender_public_key << std::endl;
                std::cout << "    Receiver: " << tx.reciever_public_key << std::endl;
                
                // FIX: Use std::dec to ensure the output is in decimal format.
                std::cout << "    Amount: " << std::dec << tx.amount << std::endl;
                
                std::cout << "    Full Hash: " << tx.full_transaction_hash << std::endl;
                std::cout << "    Message: " << tx.message << std::endl;
                std::cout << "    Signature: " << tx.signature << std::endl;
            }
            std::cout << "    --------------------" << std::endl;
        }
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
    }
    std::cout << "\n=== BLOCKCHAIN END ===\n" << std::endl;
}