#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
// implying *.pem key files are "public.pem" & "private.pem"

// openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 -outform PEM -out testprivate_key.pem
// openssl rsa -in testprivate_key.pem -inform PEM -pubout -out testpublic_key.pem -outform PEM

// Function to encrypt the data
int encrypt(int data_len, unsigned char *data, unsigned char *encrypted);
int decrypt(int data_len, unsigned char *enc_data, unsigned char *decrypted);

int encrypt(int data_len, unsigned char *data, unsigned char *encrypted) // private_encrypt, sign_private if you will
{

    RSA *privateRSA = NULL;
    FILE *privateKeyFile;
    privateKeyFile = fopen("private.pem", "rb");
    if (privateKeyFile == NULL)
    {
        printf("Unable to open private key file\n");
        return -1;
    }

    privateRSA = PEM_read_RSAPrivateKey(privateKeyFile, NULL, NULL, NULL);
    if (privateRSA == NULL)
    {
        printf("Unable to read private key file\n");
        return -1;
    }
    // Load the RSA keys

    int result = RSA_private_encrypt(data_len, data, encrypted, privateRSA, RSA_PKCS1_PADDING);

    // Free the memory
    RSA_free(privateRSA);
    fclose(privateKeyFile);

    return result;
}

// Function to decrypt the data
int decrypt(int data_len, unsigned char *enc_data, unsigned char *decrypted)
{

    RSA *publicRSA = NULL;
    FILE *publicKeyFile = fopen("public.pem", "rb");
    if (publicKeyFile == NULL)
    {
        printf("Unable to open public key file\n");
        return -1;
    }

    publicRSA = PEM_read_RSA_PUBKEY(publicKeyFile, NULL, NULL, NULL);
    if (publicRSA == NULL)
    {
        printf("Unable to read public key file\n");
        return -1;
    }

    int result = RSA_public_decrypt(data_len, enc_data, decrypted, publicRSA, RSA_PKCS1_PADDING);

    RSA_free(publicRSA);
    fclose(publicKeyFile);

    return result;
}

// int main()
// {

//     unsigned char data[] = "Hello, RSA!";
//     unsigned char encrypted[4098] = {};
//     unsigned char decrypted[4098] = {};

//     int encrypted_length = encrypt(strlen((char *)data), data, encrypted);
//     if (encrypted_length == -1)
//     {
//         printf("Public Encrypt failed\n");
//         return -1;
//     }
//     int decrypted_length = decrypt(encrypted_length, encrypted, decrypted);

//     if (decrypted_length == -1)
//     {
//         printf("Private Decrypt failed\n");
//         return -1;
//     }

//     printf("%s\n", decrypted);

//     return 0;
// }