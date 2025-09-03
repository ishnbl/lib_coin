#ifndef WALLET_H
#define WALLET_H

void hex_to_bytes(const std::string& hex, unsigned char* bytes);
std::string pubkeyGen(std::string privkey_hex);
std::string sign_generator(std::string message_toencode, std::string sec_key);
bool sign_verifier(std::string message, std::string signature, std::string pubkey);
std::string generate_random_hex_string();

#endif
