//
// Created by 姚黎东 on 2021/4/16.
//

#include "ViewState.h"
#include "Node.h"

ViewState::ViewState() {
        //_state = SEND_TRANS;
        _state = NODE_ADMISSION;
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
	case  NODE_ADMISSION:{
    node.CalculateEpochRandomness(node.GetBlockChain());
						 }
	case LEADER_Election:{

						 }

  case SEND_TRANS: {
    // TODO：主节点验证客户端请求消息签名是否正确，如果正确，则
    msg.msg_type = Message::CONFIRM;
    msg.i = node.GetNodeAdd();
    msg.n = node.GetTransNum();
    node.SendAll(msg);
    if (node.GetNodeAdd() == 2)
      _state = COMFIRM_TRANS;
    else {
      _state = WAIT_BLOCK;
    }
  }
  break;
  case WAIT_BLOCK:
  {
    if (msg.msg_type == Message::UNPACK)
    {
      //std::cout << "节点：" << node.GetNodeAdd() <<" 添加区块。"<<std::endl;
      node.GetOutBk();
    }

  }
  break;
  case COMFIRM_TRANS: {
    // TODO：副本节点验证
    //  a. 主节点PRE-PREPARE消息签名是否正确。
    //  b. 当前副本节点是否已经收到了一条在同一v下并且编号也是n，但是签名不同的PRE-PREPARE信息。
    //  c. d与m的摘要是否一致。
    //  d. n是否在区间[h, H]内。
    //  如果正确，则

    if (msg.msg_type == Message::CONFIRM) {
      accepted_confirm++;
    }
    if (accepted_confirm == k_value) // TODO:K_CA
    {
        //std::cout << "节点：" << node.GetNodeAdd() << " 达到k确认！"
                  //<< std::endl;
        node.TransToCache(msg);
        Block bNew = node.SealTrans();
        if ((msg.n+1)%400 == 0)
        {
//          if ((msg.n+1)/400 >=1 &&  (msg.n+1)/400 <=9)
//          {
//            std::cout<<"交易被质疑，进入审查。"<<std::endl;
//            //std::cout<<"审查结束。"<<std::endl;
//            this_thread::sleep_for(1.2s);
//            std::cout<<"审查结束。区块"<< (msg.n+1)/400<<"错误"<<std::endl;
//          }
          node.SendBlock(bNew);
          node.SendUnpack(msg);
          _state = SEND_BLOCK;
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








