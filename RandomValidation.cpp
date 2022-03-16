#include "Node.h"
#include "Committee.h"
#include <random>
#include <string>

typedef network_address_t (*R)[NUMOFMEMBERS];
using std::cout;
using std::endl;


std::mutex console_mutex;


R ShuffleNode(int arr1[][NUMOFMEMBERS]) {

    int num_committee = Num_Node / NUMOFMEMBERS;
    int arr[Num_Node];

    for (int i = 2; i < Num_Node + 2; ++i) {
        arr[i - 2] = i;
    }
    std::shuffle(arr, arr + Num_Node, std::mt19937(std::random_device()()));

    for (int i = 0; i < num_committee; ++i) {
        for (int j = 0; j < NUMOFMEMBERS; ++j) {

            arr1[i][j] = arr[i * num_committee + j];
        }
    }
    return arr1;

}

int main()
{
    using namespace std::chrono;
    auto start = system_clock::now();
    Client client;

    int result;



    std::vector<Committee> vec_committee;
    R arr2;
    int committee_numbers = Num_Node/NUMOFMEMBERS;
    network_address_t arr1[committee_numbers][NUMOFMEMBERS];
    arr2 = ShuffleNode(arr1);


    std::vector<std::unique_ptr<Node>> nodes;
    for (int i = 0; i < Num_Node; i++)
    {
        nodes.push_back(std::make_unique<Node>());
    }

    for (auto & node : nodes)
    {
        node->SetAllNodes(nodes);
    }



    //创建委员会并记录委员会内节点
    for (int i = 0; i < committee_numbers; ++i) {
        for (int j = 0; j < NUMOFMEMBERS; ++j) {
            if(j == 0)
            {
                vec_committee.emplace_back(Committee(NUMOFMEMBERS,i+1,arr2[i][0]));
                cout<<"第 " << i+1 <<" 个委员会的主节点：" << arr2[i][j] << endl;
                if (i == 0)
                {
                    ConsensusCommittee::instance() = ConsensusCommittee(committee_numbers,0,arr2[i][j]);
                    cout << "创立共识委员会, 该委员会编号： " <<  ConsensusCommittee::instance().sequence <<" 该委员会的领导人: " << arr2[i][j] << endl;

                }
                ConsensusCommittee::instance().GetCommitteeMembers().emplace_back(arr2[i][j]);
            }
            vec_committee[i].GetCommitteeMembers().emplace_back(arr2[i][j]);
        }
    }

    //各节点记录委员会的其他节点
    for (auto it:vec_committee) {
        for (auto &node:nodes) {
            it.ShuffleNode(node);
        }

    }

    //共识委员会节点统计
    for (auto it:ConsensusCommittee::instance().GetCommitteeMembers())
    {
        for (auto & node:nodes) {
            if (it == node->GetNodeAdd())
            {
                node->isLeader = true;
//                cout << "节点 " << it << " 是主节点。"<<endl;
            }
        }
    }

    //输出委员会的其他成员
//    for (auto & node:nodes)
//    {
//        cout << "节点 ：" << node->GetNodeAdd() << " 属于第 " << node->committe_seq << " 委员会。" << endl;
//        for (auto committee_members:node->_otherCommitteeNodes) {
//            cout<< "其他节点： " << committee_members << " ";
//
//        }
//        cout << endl;
//    }



    for(int i = 0; i < NUMOFTRANS; i++) {

        std::string str;
        std::string substr;
        str = "Test" + to_string(i);
        Message request(Message::REQUEST);
        request.t = std::time(nullptr);
        request.o = str;
        request.c = request.i = client.GetNodeAddress();
        request.d = request.diggest();
        request.m = request.str();
        request.n = i;
        substr = request.d.substr(request.d.length() - 2, request.d.length());
        result = (std::atoi(substr.c_str()) % committee_numbers) + 1;


        //交易分片

        for (auto & iter:vec_committee) {
            if (iter.sequence == result)
            {
                iter.GetTranslations().emplace_back(request);
//                cout << "交易 " << str << " 被分配到第 " << iter.sequence
//                     << " 委员会中。" << endl;
//                if (!iter.GetTranslations().empty())
//                    cout << "第" << iter.sequence << "委员会拥有 " << iter.GetTranslations().size() << " 交易。 "  << endl;
            }
        }

        for (auto &node: nodes) {
            if (result == node->committe_seq) {
                client.SendRequest(node->GetNodeAdd(), request);
//                cout << "第 " << i<< " 交易,其尾号 " << result << " 发送至节点 " << node->GetNodeAdd() << endl;
            }
        }
//        for(int j = 0; j < Num_Node; j++)
//        {
//            request.r = nodes[j]->iDentity;
//   //       client.SendRequest(nodes[j]->GetNodeAddress(),request);
//        }

//        if (i%400 == 0 && i/400 !=0)
//        {
//            while (!Network::instance().Empty())
//            {
//                continue;
//            }
//            for(int j = 0; j < Num_Node; j++)
//            {
//                cout<<"节点 " << nodes[j]->GetNodeAdd() << "节点身份码："<< nodes[j]->iDentity <<endl;
//                cout<<"节点 " << nodes[j]->GetNodeAdd() << "属于第 "<<(long )nodes[j]->iDentity.c_str() % 5 <<" 委员会。"<<endl;
//            }
//        }


    }
//将每个委员会交易数量发送给节点
    for (auto & iter:vec_committee) {
        cout <<  "委员会： " << iter.sequence << "总共拥有 " << iter.GetTranslations().size()  << "交易。"<<endl;
        for (auto  & node: nodes) {
            if (iter.sequence == node->committe_seq)
            {
                node->committee_trans = iter.GetTranslations().size();
            }
        }
    }

    while (!Network::instance().Empty())
        std::this_thread::sleep_for(1s);


//    for(int j = 0; j < Num_Node; j++)
//    {
//        cout<<"节点 " << nodes[j]->GetNodeAdd() << "节点身份码："<< nodes[j]->iDentity <<endl;
//    }



    duration<double> diff = system_clock::now() - start;
    cout<<"elapsed: " << diff.count() << " seconds" <<endl;
    cout<<"TPS: " << NUMOFTRANS/(diff.count())<<endl;

	return 0;
}
