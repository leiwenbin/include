//
// Created by leiwenbin on 16-7-1.
//

#ifndef UTILS_UUID_CONVERT_H
#define UTILS_UUID_CONVERT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <utils/uuid/uuid.h>

typedef struct UUID {
    unsigned char uuid[16];
} UUID_t;

class UUIDConvert {
public:
    UUIDConvert();

    ~UUIDConvert();

    static UUID_t GenerateUUID();

    static std::string UUID2StringLittle(UUID_t& uuid);

    static std::string UUID2StringBig(UUID_t& uuid);

    static UUID_t String2UUID(std::string str_uuid);
};


#endif //UTILS_UUID_CONVERT_H
