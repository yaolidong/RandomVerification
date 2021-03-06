#include <sstream>

#include "Message.h"


Message::Message(msg_type_t _type):msg_type(_type) {

}


Message::Message(network_address_t  master,msg_type_t _type):msg_type(_type) {
    c = master;
    t = time(nullptr);
    o = "PBFT_Consensus";
    n = 0;
    i = 0;
    v = 0;
    r = "";
    d = this->diggest();
    m = this->str();
}

std::string Message::diggest() {
    std::stringstream ss;
    ss << o << t << c << r;
    return sha256(ss.str());
}

std::string Message::str() const{
    std::stringstream ss;
    ss
        <<"o="<<o
        <<", t="<<t
        <<", v="<<v
        <<", n="<<n
        <<", c="<<c
        <<", d="<<d
        <<", r="<<r
        <<")";
    return ss.str();
}
Message::Message(const Message &msg) {
  msg_type = msg.msg_type;
  t = msg.t;
  c = msg.c;
  o = msg.o;
  d = msg.d;
  i = msg.i;
  v = msg.v;
  n = msg.n;
  r = msg.r;
}
Message &Message::operator=(const Message &msg) {
  if (this == & msg)
    return *this;
  msg_type = msg.msg_type;
  t = msg.t;
  c = msg.c;
  o = msg.o;
  d = msg.d;
  i = msg.i;
  v = msg.v;
  n = msg.n;
  r = msg.r;
  return  *this;

}
