//
// Created by 姚黎东 on 2022/3/3.
//
#include <sstream>
#include "Committee.h"
#include "Node.h"

Committee::Committee():number_members(0),sequence(0),whoismaster(0) {}

Committee::Committee(int num_members, int seq, network_address_t wism) {
    num_members = num_members;
    sequence = seq;
    whoismaster = wism;
}

void Committee::ShuffleNode(std::unique_ptr<Node> & node) {

        for (auto it:GetCommitteeMembers()) {
            if (node->GetNodeAdd() == it)
            {
                for (auto  iter:GetCommitteeMembers()) {
                    if (node->GetNodeAdd() == iter)
                    {
                        node->committe_seq = sequence;

                    }
                    else
                    {
                        node->committe_seq = sequence;
                        node->_otherCommitteeNodes.emplace_back(iter);
                    }
                }
            }
        }

}

std::vector<network_address_t> & Committee::GetCommitteeMembers() {
    return _members;
}

vector<Message> & Committee::GetTranslations()  {
    return _translations;
}

std::vector<network_address_t> Committee::GetMembers() {
    return _members;
}

network_address_t Committee::GetLeaderAddress() {
    return whoismaster;
}

network_address_t Committee::GetWhoisMaster() {
    return whoismaster;
}


//void Committee::AssignCommittee(Node &node, Message &msg) {
//
//}
ConsensusCommittee::ConsensusCommittee(){
    Committee(0,0,0);
}

ConsensusCommittee::ConsensusCommittee(int num_members, int seq, network_address_t wism) :
Committee(num_members, seq, wism) {}

ConsensusCommittee &ConsensusCommittee::instance() {
    static ConsensusCommittee  inst;
    return inst;
}

void ConsensusCommittee::PBFT(Message msg) {

}


BigBlock  ConsensusCommittee::SealerBlock() {
    BigBlock bigBlock;
    std::stringstream ss;
    for (auto iter:vec_blocks) {
        ss << iter._bHash;
        bigBlock.AddMicroBlock(iter);//将每个委员会的区块都装进大区块中
    }
    bigBlock._bHash = sha256(ss.str());
    return bigBlock;
}
void ConsensusCommittee::SendBigBlock(Node & node,BigBlock & bBlk) {
        //TODO:发送大区块

        if (node.GetNodeAdd() == GetWhoisMaster())
        {
            Network::instance().SendBigBlock(GetLeaderAddress(),node.GetNodeAdd(),bBlk);
            cout << "共识委员会：" << sequence << "Master节点" << GetWhoisMaster() << "发送大区块给节点 " << node.GetNodeAdd() << endl;
        }

}


