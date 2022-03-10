//
// Created by 姚黎东 on 2022/3/3.
//

#ifndef K_CA_COMMITTEE_H
#define K_CA_COMMITTEE_H

//#include "Message.h"
#include "Node.h"
#include <algorithm>
#include <random>



class Committee{
private:
    int number_members;
    int number_trans;
    int sequence;
    std::vector<network_address_t> _members;
    network_address_t whoismaster;
public:

    Committee();
    int GetCommitteeSeq();
    std::vector<network_address_t> GetMembers();
    Committee(int num_members, int seq, network_address_t wism);
    std::vector<network_address_t>& GetCommitteeMembers();
    network_address_t GetLeaderAddress();
    void ShuffleNode(std::vector<std::unique_ptr<Node>> &nodes);




};


class ConsensusCommittee:public Committee{
private:
    vector<Block>vec_blocks;
    BigBlock bigBlock;
public:
    static ConsensusCommittee & instance();
    ConsensusCommittee();
    ConsensusCommittee(int num_members, int seq, network_address_t wism);
    void PBFT(Message msg);
    void SealerBlock(std::vector<std::unique_ptr<Node>> &nodes);
    void SendBigBlock(std::vector<std::unique_ptr<Node>> &nodes);
};

#endif //K_CA_COMMITTEE_H
