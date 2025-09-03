#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <unordered_map>
#include <string>
#include <vector>
#include "block.h"
extern std::unordered_map<std::string, Block*> blockIndex;
extern std::vector<Block*> block_chain;
void addBlock(Block* block_to_Add);
#endif