// #include <iostream>    // streams
// #include <sstream>     // string streams
// #include <string>      // std::string
// #include <cstdint>     // uint32_t, etc.
// #include "transaction.h"
// #include "block.h"
// #include "blockchain.h"

// void write_int32(std::ostream &out, uint32_t value) {
//     out.write(reinterpret_cast<const char*>(&value), sizeof(value));
// }

// void write_int64(std::ostream &out, uint64_t value) {
//     out.write(reinterpret_cast<const char*>(&value), sizeof(value));
// }

// void write_string(std::ostream &out, const std::string &str) {
//     uint32_t len = str.size();
//     write_int32(out, len);
//     out.write(str.data(), len);
// }

// void write_transaction(std::ostream &out, const Transaction &tx) {
//     write_string(out, tx.sender_public_key);
//     write_string(out, tx.reciever_public_key);
//     write_int64(out, tx.timestamp);
//     write_string(out, tx.message);
//     write_string(out, tx.signature);
//     write_int64(out, tx.amount);
//     write_string(out, tx.full_transaction_hash);
// }

// void write_transaction_vector(std::ostream &out, std::vector<Transaction> &transacs_serialize) {
//     write_int32(out, transacs_serialize.size());
//     for (size_t i = 0; i < transacs_serialize.size(); i++) {
//         write_transaction(out, transacs_serialize[i]);
//     }
// }

// void write_block(std::ostream &out, Block* block_serialize) {
//     if (!block_serialize) return;

//     write_int32(out, block_serialize->index);
//     write_string(out, block_serialize->hash_previous);
//     write_transaction_vector(out, block_serialize->transactions_block);

//     write_string(out, block_serialize->pk_miner);
//     write_string(out, block_serialize->merkel_root);
//     write_string(out, block_serialize->state_root);

//     write_int64(out, block_serialize->miner_reward);
//     write_int64(out, block_serialize->nonce);
//     write_int64(out, block_serialize->timestamp);

//     write_string(out, block_serialize->hash_current);
// }

// std::ostream& write_block_chain(std::ostream &out, std::vector<Block*> chain_serialize) {
//     write_int32(out, chain_serialize.size());
//     for (size_t i = 0; i < chain_serialize.size(); i++) {
//         write_block(out, chain_serialize[i]);
//     }
//     return out;
// }

// uint32_t read_int32(std::istream &in) {
//     uint32_t value;
//     in.read(reinterpret_cast<char*>(&value), sizeof(value));
//     return value;
// }

// uint64_t read_int64(std::istream &in) {
//     uint64_t value;
//     in.read(reinterpret_cast<char*>(&value), sizeof(value));
//     return value;
// }

// std::string read_string(std::istream &in) {
//     uint32_t len = read_int32(in);
//     std::string str(len, '\0');
//     in.read(&str[0], len);
//     return str;
// }

// Transaction read_transaction(std::istream &in) {
//     Transaction tx;

//     tx.sender_public_key = read_string(in);
//     tx.reciever_public_key = read_string(in);
//     tx.timestamp = read_int64(in);
//     tx.message = read_string(in);
//     tx.signature = read_string(in);
//     tx.amount = read_int64(in);
//     tx.full_transaction_hash = read_string(in);

//     return tx;
// }

// std::vector<Transaction> read_transaction_vector(std::istream &in) {
//     uint32_t size = read_int32(in);
//     std::vector<Transaction> transactions;
//     transactions.reserve(size);

//     for (uint32_t i = 0; i < size; ++i) {
//         transactions.push_back(read_transaction(in));
//     }

//     return transactions;
// }

// Block* read_block(std::istream &in) {
//     Block* block = new Block();

//     block->index = read_int32(in);
//     block->hash_previous = read_string(in);
//     block->transactions_block = read_transaction_vector(in);

//     block->pk_miner = read_string(in);
//     block->merkel_root = read_string(in);
//     block->state_root = read_string(in);

//     block->miner_reward = read_int64(in);
//     block->nonce = read_int64(in);
//     block->timestamp = read_int64(in);

//     block->hash_current = read_string(in);

//     return block;
// }

// std::vector<Block*> read_block_chain(std::istream &in) {
//     uint32_t size = read_int32(in);
//     std::vector<Block*> blockchain;
//     blockchain.reserve(size);

//     for (uint32_t i = 0; i < size; ++i) {
//         blockchain.push_back(read_block(in));
//     }

//     return blockchain;
// }
