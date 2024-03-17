#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <openssl/sha.h>
#include "base26.cpp"
#include "rsa.cpp"

// TODO: добавить поддержку (?) адишионал дата блоакс

// checksum and VersionNumber always set to 0

// Activation Code
// This is conditional, and if not used should be excluded from the header, rather than being set
// to 0.

unsigned char *HeaderGen06(int dateOfIssue, int machineNumber, int transactionNumber, const char *issuingSystemId, int activationCode)
{
    unsigned char *header = (unsigned char *)malloc(16 * sizeof(char));

    header[0] = header[11] = header[12] = '0';
    header[1] = '6';
    header[13] = issuingSystemId[0];
    header[14] = issuingSystemId[1];
    header[15] = '\0';
    std::bitset<11> bdateOfIssue(dateOfIssue);
    std::bitset<14> bmachineNumber(machineNumber);
    std::bitset<17> btransactionNumber(transactionNumber);
    std::bitset<42> combined;

    for (int i = 0; i < 17; i++)
        combined[i] = btransactionNumber[i];

    for (int i = 0; i < 14; i++)
        combined[i + 17] = bmachineNumber[i];

    for (int i = 0; i < 11; i++)
        combined[i + 17 + 14] = bdateOfIssue[i];

    long num = combined.to_ulong();

    // 1271929255385

    std::string base30UniqueMapping = "BCDEFGHJKLMNPQRSTVWXYZ23456789";
    int i = 0;
    while (num > 0)
    {
        header[10 - i] = base30UniqueMapping[num % 30];

        num /= 30;
        i++;
    }
    // std::cout << header << std::endl;

    return header;
}

std::string sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

int main()
{
    std::vector<uint8_t> ticketPayload = {0x28, 0x41, 0x04, 0x10, 0x41, 0x04, 0x51, 0x55, 0x30, 0xC5, 0x0C, 0x39, 0xDF, 0x92, 0x0A, 0x22, 0xC2, 0xAA, 0xA4, 0x82, 0xA8, 0xA8, 0xC3, 0x00, 0x00, 0x00, 0x08, 0x6E, 0xB6, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x81, 0x30, 0x03, 0xD4, 0x02, 0xE1, 0x80, 0x00, 0x65, 0x2C, 0xC8, 0xA4, 0x9A, 0x8A, 0x87, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    std::string ticketPayloadStr(ticketPayload.cbegin(), ticketPayload.cend());

    std::string hash = sha256(ticketPayloadStr);

    // Convert the string to an unsigned char array
    std::string substr = hash.substr(0, 16);

    for (int i = 0; i < substr.size() / 2; i++)
    {
        std::string byteStr = substr.substr(i * 2, 2);
        ticketPayload.push_back(strtoul(byteStr.c_str(), nullptr, 16));
    }

    std::cout << "Binary Ticket (Hex) with hash 8 bytes appended:" << std::endl;
    for (int i = 0; i < ticketPayload.size(); i++)
    {
        std::cout << std::hex << static_cast<int>(ticketPayload[i]) << "";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    unsigned char encrypted[4098];
    unsigned char decrypted[4098];
    // std::vector<uint8_t> encrypted(4098);

    // unsigned char *ticketPayloadPtr = ticketPayload.data();

    int encryptedLength = encrypt(ticketPayload.size(), &ticketPayload[0], encrypted);
    if (encryptedLength < 0)
    {
        std::cout << "Ecnryption failed." << std::endl;
        return -1;
    }

    // std::cout << "RSA Encrypted Ticket (Hex):" << std::endl;
    // for (int i = 0; i < strlen((const char *)encrypted); i++)
    // {
    //     std::cout << std::hex << static_cast<int>(encrypted[i]);
    // }
    // std::cout << std::endl;
    // std::cout << std::endl;

    int decryptedLength = decrypt(encryptedLength, encrypted, decrypted);
    if (decryptedLength < 0)
    {
        std::cout << "Decryption failed." << std::endl;
        return -1;
    }

    std::cout << "RSA Decrypted Ticket (Hex):" << std::endl;
    for (int i = 0; i < decryptedLength; i++)
    {
        std::cout << std::hex << static_cast<int>(decrypted[i]);
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // size_t *base26_encoded_size = (size_t *)malloc(sizeof(size_t));
    // unsigned char *base26_encoded = base26_encode(encrypted, strlen((const char *)encrypted), base26_encoded_size);
    std::vector<uint8_t> encryptedVec(encrypted, encrypted + strlen((char *)encrypted));

    std::vector<uint8_t> base26_encoded = base26_encode(encryptedVec, encryptedVec.size());
    unsigned char *header = HeaderGen06(592, 4722, 13785, "MS", 0);

    unsigned char *final = (unsigned char *)malloc(((strlen((char *)header) + base26_encoded.size())) * sizeof(char)+1);

    if (final == NULL)
    {
        std::cout << ("Allocation err");
        return -1;
    }
    for (int i = 0; i < strlen((char *)header); i++) // strlen always 15
    {
        final[i] = header[i];
    }

    for (int i = 0; i < base26_encoded.size(); i++)
    {
        final[i + 15] = base26_encoded[i];
    }

    std::cout << "Final Barcode Payload (Including Header):" << std::endl;
    std::cout << final << std::endl;
    free(header);
    free(final);

    return 0;
}

// g++ main.cpp -lcrypto -o main