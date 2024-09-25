#include "Token.h"
#include "unixHeader.h"

#include <openssl/md5.h>
#include <openssl/evp.h>

#include <iostream>

string Token::genToken() const
{
    // 生成 MD5 哈希值
    std::string tmp = _salt + _username;
    unsigned char md[EVP_MAX_MD_SIZE] = {0};  // MD5 输出的长度
    unsigned int md_len = 0;

    // 初始化 EVP 上下文
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        // 处理初始化失败的情况
        return "";
    }

    // 使用 MD5 算法初始化
    EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);
    EVP_DigestUpdate(ctx, tmp.c_str(), tmp.size());
    EVP_DigestFinal_ex(ctx, md, &md_len);

    // 释放 EVP 上下文
    EVP_MD_CTX_free(ctx);

    // 将 MD5 结果转换为十六进制字符串
    char fragment[3] = {0};
    std::string result;
    for (unsigned int i = 0; i < md_len; ++i) {
        sprintf(fragment, "%02x", md[i]);
        result += fragment;
    }

    return result;
    // time_t secs = time(nullptr) + 3600;//过期时间为1小时
    // struct tm  * ptm = localtime(&secs);
    // char buff[15] = {0};
    // sprintf(buff, "%04d%02d%02d%02d%02d%02d",
    //         ptm->tm_year + 1900,
    //         ptm->tm_mon + 1,
    //         ptm->tm_mday,
    //         ptm->tm_hour,
    //         ptm->tm_min,
    //         ptm->tm_sec);
    // return result + buff;
}
