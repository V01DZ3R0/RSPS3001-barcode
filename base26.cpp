#include <iostream>
#include <vector>

std::vector<uint8_t> base26_encode(std::vector<uint8_t> in_bytes, size_t in_length);
std::vector<uint8_t> base26_decode(std::vector<uint8_t> base26_string, size_t in_length);

std::vector<uint8_t> base26_encode(std::vector<uint8_t> in_bytes, size_t in_length)
{

    int accumulator, full_value;
    size_t out_length;

    out_length = in_length * 851 + 499;
    out_length = out_length / 500;

    std::vector<uint8_t> out_string(out_length);

    for (int x = 0; x < out_length; x++)
    {
        accumulator = 0;
        for (int i = in_length - 1; i >= 0; i--)
        {
            full_value = (accumulator * 256) + in_bytes[i];
            accumulator = full_value % 26;
            in_bytes[i] = (full_value - accumulator) / 26;
        }

        out_string[x] = accumulator + 65;
    }

    return out_string;
}
std::vector<uint8_t> base26_decode(std::vector<uint8_t> base26_string, size_t in_length)
{

    std::vector<uint8_t> in_values(in_length);

    int accumulator, full_value;

    for (int i = 0; i < in_values.size(); i++)
    {

        in_values[i] = base26_string[i] - 65;
    }

    /* estimate the result array. Note - this might be oversized by 1 byte */
    size_t out_size = ((in_length * 500) + 850) / 851;
    std::vector<uint8_t> out_bytes(out_size);

    for (int j = 0; j < out_size; j++)
    {
        accumulator = 0;

        for (int i = in_values.size() - 1; i >= 0; i--)
        {

            full_value = accumulator * 26 + in_values[i];
            in_values[i] = full_value / 256;
            accumulator = full_value % 256; /* where & is the bitwise AND operator*/
        }

        out_bytes[j] = accumulator;
    }

    // there may be an extra zero character at the end of this array. Check if the 129th byte is zero, and if so, truncate to 128 bytes.
    if (out_bytes.size() == 129 && out_bytes[129] == 0)
        out_bytes.pop_back();

    return out_bytes;
}

// int main()
// {

//     std::vector<uint8_t> in_bytes = {0x4A, 0x2A, 0x78, 0x52, 0xEB, 0x17, 0xD8, 0x04, 0x93, 0xBF, 0xB5, 0x26, 0x18, 0x7F, 0x55, 0x04, 0x56, 0x42, 0x44, 0x58, 0x1F, 0x60, 0x0D, 0x64, 0x9A, 0xF8, 0x1B, 0x28, 0x62, 0x7B, 0x9D, 0xF0, 0x8F, 0x73, 0x78, 0xBB, 0x4B, 0x0C, 0x9C, 0x75, 0xAF, 0xE0, 0xAF, 0x83, 0x42, 0xA8, 0x61, 0x54, 0xCD, 0xC7, 0xAE, 0x41, 0xBD, 0x51, 0xEF, 0xE2, 0x91, 0xFD, 0x9B, 0x81, 0x6B, 0x92, 0x50, 0x6A, 0x16, 0x22, 0x22, 0x8B, 0x07, 0x3D, 0x08, 0xB9, 0x2F, 0x3C, 0xA8, 0x38, 0x08, 0x38, 0x03, 0x62, 0xA8, 0xF6, 0xC4, 0xD6, 0x08, 0x75, 0xAA, 0x26, 0x82, 0x47, 0x50, 0x48, 0x25, 0xA0, 0x94, 0xCD, 0xE1, 0x17, 0xFC, 0xD4, 0xF3, 0x18, 0x80, 0x90, 0xD8, 0x81, 0xEF, 0x82, 0xDF, 0xF3, 0x10, 0x2F, 0x0D, 0x74, 0x5E, 0x26, 0x9D, 0xBF, 0x98, 0x95, 0xFF, 0xE4, 0x23, 0x9F, 0xFE, 0xB5, 0x1A, 0xB4};
    
//     std::vector<uint8_t> hex(1024);

//     std::vector<uint8_t> str = base26_encode(in_bytes, in_bytes.size());

//     for (int i : str)
//         std::cout << unsigned(i);
//     std::cout << std::endl;
//     // printf("%ld\n",out_str);
//     hex = base26_decode(str, str.size());
//     std::cout << std::endl;

//     for (int i : hex)
//         std::cout << std::hex << i;
//     std::cout << std::endl;
//     // base26_decoding(tmp);

//     return 0;
// }
