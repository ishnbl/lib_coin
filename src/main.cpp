#include <iostream>
#include <string>
#include <limits>
#include <secp256k1.h>
#include "wallet.h"
#include "picosha2.h"
#include <unordered_map>
#include <cstdint>
#include "transaction.h"
#include "block.h"
#include <vector>
#include <iomanip> // For std::put_time
#include <ctime>   // For std::time_t, std::localtime
#include "blockchain.h"
#include "utils.h"
// #include "serdes.h"
int main() {
    std::vector<Transaction> mem_store;
    // std::string privkey_hex;
    // std::cout <<  "Enter the private key in hex of" << std::endl;
    // std::cin>>privkey_hex;
    // if (privkey_hex.empty()) {
    //     std::cout << "No private key entered. Exiting." << std::endl;
    //     return 1;
    // }
    // std::string pubkey = pubkeyGen(privkey_hex);
    // std::string message = "HOBO DOBO";
    // s td::string signature = sign_generator(message, privkey_hex);
    // std::cout << signature << std::endl;
    // std::string message_manip = "dab dab";
    // bool signVerified = sign_verifier(message, signature, pubkey);
    // bool signVerified2 = sign_verifier(message_manip, signature, pubkey);
    while (true)
    {
        int choice;
        std::cout << "Enter: 1 for generating private and public key; 2 for signing a transaction; 3: To show balance, 4 to mint a coin, 5 to show the chain"<< std::endl;
        std::cin >> choice;
        if(choice==1){
            std::string gen_priv_key = generate_random_hex_string();
            std::string gen_pub_key = pubkeyGen(gen_priv_key);
            std::cout << gen_priv_key << std::endl;
            std::cout << gen_pub_key << std::endl;
        }
        else if (choice==2)
        {
            std::string s_p_k;
            std::string r_p_k;
            std::string s_priv_k;
            std::string amount_str;
            uint64_t amount_transac;
            std::cout << "Tell your public key" << std::endl;
            std::cin >> s_p_k;
            std::cout << "Tell your private key" << std::endl;
            std::cin >> s_priv_k;
            std::cout << "Tell reciever public key" << std::endl;
            std::cin >> r_p_k;
            std::cout << "Amount" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin,amount_str);
            amount_transac = std::stoull(amount_str);
            make_transaction(s_p_k,r_p_k,s_priv_k,amount_transac);
            std::cout << "Transaction added waiting for verification";
        }else if (choice==5)
        {
            if (block_chain.empty()) {
                std::cout << "Blockchain is empty." << std::endl;
            } else {
                std::cout << "\n=== BLOCKCHAIN START ===\n" << std::endl;
                for (const auto& block : block_chain) {
                    std::cout << "--------------------------------------------------------------------------------" << std::endl;
                    std::cout << "Block Index: " << block->index << std::endl;
                    std::cout << "Current Hash: " << block->hash_current << std::endl;
                    std::cout << "Previous Hash: " << block->hash_previous << std::endl;
                    std::cout << "Miner Public Key: " << block->pk_miner << std::endl;
                    std::cout << "Miner Reward: " << block->miner_reward << std::endl;
                    std::cout << "Nonce: " << block->nonce << std::endl;
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
                            std::cout << "    Amount: " << tx.amount << std::endl;
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
        }
        else if (choice==4)
        {
            std::string pk_miner;
            std::cout << "Enter your public key"<< std::endl;
            std::cin >> pk_miner;
            begin_mining(pk_miner);
        }
        else if (choice==3)
        {
                for (const auto& pair : balances) {
                std::cout << pair.first << " => " << pair.second << std::endl;
                }
        }
        
        
        
    }
    
}



