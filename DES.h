#ifndef DES_H
#define DES_H

#include <string>
#include <vector>
using namespace std;

vector<string> generate_key(string keyhex);

string text2hex(string s);

string hex2text(string s);

string encrypt(string ptHex, vector<string> & roundkeys);

string decrypt(string ctHex, vector<string> & roundkeys);

string encryptmsg (string pt, string key);

string decryptmsg (string ct, string key);

#endif