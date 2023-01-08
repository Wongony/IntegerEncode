#include <cstdio>

#include <iostream>
#include <iomanip>

#include "coding.h"

int main(int argc, char *argv[])
{
    std::string str_n;
    uint32_t n = 150;
    if (argc >= 2) {
	n = std::stoul(argv[1], nullptr, 0);
    }
    util::PutVarint32(&str_n, n);
    for (auto i = 0; i < str_n.size(); ++i) {
    	printf("[%u]: 0x%02hhx\n", i, str_n[i]);
    }
    uint64_t out_n;
    util::GetVarint64(&str_n, &out_n);
    printf("n : %lu, str_n.size:%lu\n", out_n, str_n.size());

    std::string str_m;
    int32_t m = 250;
    if (argc >= 3) {
        m = std::stoi(argv[2], nullptr, 0);
    }
    util::PutZigZag32(&str_m, m);
    for (auto i = 0; i < str_m.size(); ++i) {
    	printf("[%u]: 0x%02hhx\n", i, str_m[i]);
    }
    int64_t out_m;
    util::GetZigZag64(&str_m, &out_m);
    printf("m : %ld, str_m.size:%lu\n", out_m, str_m.size());

    return 0;
}
