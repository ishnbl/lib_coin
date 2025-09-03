#include <iostream>
#include <string>
#include <sstream>
#include "wallet.h"
#include <secp256k1.h>
#include "picosha2.h"
#include <vector>
#include <random>
#include <iomanip>

void hex_str_to_bytes(const std::string& hex, unsigned char* bytes, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (i * 2 + 2 > hex.length()) {
            bytes[i] = 0;
            continue;
        }
        std::string byte_string = hex.substr(i * 2, 2);
        bytes[i] = (unsigned char)strtol(byte_string.c_str(), NULL, 16);
    }
}

void hex_to_bytes(const std::string &hex, unsigned char *bytes) {
    hex_str_to_bytes(hex, bytes, 32);
}

std::string pubkeyGen(std::string privkey_hex){
    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    unsigned char privkey[32];
    hex_to_bytes(privkey_hex,privkey);
    if (!secp256k1_ec_seckey_verify(ctx, privkey)) {
        printf("Invalid private key\n");
        secp256k1_context_destroy(ctx);
        return "";
    }
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, privkey)) {
        printf("Failed to create public key\n");
        secp256k1_context_destroy(ctx);
        return "";
    }
     // Step 5: Serialize the public key (compressed or uncompressed)
    unsigned char output[65];
    size_t output_len = 65;

    // compressed: 33 bytes
    output_len = 33;
    secp256k1_ec_pubkey_serialize(
        ctx,
        output,
        &output_len,
        &pubkey,
        SECP256K1_EC_COMPRESSED
    );

    std::string pubkey_hex;
    char buf[3];
    for (size_t i = 0; i < output_len; i++) {
        snprintf(buf, sizeof(buf), "%02x", output[i]);
        pubkey_hex += buf;
    }

    std::cout << "Public key (compressed) as string: " << pubkey_hex << std::endl;
    secp256k1_context_destroy(ctx);
    return pubkey_hex;
}

std::string sha256_hasher_bytes(std::string string_to_hash){
    std::string hash = picosha2::hash256_hex_string(string_to_hash);
    return hash;
}

std::string sign_generator(std::string mesage, std::string seckey) {
    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    secp256k1_ecdsa_signature signature;
    unsigned char msg32[32];
    unsigned char secret_key[32];
    hex_to_bytes(sha256_hasher_bytes(mesage), msg32);
    hex_to_bytes(seckey, secret_key);
    if (!secp256k1_ecdsa_sign(ctx, &signature, msg32, secret_key, NULL, NULL)) {
        printf("Failed to sign\n");
        secp256k1_context_destroy(ctx);
        return "";
    }
    unsigned char sig[64];
    secp256k1_ecdsa_signature_serialize_compact(ctx, sig, &signature);

    std::string signature_hex;
    char buf[3];
    for (size_t i = 0; i < 64; i++) {
        snprintf(buf, sizeof(buf), "%02x", sig[i]);
        signature_hex += buf;
    }

    printf("Signature (compact): %s\n", signature_hex.c_str());
    secp256k1_context_destroy(ctx);
    return signature_hex;
}

bool sign_verifier(std::string message, std::string signature_hex, std::string pubkey_hex) {
    secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

    // 1. Deserialize signature
    unsigned char sig_bytes[64];
    hex_str_to_bytes(signature_hex, sig_bytes, 64);
    secp256k1_ecdsa_signature sig;
    if (!secp256k1_ecdsa_signature_parse_compact(ctx, &sig, sig_bytes)) {
        printf("Failed to parse signature\n");
        secp256k1_context_destroy(ctx);
        return false;
    }

    // 2. Deserialize public key
    size_t pubkey_len = pubkey_hex.length() / 2;
    if (pubkey_len != 33 && pubkey_len != 65) {
        printf("Invalid public key length\n");
        secp256k1_context_destroy(ctx);
        return false;
    }
    std::vector<unsigned char> pubkey_bytes(pubkey_len);
    hex_str_to_bytes(pubkey_hex, pubkey_bytes.data(), pubkey_len);
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_parse(ctx, &pubkey, pubkey_bytes.data(), pubkey_len)) {
        printf("Failed to parse public key\n");
        secp256k1_context_destroy(ctx);
        return false;
    }

    // 3. Hash the message
    unsigned char msg_hash[32];
    std::string hashed_message = sha256_hasher_bytes(message);
    hex_to_bytes(hashed_message, msg_hash);

    // 4. Verify the signature
    bool is_valid = secp256k1_ecdsa_verify(ctx, &sig, msg_hash, &pubkey) == 1;

    secp256k1_context_destroy(ctx);

    if (is_valid) {
        printf("Signature is valid\n");
    } else {
        printf("Signature is invalid\n");
    }

    return is_valid;
}

std::string generate_random_hex_string() {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(0, 255);

    std::stringstream ss;
    for (int i = 0; i < 32; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << distribution(generator);
    }
    return ss.str();
}