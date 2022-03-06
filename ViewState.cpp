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
    msg.msg_type = Message::CONFIRM;
    msg.i = node.GetNodeAdd();

    //发送给委员会的其他成员
    node.SendAll(msg);

    //如果该节点是主节点，则进入交易确认阶段，否则，进入等待区块阶段
    if (node.GetNodeAdd() == 2)
    {
        if (msg.n%400 == 1)
        {
            cout<<"主节点 "<<node.GetNodeAdd() <<"即将进入交易确认阶段。"<<endl;
        }
        _state = COMFIRM_TRANS;
    }

    else {
        if (msg.n%400 == 1)
        {
            cout<<"节点 "<<node.GetNodeAdd() <<"即将进入等待区块阶段。"<<endl;
        }
      _state = WAIT_BLOCK;
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
      accepted_confirm++;
    }
    if (accepted_confirm == k_value)
    {
        node.TransToCache(msg);
        Block bNew = node.SealTrans();//交易打包进区块中
        if ((msg.n+1)%400 == 0)
        {
          node.SendBlock(bNew);
          node.SendUnpack(msg);
          node.iDentity = node.CalculateEpochRandomness(bNew);
         // _state = NODE_ADMISSION;
          cout<< "主节点发送区块完成，即将进入下一个epoch。"<<endl;


        }
        else
          _state = WAIT_BLOCK;
    }
  }
  break;
  case SEND_BLOCK:
    break;
  }
}








