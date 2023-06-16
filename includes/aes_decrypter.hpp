#ifndef AES_DECRYPTER_HPP
# define AES_DECRYPTER_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

class AesDecrypter
{
  public:
    // Constructors
    AesDecrypter();
    AesDecrypter(const AesDecrypter &copy);
    AesDecrypter(std::string key_path);

    // Destructor
    ~AesDecrypter();

    // Operators
    AesDecrypter &operator=(const AesDecrypter &assign);

    // fonctions membres
    void SetKey(std::string key_path);
    std::string DecryptContent(std::string ct_raw);
  private:
    std::string key_;
};

#endif



