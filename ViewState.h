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
        WAIT_BLOCK,
        PBFT_CONSENSUS//共识委员会投票
	}_state;

      size_t accepted_confirm = 0;


public:
    ViewState();
    ViewState(const ViewState& vt);
    ViewState & operator = (const ViewState& vt);
    explicit ViewState(const Message& msg);
    void handle_message(Message msg, Node & node);
};
#endif
