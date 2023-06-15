#include "aes_encrypt.hpp"

// Constructors
AesEncrypt::AesEncrypt()
{
  key_ = "";
}

AesEncrypt::AesEncrypt(const AesEncrypt &copy)
{
  key_ = copy.key_;
}

AesEncrypt::AesEncrypt(std::string key_path)
{
  // open key_path check if file exists
  if (std::filesystem::exists(key_path) == false)
    throw std::runtime_error("Could not find the key file.");
  
  std::ifstream key_file(key_path, std::ios_base::in);

  if (key_file.is_open() == false)
    throw std::runtime_error("Could not open the key file.");

  std::string line;
  while (std::getline(key_file, line))
    this->key_ += line;
  key_file.close();
  // Check length of key
  if (this->key_.length() != 32)
    throw std::runtime_error("The key file does not contain a valid key.");
  
  // Check hexa key
  for (int i = 0; i < this->key_.length(); i++)
  {
    if (!std::isxdigit(this->key_[i]))
      throw std::runtime_error("The key file does not contain a valid key.");
  }

}

AesEncrypt::~AesEncrypt() {}

std::string AesEncrypt::DecryptContent()
{
  return key_;
}

std::string AesEncrypt::EncryptContent()
{
  return key_;
}