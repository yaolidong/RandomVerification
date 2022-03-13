//
// Created by 姚黎东 on 2021/4/19.
//缓存池是否为空
//计算Merkle root
//获取交易数量
//区块打包上链
//

#ifndef K_CA_SEALER_H
#define K_CA_SEALER_H

#include "Cache.h"
#include "Blockchain.h"

class Sealer{
public:
    std::string merkle_root;
    Sealer();
     bool IsCacheEmpty(Cache & ca) ;
    void CalculateMerkRoot(Cache & ca);
    BigBlock Upchain(Blockchain & bc);
    size_t GetTransCount() const;

private:
    std::string str;
    size_t trans_count = 0;

};
#endif //K_CA_SEALER_H
