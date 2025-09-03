#include "transaction.h"
#include "block.h"
std::string serializer_chain(std::vector<Block*> chain_serialize);
void print_blockchain(const std::vector<Block*>& chain_rec);