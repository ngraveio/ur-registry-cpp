#include "uuid.h"

#include <algorithm>

using namespace std;

constexpr char nil_uid[] = "00000000-0000-0000-0000-000000000000";

Uuid::Uuid() : Uuid(nil_uid) {

}

static string byte_to_hex(uint8_t byte) {
    auto hex = "0123456789abcdef";
    string result;
    result.append(1, hex[(byte >> 4) & 0xF]);
    result.append(1, hex[byte & 0xF]);
    return result;
}

static uint8_t hex_digit_to_bin(char hex) {
    if (hex >= '0' && hex <= '9')
        return hex - '0';

    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;

    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;

    return 0;
}

static ur::ByteVector hex_to_data(const string& hex) {
    ur::ByteVector result;

    auto len = hex.length();
    if(len % 2 != 0) {
        return {};
    }
    auto count = len / 2;
    result.reserve(count);
    for(size_t i = 0; i < count; i++) {
        auto b1 = hex_digit_to_bin(hex[i * 2]);
        auto b2 = hex_digit_to_bin(hex[i * 2 + 1]);
        result.push_back((b1 << 4) | b2);
    }

    return result;
}


const string Uuid::str() const {

    if(_data.empty())
        return "";

    const bool seps[] = { false, false, false, true, false, true, false, true, false, true, false, false, false, false, false, false };
    string result;

    for(int i = 0; i < 16; i++) {
        result.append(byte_to_hex(_data[i]));
        if(seps[i]) {
            result.append("-");
        }
    }
    return result;
}

static bool check_uuid(const ur::ByteVector& data) {
    if(data.size() != UUID_SIZE) {
        return false;
    }
    return true;
}

Uuid::Uuid(const string& uuid_str) {

    if(uuid_str.empty()) {
        _data = ur::ByteVector{};
        return;
    }

    string uuid_hexstr = uuid_str;
    uuid_hexstr.erase(remove(uuid_hexstr.begin(), uuid_hexstr.end(), '-'), uuid_hexstr.end());
    auto data = hex_to_data(uuid_hexstr);
    if (check_uuid(data))
        _data = data;
}

Uuid::Uuid(const ur::ByteVector& data) {
    if (check_uuid(data))
        _data = data;
}
