#include "Node.h"
#include "Committee .h"
#include <random>

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



    std::vector<Committee> vec_committee;
    R arr2;
    network_address_t arr1[Num_Node/NUMOFMEMBERS][NUMOFMEMBERS];
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
    for (int i = 0; i < Num_Node/NUMOFMEMBERS; ++i) {

        for (int j = 0; j < NUMOFMEMBERS; ++j) {
            if(j == 0)
            {
                vec_committee.emplace_back(Committee(NUMOFMEMBERS,i+1,arr2[i][0]));
                cout<<"第 " << i+1 <<" 个委员会的主节点：" << arr2[i][j] << endl;
            }
            vec_committee[i].GetCommitteeMembers().emplace_back(arr2[i][j]);
        }
    }

for (auto it:vec_committee) {
	it.ShuffleNode(nodes);
}



    for(int i = 0; i < NUMOFTRANS; i++)
    {

        std::string str;
        str = "Test"+ to_string(i);
        Message request(Message::REQUEST);
        request.t = std::time(nullptr);
        request.o = str;
        request.c = request.i = client.GetNodeAddress();
        request.d = request.diggest();
        request.m = request.str();

        for(int j = 0; j < Num_Node; j++)
        {
            request.r = nodes[j]->iDentity;

            client.SendRequest(nodes[j]->GetNodeAddress(),request);
        }

        if (i%400 == 0 && i/400 !=0)
        {
            while (!Network::instance().Empty())
            {
                continue;
            }
            for(int j = 0; j < Num_Node; j++)
            {
                cout<<"节点 " << nodes[j]->GetNodeAdd() << "节点身份码："<< nodes[j]->iDentity <<endl;
                cout<<"节点 " << nodes[j]->GetNodeAdd() << "属于第 "<<(long )nodes[j]->iDentity.c_str() % 5 <<" 委员会。"<<endl;
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
