#include "../../include/message/message.hpp"
#include "../../include/message/answer_msg.hpp"
#include "../../include/message/put_msg.hpp"

string Message::typeStrings[] = {"PUT", "GET", "SUB", "UNSUB", "ANSWER"};

CEREAL_REGISTER_TYPE(GetMessage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, GetMessage);
CEREAL_REGISTER_TYPE(SubMessage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, SubMessage);
CEREAL_REGISTER_TYPE(UnsubMessage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, UnsubMessage);
CEREAL_REGISTER_TYPE(AnswerMessage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, AnswerMessage);
CEREAL_REGISTER_TYPE(PutMessage)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Message, PutMessage);
