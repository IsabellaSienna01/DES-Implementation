#include "DES.h"
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#define ull unsigned long long
using namespace std;

string text2hex (string s){
    string hex = "0123456789ABCDEF";
    string res = "";

    for (unsigned char c : s){
        res += hex[c/16];
        res += hex[c%16];
    }
    return res;
}

string hex2text(string s){
    string res = "";

    for (int i = 0; i<s.size(); i+=2){
        string byte_str = s.substr(i, 2);
        char c = char(stoi(byte_str, nullptr, 16));

        res += c;
    }
    return res;
}

string hex2bin(string s){
    map<char, string> hex = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };
    string bin = "";
   for (char c : s){
        if (hex.find(c) == hex.end()){
            throw invalid_argument("Invalid hexadecimal character");
        }
        bin += hex[c];
   }
    return bin;
}

string bin2hex (string s){
    map <string, char> bin = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'}, {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'}, {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'}, {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'},
        {"1111", 'F'}
    };
    string hex = "";
    for (int i = 0; i<s.size(); i += 4){
        string temp = "";
        temp += s[i];
        temp += s[i+1];
        temp += s[i+2];
        temp += s[i+3];
        hex += bin[temp];
    }
    return hex;
}

ull bin2dec (string s){
    ull decimal = 0;
    for (char bit: s){
        decimal = decimal * 2 + (bit - '0');
    }
    return decimal;
}

string dec2bin (ull n){
    string res = "";

    for (int i = 0; i < 4; i++) {
        res = char((n % 2) + '0') + res;
        n /= 2;
    }

    return res;
}

string permute (string k, int table[], int sz){
    string res = "";
    for (int i= 0; i<sz; i++){
        res += k[table[i] - 1];
    }
    return res;
}

string shift_left (string k, int shifts){
    string s = "";
    for (int i = 0; i<shifts; i++){
        rotate(k.begin(), k.begin() + 1, k.end());
    }
    return k;
}

string xor_op(string a, string b){
    string ans = "";
    for (int i = 0; i<a.size(); i++){
        if (a[i] == b[i]){
            ans += "0";
        }
        else{
            ans += "1";
        }
    }
    return ans;
}

int initial_perm[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                    60, 52, 44, 36, 28, 20, 12, 4,
                    62, 54, 46, 38, 30, 22, 14, 6,
                    64, 56, 48, 40, 32, 24, 16, 8,
                    57, 49, 41, 33, 25, 17, 9, 1,
                    59, 51, 43, 35, 27, 19, 11, 3,
                    61, 53, 45, 37, 29, 21, 13, 5,
                    63, 55, 47, 39, 31, 23, 15, 7};

int exp_dbox[48] = {32, 1, 2, 3, 4, 5, 4, 5,
            6, 7, 8, 9, 8, 9, 10, 11,
            12, 13, 12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21, 20, 21,
            22, 23, 24, 25, 24, 25, 26, 27,
            28, 29, 28, 29, 30, 31, 32, 1};

int perm_table[32] = {16, 7, 20, 21,
            29, 12, 28, 17,
            1, 15, 23, 26,
            5, 18, 31, 10,
            2, 8, 24, 14,
            32, 27, 3, 9,
            19, 13, 30, 6,
            22, 11, 4, 25};

int sbox[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },

    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },

    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },

    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },

    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },

    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },

    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },

    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

int final_perm[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                    39, 7, 47, 15, 55, 23, 63, 31,
                    38, 6, 46, 14, 54, 22, 62, 30,
                    37, 5, 45, 13, 53, 21, 61, 29,
                    36, 4, 44, 12, 52, 20, 60, 28,
                    35, 3, 43, 11, 51, 19, 59, 27,
                    34, 2, 42, 10, 50, 18, 58, 26,
                    33, 1, 41, 9, 49, 17, 57, 25};

