//
// Created by 姚黎东 on 2022/3/3.
//

#ifndef K_CA_COMMITTEE_H
#define K_CA_COMMITTEE_H

#include "Node.h"
#include "Message.h"
#include <algorithm>
#include <random>

class Committee{
private:
    int number_members;
    int sequence;
    std::vector<network_address_t> _members;
    network_address_t whoismaster;
public:
    Committee();
    Committee(int num_members, int seq, network_address_t wism);
    std::vector<network_address_t>& GetCommitteeMembers();

    void ShuffleNode(std::vector<std::unique_ptr<Node>> &nodes);

    //void AssignCommittee(Node &node Message &msg);





};


class ConsensusCommittee:public Committee{
private:
    network_address_t whoisleader;

public:
    ConsensusCommittee();
    ConsensusCommittee(int num_members, int seq, network_address_t wism, network_address_t wisl);
    void SealerBlock();
    void SendBlock();
};

#endif //K_CA_COMMITTEE_H
