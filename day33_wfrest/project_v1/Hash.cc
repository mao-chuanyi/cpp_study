#include "Hash.h"
#include "unixHeader.h"

#include <openssl/evp.h>  // 使用 EVP 接口
#include <iostream>
#include <cstring>        // for memset
using std::cout;
using std::endl;
using std::string;

std::string Hash::sha1() const
{
    int fd = open(_filename.c_str(), O_RDONLY);
    cout << "_filename:" << _filename << endl;
    if(fd < 0) {
        perror("open");
        return string();
    }

    char buff[1024] = {0};
    unsigned char md[EVP_MAX_MD_SIZE] = {0};  // 哈希结果缓冲区
    unsigned int md_len = 0;  // 存储哈希长度

    // 初始化 EVP 上下文
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        // 处理上下文创建失败
        close(fd);  // 确保文件描述符关闭
        return "";
    }

    // 使用 SHA1 算法初始化
    EVP_DigestInit_ex(ctx, EVP_sha1(), nullptr);

    // 循环读取文件并更新哈希计算
    while (true) {
        int ret = read(fd, buff, sizeof(buff));
        if (ret < 0) {
            perror("read");
            EVP_MD_CTX_free(ctx);  // 确保上下文释放
            close(fd);             // 确保文件描述符关闭
            return "";
        }
        if (ret == 0) {
            break;  // 读取结束
        }
        EVP_DigestUpdate(ctx, buff, ret);
        memset(buff, 0, sizeof(buff));  // 清空缓冲区
    }

    // 完成哈希计算
    EVP_DigestFinal_ex(ctx, md, &md_len);

    // 释放 EVP 上下文
    EVP_MD_CTX_free(ctx);

    // 关闭文件描述符
    close(fd);

    // 将哈希结果转换为十六进制字符串
    char fragment[3] = {0};
    string result;
    for (unsigned int i = 0; i < md_len; ++i) {
        sprintf(fragment, "%02x", md[i]);
        result += fragment;
    }

    return result;
}