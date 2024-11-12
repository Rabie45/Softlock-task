#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>

#include <openssl/rand.h>
#include <openssl/err.h>
/**
 * @brief AES encryption/decryption class using OpenSSL
 * Implements AES-256-CBC encryption with password-based key derivation
 */
class AESCipher {
private:
    // Constants for cryptographic parameters
    const int KEY_SIZE = 32;  // AES-256 key size in bytes
    const int IV_SIZE = 16;   // Initialization Vector size in bytes
    const int SALT_SIZE = 8;  // Salt size for key derivation

    // Storage for derived key and initialization vector
    std::vector<unsigned char> key;
    std::vector<unsigned char> iv;

    /**
     * @brief Handles OpenSSL errors by throwing exceptions with detailed error messages
     * @throws std::runtime_error with OpenSSL error description
     */
    void handleErrors() {
        char error_buf[256];
        ERR_error_string_n(ERR_get_error(), error_buf, sizeof(error_buf));
        throw std::runtime_error(error_buf);
    }

public:
    /**
     * @brief Constructor that initializes the cipher with a password
     * @param password The password used for key derivation
     * @throws std::runtime_error if key derivation fails
     */
    AESCipher(const std::string& password) {
        // Initialize OpenSSL cryptographic algorithms
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings();

        // Generate random salt for key derivation
        unsigned char salt[SALT_SIZE];
        if (RAND_bytes(salt, SALT_SIZE) != 1) {
            handleErrors();
        }

        // Allocate space for key and IV
        key.resize(KEY_SIZE);
        iv.resize(IV_SIZE);

        // Derive key and IV from password using PBKDF1
        if (!EVP_BytesToKey(
                EVP_aes_256_cbc(),    // Cipher mode: AES-256-CBC
                EVP_sha256(),         // Hash function: SHA-256
                salt,                 // Salt for key derivation
                reinterpret_cast<const unsigned char*>(password.c_str()),
                password.length(),    // Password and its length
                1,                    // Iteration count
                key.data(),          // Output key
                iv.data()            // Output IV
                )) {
            handleErrors();
        }
    }

    /**
     * @brief Encrypts data using AES-256-CBC
     * @param plaintext Vector containing the data to encrypt
     * @return Vector containing the encrypted data
     * @throws std::runtime_error if encryption fails
     */
    std::vector<unsigned char> encrypt(const std::vector<unsigned char>& plaintext) {
        // Create and initialize encryption context
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            handleErrors();
        }

        // Initialize encryption operation
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            handleErrors();
        }

        // Prepare output buffer (includes space for padding)
        std::vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
        int len1, len2;

        // Encrypt the data
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len1,
                              plaintext.data(), plaintext.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            handleErrors();
        }

        // Finalize encryption and add padding
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len1, &len2) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            handleErrors();
        }

        EVP_CIPHER_CTX_free(ctx);
        ciphertext.resize(len1 + len2);  // Resize to actual encrypted data size
        return ciphertext;
    }

    /**
     * @brief Decrypts data using AES-256-CBC
     * @param ciphertext Vector containing the encrypted data
     * @return Vector containing the decrypted data
     * @throws std::runtime_error if decryption fails
     */
    std::vector<unsigned char> decrypt(const std::vector<unsigned char>& ciphertext) {
        // Create and initialize decryption context
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            handleErrors();
        }

        // Initialize decryption operation
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            handleErrors();
        }

        // Prepare output buffer
        std::vector<unsigned char> plaintext(ciphertext.size());
        int len1, len2;

        // Decrypt the data
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len1,
                              ciphertext.data(), ciphertext.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            handleErrors();
        }

        // Finalize decryption and remove padding
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len1, &len2) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            handleErrors();
        }

        EVP_CIPHER_CTX_free(ctx);
        plaintext.resize(len1 + len2);  // Resize to actual decrypted data size
        return plaintext;
    }

    /**
     * @brief Destructor that cleans up OpenSSL resources
     */
    ~AESCipher() {
        EVP_cleanup();
        ERR_free_strings();
    }
};
