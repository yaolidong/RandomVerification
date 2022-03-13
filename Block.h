#ifndef BLOCK_H
#define BLOCK_H
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//区块索引
class Block{
public:
	string bPrevHash;//上一个区块的哈希值
    string _bHash;//区块哈希值
    Block();
    Block(string &merkleroot);
	Block(uint32_t bIndexIn, const string & bDataIn,string merkleroot);
	string GetHash();
	uint32_t GetBIndex();
    string GetMerkleRoot() const;
    string CalculateBlockHash() ;//计算区块哈希值


protected:
    string merkle_root;//merkle_root
	int64_t _bNonce;
	string _bData;//区块数据
	time_t _cTime;//区块生成时间
    uint32_t _bIndex = 0;
};


class BigBlock:public Block{
private:

std::vector<Block> vec_blocks;
public:
    uint32_t _bBIndex = 0;
    BigBlock();

    BigBlock(uint32_t bIndexIn, const string & bDataIn,string merkleroot);

//    explicit BigBlock(string & merkleroot);
//    BigBlock(std::vector<Block> vec_blocks);

    void AddMicroBlock(Block & blk);

};
#endif

