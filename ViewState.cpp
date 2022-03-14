//
// Created by 姚黎东 on 2021/4/16.
//

#include "ViewState.h"
#include "Node.h"

ViewState::ViewState() {
        _state = SEND_TRANS;
}

ViewState::ViewState(const ViewState &vt) {
    _state = vt._state;
    accepted_confirm= vt.accepted_confirm;
    accepted_pre_prepare = vt.accepted_pre_prepare;
    accepted_prepare= vt.accepted_prepare;
    accepted_commit = vt.accepted_commit;
}
ViewState & ViewState::operator=(const ViewState &vt) {
    if (this == &vt)
        return *this;
    _state = vt._state;
    accepted_pre_prepare = vt.accepted_pre_prepare;
    accepted_prepare= vt.accepted_prepare;
    accepted_commit = vt.accepted_commit;
    accepted_confirm = vt.accepted_confirm;
    return *this;
}
void ViewState::handle_message(Message msg, Node & node) {
  switch (_state) {
    //交易发送阶段：
      case SEND_TRANS: {
          //交易验证正确后，转发给委员会其他节点
        msg.msg_type = Message::CONFIRM;
        msg.i = node.GetNodeAdd();
        cout << "节点 " << node.GetNodeAdd() << " 接收到" << msg.o
        << " 并转发给委员会其他成员"<<endl;
        node.SendAll(msg);

        //如果该节点是主节点，则进入交易确认阶段，否则，进入等待区块阶段
          if (node.isLeader)
          {
              _state = COMFIRM_TRANS;
              cout<<"第 " << node.committe_seq
              <<" 委员会主节点: "<<node.GetNodeAdd() <<"即将进入交易确认阶段。"<<endl;
          }
          else
          {
                _state = WAIT_BLOCK;
              cout<<"第 " << node.committe_seq
                  <<" 委员会节点: "<<node.GetNodeAdd() <<"即将进入交易等待阶段。"<<endl;
          }
      }
      break;
  //区块等待阶段：主节点将区块分发给其他节点，网络区块同步

  //交易确认阶段（主节点）：主节点将达到k确认的交易放入交易池中，若交易池中的交易数量达到400个，则打包交易成区块并发送给其他共识节点
  //不然，则进入等待交易模式
      case COMFIRM_TRANS: {
        if (msg.msg_type == Message::CONFIRM) {
//            cout << "主节点 " << node.GetNodeAdd() << "接收到确认信息" << endl;
          accepted_confirm++;
        }
        if (accepted_confirm == k_value)
        {
            cout << "主节点 " << node.GetNodeAdd() << "接收到的确认信息：" <<accepted_confirm << endl;
            cout << msg.o << " 添加进交易池。"<< endl;
            node.TransToCache(msg);
            node.committee_trans--;
            //打包微区块；
            //发送微区块至共识委员会其他成员，进入Pre_Prepare阶段；
            if (!node.committee_trans)
            {
                Block bNew = node.SealTrans();
                for (auto & iter:ConsensusCommittee::instance().GetCommitteeMembers()) {
                    if (iter == node.GetNodeAdd())
                        continue;
                    else
                    {
                        cout << "节点：" << node.GetNodeAdd() << "发送区块给共识委员会其他成员：" << iter <<endl;
                        node.SendBlock(bNew);
                    }
                }
                ConsensusCommittee::instance().vec_blocks.emplace_back(bNew);
                cout << "节点 " << node.GetNodeAdd() << "添加区块至共识委员会。" <<endl;
                cout << ConsensusCommittee::instance().vec_blocks.size() << endl;
                msg.msg_type = Message::PRE_PREPARE;
                cout << "节点： " << node.GetNodeAdd();
                msg.i = node.GetNodeAdd();
                node.SendConsensus(msg);
                cout << " 发送pre_prepare消息。" <<endl;

            }

        }
          _state = PBFT_Pre_prepare;
      }
      break;

      case PBFT_Pre_prepare:
      {
          if (msg.msg_type == Message::PRE_PREPARE)
          {
              accepted_pre_prepare++;
              cout<< "节点 " << node.GetNodeAdd() << accepted_pre_prepare << endl;
              if (accepted_pre_prepare == Num_Node/NUMOFMEMBERS)
              {
                  cout << "主节点 " << node.GetNodeAdd() << "进入PBFT预准备共识阶段" << endl;
                  BigBlock bNew = node.SealBlocks();//交易打包进区块中
                  node.SendBigBlock(bNew);
                  msg.i = node.GetNodeAdd();
                  msg.msg_type = Message::PREPARE;
                  node.SendConsensus(msg);
                  cout << "发送prepare消息。" <<endl;
              }
          }
          _state = PBFT_Prepare;
      }
          break;
      //主节点确认该区块交易正确，将区块并将结果发送给其他主节点
      case PBFT_Prepare:
      {
          if (msg.msg_type == Message::PREPARE)
          {
              msg.msg_type = Message::PREPARE;
              cout << "主节点 " << node.GetNodeAdd() << "进入PBFT准备共识阶段" << endl;
          }
//        验证交易正确后，把验证结果发送给共识委员会其他成员
          _state = PBFT_Commit;
      }
          break;
      //节点接收到2f+1个确认信息，将区块
      case PBFT_Commit:
      {
          if (msg.msg_type == Message::PREPARE)
              accepted_prepare++;
          if (accepted_prepare == ceil((2/3)* Num_Node/NUMOFMEMBERS))
          {
              cout<< "主节点" << node.GetNodeAdd() <<"收到足够commit信息，即将进入commit阶段。"<<endl;
          }
        msg.msg_type = Message::COMMIT;
        //交易上链，上链成功向共识委员会其他成员发送提交信息。
        _state = PBFT_Reply;
      }
          break;
      case PBFT_Reply:
      {
          if (msg.msg_type == Message::COMMIT)
              accepted_commit++;
          if (accepted_commit ==ceil((2/3)* Num_Node/NUMOFMEMBERS))
          {
              node.SendUnpack(msg);
//              node.iDentity = node.CalculateEpochRandomness(bNew);
              cout<< "Leader节点" << node.GetNodeAdd() <<"收到多个commit信息，共识完成，即将进入下一个epoch。"<<endl;
          }

            _state = WAIT_BLOCK;
      }

    break;

      case WAIT_BLOCK:
      {
          if (msg.msg_type == Message::UNPACK)
          {
              node.GetOutBk();
              cout<<"所有节点区块同步完成，进入下一个epoch."<<endl;
          }

      }
          break;
  }
}