int keyp[56] = {
    57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

int shift_table[16] = {
    1, 1, 2, 2,
	2, 2, 2, 2,
	1, 2, 2, 2,
	2, 2, 2, 1
};

int key_comp_table[48] = {
    14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32  
};

vector<string> generate_key (string key){
    if (key.size() != 16){
        throw invalid_argument("DES key must contain exactly 16 hexadecimal characters");
    }

    key = hex2bin(key);
    key = permute(key, keyp, 56);

    string left = key.substr(0, 28);
    string right = key.substr(28, 28);
    
    string combine_str = "";
    string round_key = "";
    vector<string> roundkeys;

    for (int i = 0; i<16; i++){
        left = shift_left(left, shift_table[i]);
        right = shift_left(right, shift_table[i]);

        combine_str = left + right;
        round_key = permute(combine_str, key_comp_table, 48);

        roundkeys.push_back(round_key);
    }

    return roundkeys;
}

string processDES(string hex, vector<string> & roundkeys){
    if (hex.size() != 16){
        throw invalid_argument("DES block must contain exactly 16 hexadecimal characters");
    }
    if (roundkeys.size() != 16){
        throw invalid_argument("DES requires exactly 16 round keys");
    }

    string bin = hex2bin(hex);

    bin = permute(bin, initial_perm, 64);

    string left = bin.substr(0, 32);
    string right = bin.substr(32, 32);

    for (int i = 0; i<16; i++){
        string right_expanded = permute(right, exp_dbox, 48);
        string xor_str = xor_op(right_expanded, roundkeys[i]);
        string sbox_str = "";

        for (int j = 0; j < 8; j++){
            string curr = xor_str.substr(j*6, 6);

            string row_bits = "";
            row_bits += curr[0];
            row_bits += curr[5];

            string col_bits = "";
            col_bits = curr.substr(1, 4);

            int row = bin2dec(row_bits);
            int col = bin2dec(col_bits);

            int val = sbox[j][row][col];
            sbox_str += dec2bin(val);
        }
        sbox_str = permute(sbox_str, perm_table, 32);

        string result = xor_op(left, sbox_str);
        left = result;

        string tmp = left;
        if (i != 15){
            left = right;
            right = tmp;
        }
    }
    string combine = left + right;

    string outputbin = permute(combine, final_perm, 64);

    return bin2hex(outputbin);
}

string encrypt(string ptHex, vector<string> & roundkeys){
    return processDES(ptHex, roundkeys);
}

string decrypt(string ctHex, vector<string> & roundkeys){
    vector<string> roundkeys_rev =roundkeys;
    reverse(roundkeys_rev.begin(), roundkeys_rev.end());

    return processDES(ctHex, roundkeys_rev);
}

string add_padding (string s){
    int padding = 8 - (s.size() % 8);

    for (int i = 0; i<padding; i++){
        s += char(padding);
    }
    return s;
}

string remove_padding (string s){
    if (s.empty()){
        return s;
    }
    int padding = (unsigned char) s.back();

    if (padding < 1 || padding > 8){
        throw invalid_argument("Invalid padding");
    }
    for (int i = 0; i<padding; i++){
        if ((unsigned char) s[s.size() - 1 - i] != padding){
            throw invalid_argument("Invalid padding");
        }
    }
    s.erase(s.size() - padding);

    return s;
}

string encryptmsg(string pt, string key){
    vector<string> roundkeys = generate_key(key);

    pt = add_padding(pt);

    string ct = "";

    for (int i = 0; i<pt.size(); i+=8){
        string block = pt.substr(i, 8);

        string blockHex =text2hex(block);

        blockHex = encrypt(blockHex, roundkeys);

        ct += blockHex;
    }
    return ct;
}

string decryptmsg(string ct, string key){
    if (ct.empty() || ct.size() % 16 != 0){
        throw invalid_argument("Invalid ciphertext length");
    }
    vector <string> roundkeys = generate_key(key);

    string pt = "";

    for (int i = 0; i<ct.size(); i+=16){
        string block = ct.substr(i, 16);

        string decrypted = decrypt(block, roundkeys);

        decrypted = hex2text(decrypted);

        pt += decrypted;
    }
    pt = remove_padding(pt);
    
    return pt;
}