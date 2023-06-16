#include "aes_decrypter.hpp"
#include "matt_daemon.hpp"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

// Constructors
AesDecrypter::AesDecrypter()
{
  key_ = "";
}

AesDecrypter::AesDecrypter(const AesDecrypter &copy)
{
  key_ = copy.key_;
}

AesDecrypter::AesDecrypter(std::string key_path)
{
  this->SetKey(key_path);
}

AesDecrypter::~AesDecrypter() {}


void hexstr_touchar(std::string ct_raw, unsigned char *ct)
{
  for (int i = 0; i < ct_raw.length()/2; i++)
  {
    std::string byte = ct_raw.substr(i * 2, 2);
    ct[i] = (unsigned char)strtol(byte.c_str(), NULL, 16);
  }

}


/**
 * @brief This function will initialize the key for the AES decryption.
 * 
 * @param key_path The path to the key file.
 * @return void
*/
void AesDecrypter::SetKey(std::string key_path)
{
  // open key_path check if file exists
  if (std::filesystem::exists(key_path) == false)
    throw std::runtime_error("Could not find the key file.");
  
  std::ifstream key_file(key_path, std::ios_base::in);

  if (key_file.is_open() == false)
    throw std::runtime_error("Could not open the key file.");

  
  std::string line;
  std::getline(key_file, line); // get only the first line
  this->key_ = line.c_str();

  key_file.close();

  // Check length of key
  if (this->key_.length() != 16)
    throw std::runtime_error("The key file does not contain a valid key.");
  
  // Check hexa key
  for (int i = 0; i < this->key_.length(); i++)
  {
    if (!std::isxdigit(this->key_[i]))
      throw std::runtime_error("The key file does not contain a valid key.");
  }
}

/**
 * @brief This function will decrypt an encrypted content and return the decrypted content.
 * 
 * @param ct_raw 
 * @return std::string 
 */
std::string AesDecrypter::DecryptContent(std::string ct_raw)
{
  EVP_CIPHER_CTX *ctx;

  int len;
  int plaintext_len;
  unsigned char plaintext[BUFFER_LEN];
  int ciphertext_len = ct_raw.length() / 2;
  unsigned char *ciphertext = new unsigned char[ciphertext_len];

  memset(plaintext, 0, BUFFER_LEN);
  hexstr_touchar(ct_raw, ciphertext);

  if (!(ctx = EVP_CIPHER_CTX_new())) // Init context for decryption
    throw std::runtime_error("Error on decryption phase, please verify your ciphertext.");

  if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (unsigned char *)this->key_.c_str(), NULL) != 1) // Init the decryption operation. (check of key, init ctx with good algo etc...)
    throw std::runtime_error("Error on decryption phase, please verify your ciphertext.");

  if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len) != 1) // Provide the ciphertext bytes to be decrypted.
    throw std::runtime_error("Error on decryption phase, please verify your ciphertext.");
  plaintext_len = len;

  if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) // finalize the decryption process. (unpadding etc...)
    throw std::runtime_error("Error on decryption phase, please verify your ciphertext.");
  plaintext_len += len;


  EVP_CIPHER_CTX_free(ctx); // clean up the context.

  plaintext[plaintext_len] = '\0';
  std::string result(reinterpret_cast<char*>(plaintext));

  return result;
}