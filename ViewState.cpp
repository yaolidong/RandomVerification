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
}
ViewState & ViewState::operator=(const ViewState &vt) {
    if (this == &vt)
        return *this;
    _state = vt._state;
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
  case WAIT_BLOCK:
  {
    if (msg.msg_type == Message::UNPACK)
    {
      node.GetOutBk();
      cout<<"节点区块同步完成，进入下一个epoch."<<endl;
     // _state = NODE_ADMISSION;
    }

  }
  break;
  //交易确认阶段（主节点）：主节点将达到k确认的交易放入交易池中，若交易池中的交易数量达到400个，则打包交易成区块并发送给其他共识节点
  //不然，则进入等待交易模式
  case COMFIRM_TRANS: {
    if (msg.msg_type == Message::CONFIRM) {
        cout << "主节点 " << node.GetNodeAdd() << "接收到确认信息" << endl;
      accepted_confirm++;
    }
    if (accepted_confirm == k_value)
    {
        cout << "主节点 " << node.GetNodeAdd() << "接收到的确认信息：" <<accepted_confirm << endl;
        cout << msg.o << " 添加进交易池。"<< endl;
        node.TransToCache(msg);
        _state = PBFT_CONSENSUS;
    }
  }
  break;
  case PBFT_CONSENSUS:
  {
      cout << "主节点 " << node.GetNodeAdd() << "进入PBFT共识阶段" << endl;
          //TODO:交易整合打包
          BigBlock bNew = node.SealBlocks();//交易打包进区块中

          node.SendBigBlock(bNew);
          node.SendUnpack(msg);
//          node.iDentity = node.CalculateEpochRandomness(bNew);
          cout<< "主节点发送区块完成，即将进入下一个epoch。"<<endl;
          _state = WAIT_BLOCK;
  }

    break;
  }
}








