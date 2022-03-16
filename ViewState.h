#ifndef K_CA_VIEWSTATE_H
#define K_CA_VIEWSTATE_H

#include "Message.h"
#include <map>


class Node;

class ViewState
{

	enum state_t
	{
        SEND_TRANS,//K-ca共识阶段
        COMFIRM_TRANS,
        PBFT_Pre_prepare,
        PBFT_Prepare,
        PBFT_Commit,
        PBFT_Reply,//共识委员会投票
        WAIT_BLOCK
	}_state;


    size_t accepted_confirm = 0;
    size_t accepted_pre_prepare = 0;
    double accepted_prepare = 0;
    double accepted_commit = 0;


public:
    ViewState();
//    ViewState(Message & msg);
    ViewState(const ViewState& vt);
    ViewState & operator = (const ViewState& vt);
    explicit ViewState(const Message& msg);
    void handle_message(Message msg, Node & node);

};
#endif
