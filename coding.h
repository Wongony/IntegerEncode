#ifndef UTIL_CODING_H_
#define UTIL_CODING_H_

#include <cstdint>
#include <string>

namespace util {

char* EncodeVarint(char *dst, uint64_t value)
{
    uint8_t *ptr = reinterpret_cast<uint8_t*>(dst);
    uint8_t base = 128;
    while (value >= base) {
        *(ptr++) = static_cast<uint8_t>(value | base);
	value >>= 7;
    }
    *(ptr++) = static_cast<uint8_t>(value);
    return reinterpret_cast<char*>(ptr);
}

const char* DecodeVarint(const char *src, const char *limit, uint64_t *value)
{
    uint64_t result = 0, byte = 0;
    uint8_t base = 128;
    for (uint8_t shift = 0; shift <= 63 && src < limit; shift += 7) {
        byte = *(reinterpret_cast<const uint8_t*>(src));
	++src;
	if (byte & base) {
	    result |= ((byte & (base-1)) << shift);
	}
	else {
	    result |= (byte << shift);
	    *value = result;
	    return reinterpret_cast<const char*>(src);
	}
    }
    return nullptr;
}

void PutVarint32(std::string *dst, uint32_t value)
{
    char buf[5 + 1] = {0};
    char *ptr = EncodeVarint(buf, value);
    dst->append(buf, ptr - buf);
}

void PutVarint64(std::string *dst, uint64_t value)
{
    char buf[10 + 1] = {0};
    char *ptr = EncodeVarint(buf, value);
    dst->append(buf, ptr - buf);
}

bool GetVarint64(std::string *src, uint64_t *value)
{
    const char *begin = src->data();
    const char *end = begin + src->size();
    const char *cur = DecodeVarint(begin, end, value);
    if (cur == nullptr) {
        return false;
    }
    *src = std::string(cur, end - cur);
    return true;
}

void PutZigZag32(std::string *dst, int32_t value)
{
    uint32_t v = (value << 1) ^ (value >> 31);
    PutVarint32(dst, v);
}

void PutZigZag64(std::string *dst, int64_t value)
{
    uint64_t v = (value << 1) ^ (value >> 63);
    PutVarint64(dst, v);
}

bool GetZigZag64(std::string *src, int64_t *value)
{
    uint64_t result = 0;
    bool ret = GetVarint64(src, &result);
    if (ret) {
       *value = (result >> 1) ^ (-(result & 1)); 
    }
    return ret;
}

}  //namespace util


#endif
