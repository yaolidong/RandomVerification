#ifndef K_CA_VIEWSTATE_H
#define K_CA_VIEWSTATE_H

#include "Message.h"
#include <map>


class Node;

class ViewState
{

	enum state_t
	{
        NODE_ADMISSION
        LEADER_Election
        SEND_TRANS,//K-ca共识阶段
        COMFIRM_TRANS,
        WAIT_BLOCK,
        SEND_BLOCK//K-ca共识阶段
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
