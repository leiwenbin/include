#ifndef ZBASE64_H_
#define ZBASE64_H_

#include <string>

using namespace std;

class ZBase64 {
public:
    static string encode(const string& data);

    static string decode(const string& data);

    static string encodeFromArray(const char* data, size_t len);

    static string encodeStd(const string& data);

    static string decodeStd(const string& data);

    static string encodeStdFromArray(const char* data, size_t len);

private:
    static const string Base64Table;
    static const string::size_type DecodeTable[];

    static const string Base64StdTable;
    static const string::size_type DecodeStdTable[];

};

#endif /* ZBASE64_H_ */
