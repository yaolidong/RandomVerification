#include "sha256.h"
#include "Block.h"
#include <sstream>
#include <string>
#include <ctime>
using namespace std;

Block::Block(uint32_t bIndexIn,const string & bDataIn, string merkleroot):_bIndex(bIndexIn),_bData(bDataIn){
    merkle_root = merkleroot;
	_bNonce = -1;
	_cTime = time(nullptr);
}

//TODO:
Block::Block(string &merkleroot) {
    _cTime = time(nullptr);
    _bIndex = 0;
    _bNonce = 0;
    _bData = "";
    merkle_root = merkleroot;
}

Block::Block() =default;



string Block::GetHash()
{
	return _bHash;
}

uint32_t Block::GetBIndex() {
    return _bIndex++;
}

 string Block::CalculateBlockHash()
{
	stringstream ss;
	ss<< _bIndex << _cTime << _bData << _bNonce << bPrevHash << merkle_root;
	return sha256(ss.str());

}
string Block::GetMerkleRoot() const {
    return merkle_root;
}


BigBlock::BigBlock() {
    merkle_root = "";
    _bNonce = 0;
    _cTime = time(nullptr);
    _bIndex ++;

}

void BigBlock::SealerBlock() {
    stringstream ss;
    for (auto iter:vec_blocks) {
        ss << iter.GetBIndex();
    }

}

//BigBlock::BigBlock(string & merkleroot) {
//    merkle_root = merkleroot;
//    _bNonce = 0;
//    _cTime = time(nullptr);
//    _bIndex ++;
//
//}

BigBlock::BigBlock(uint32_t bIndexIn, const string &bDataIn, string merkleroot) {
    _bIndex = bIndexIn;
    _bData = bDataIn;
    merkle_root = merkleroot;
    _cTime = time(nullptr);
    _bNonce = -1;

}

