#ifndef K_CA_MESSAGE_H
#define K_CA_MESSAGE_H
#include "RandomValidation.h"
#include "sha256.h"
#include <string>

 struct Message
{
	enum msg_type_t
	{

		REQUEST,
		CONFIRM,
		UNPACK,
        PRE_PREPARE,
        PREPARE,
        COMMIT,
        PBFT
	}msg_type;

	std::string o;
	time_t t;
	network_address_t c = 0;//交易发送方IP地址
	std::string d;
	size_t v = 0;
	size_t n = 0;
	network_address_t i = 0;//消息发送方IP地址
    std::string m;
     std::string r;//epochRandomness


        explicit Message(msg_type_t _type);
        Message(network_address_t master,msg_type_t _type);
        Message(const Message & msg);
        Message & operator=(const Message & msg);
	std::string diggest();
	std::string str() const;
};
#endif
