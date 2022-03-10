//
// Created by 姚黎东 on 2022/3/3.
//
#include <sstream>
#include "Committee.h"

Committee::Committee():number_members(0),number_trans(0),sequence(0),whoismaster(0) {}

Committee::Committee(int num_members, int seq, network_address_t wism) {
    number_members = num_members;
    sequence = seq;
    whoismaster = wism;
}

void Committee::ShuffleNode(std::vector<std::unique_ptr<Node>> &nodes) {

    for (auto &node:nodes) {
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
//    int num_committee = Num_Node/NUMOFMEMBERS;
//
//    //节点分配委员会，并保存所在委员会中其他节点
//    for (int l = 0; l < Num_Node; ++l) {
//        for (int i = 0; i < num_committee; ++i) {
//            for (int j = 0; j < NUMOFMEMBERS; ++j) {
//                if (nodes[l]->GetNodeAdd() == arr1[i][j])
//                {
//                    for (int k = 0; k < NUMOFMEMBERS; ++k) {
//                        if (arr1[i][k] == nodes[l]->GetNodeAdd())
//                            continue;
//                        else
//                            nodes[l]->_otherCommitteeNodes.push_back(arr1[i][k]);
//                    }
//                }
//            }
//
//        }
//    }

}

std::vector<network_address_t> & Committee::GetCommitteeMembers() {
    return _members;
}

int Committee::GetCommitteeSeq() {
    return sequence;
}

std::vector<network_address_t> Committee::GetMembers() {
    return _members;
}

network_address_t Committee::GetLeaderAddress() {
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


void ConsensusCommittee::SealerBlock(std::vector<std::unique_ptr<Node>> &nodes) {
    BigBlock bNew;
    std::stringstream ss;
    for (auto iter:vec_blocks) {
        ss << iter._bHash;
    }
    bNew._bHash = sha256(ss.str());
    bNew = sl.Upchain(bChain);//区块上链
    return bNew;

}
void ConsensusCommittee::SendBigBlock(std::vector<std::unique_ptr<Node>> &nodes) {
    for (auto &  node:nodes) {
        //TODO:发送大区块
      Network::instance().SendBigBlock(GetLeaderAddress(),node->GetNodeAdd(),bigBlock);
      cout << "Master节点发送大区块给节点 " << node->GetNodeAdd() << endl;
    }

}


