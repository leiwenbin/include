#ifndef MD5_H
#define MD5_H
#ifdef WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#endif

#include <string>
#include <fstream>
#include <string.h>

/* Type define */
typedef unsigned char byte;

using namespace std;
const int MD5_SIZE_PER_READ = 512 * 1024;

/* CMD5 declaration. */
class CMD5 {
public:
    static string GetStringResult(const string& str) {
        CMD5 md5;
        md5.update(str);
        return md5.toString();
    }

    // static string GetFileResult(const string& strfile,unsigned long* pCurrentPos=NULL)
    static string GetFileResult(const string& strfile) {
        //   setlocale(LC_ALL,"");
        // 		ifstream in(strfile.c_str(), ios::in|ios::binary);
        // 		if (!in) {
        // 			return "";
        // 		}
        FILE* f = fopen(strfile.c_str(), "rb");
        if (f == NULL) {
            return "";
        }
        CMD5 md5;
        //		std::streamsize length;
        size_t length;
        bool bSuccess = true;
        /*char *buffer=NULL;
         buffer=new(nothrow) char[MD5_SIZE_PER_READ];*/

        char buffer[MD5_SIZE_PER_READ] = {0};

        /*if (pCurrentPos!=NULL)
         {
         *pCurrentPos=0;
         }*/
        while (/*!in.eof()*/!feof(f)) {
            length = fread(buffer, 1, MD5_SIZE_PER_READ, f);
            // 			in.read(buffer, 1024);
            // 			length = in.gcount();
            /*if (pCurrentPos!=NULL)
             {
             *pCurrentPos+=length;
             }	*/
            if (length > 0) {
                md5.update(buffer, length);
            }
            memset(buffer, 0, MD5_SIZE_PER_READ);
        }
        /*if (buffer!=NULL)
         {
         delete [] buffer;
         buffer=NULL;
         }*/
        //		in.close();
        fclose(f);

        if (!bSuccess) {
            return "";
        }
        return md5.toString();
    }

    CMD5();

    CMD5(const void* input, size_t length);

    CMD5(const string& str);

    CMD5(ifstream& in);

    void update(const void* input, size_t length);

    void update(const string& str);

    void update(ifstream& in);

    const byte* digest();

    string toString();

    void reset();

    unsigned long current;
private:
    void update(const byte* input, size_t length);

    void final();

    void transform(const byte block[64]);

    void encode(const unsigned int* input, byte* output, size_t length);

    void decode(const byte* input, unsigned int* output, size_t length);

    string bytesToHexString(const byte* input, size_t length);

    /* class uncopyable */
    CMD5(const CMD5&);

    CMD5& operator=(const CMD5&);

private:
    unsigned int _state[4];
    /* state (ABCD) */
    unsigned int _count[2];
    /* number of bits, modulo 2^64 (low-order word first) */
    byte _buffer[64];
    /* input buffer */
    byte _digest[16];
    /* message digest */
    bool _finished;
    /* calculate finished ? */

    static const byte PADDING[64];
    /* padding for calculate */
    static const char HEX[16];
    char* memarypool;
};

#endif /*MD5_H*/
