#ifndef AES_ENCRYPT_HPP
# define AES_ENCRYPT_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

class AesEncrypt
{
  public:
    // Constructors
    AesEncrypt();
    AesEncrypt(const AesEncrypt &copy);
    AesEncrypt(std::string key_path);

    // Destructor
    ~AesEncrypt();

    // Operators
    AesEncrypt &operator=(const AesEncrypt &assign);

    // fonctions membres
    std::string DecryptContent();
    std::string EncryptContent();
  private:
    std::string key_;
};

#endif
