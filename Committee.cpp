//
// Created by 姚黎东 on 2022/3/3.
//
#include "Committee.h"

Committee::Committee():number_members(0),sequence(0),whoismaster(0) {}

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
                        continue;
                    else
                    {
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

//void Committee::AssignCommittee(Node &node, Message &msg) {
//
//}
ConsensusCommittee::ConsensusCommittee(){
    Committee(0,0,0);
    whoisleader = 0;
}

ConsensusCommittee::ConsensusCommittee(int num_members, int seq, network_address_t wism, network_address_t wisl) :
Committee(num_members, seq, wism),whoisleader(wisl) {}
