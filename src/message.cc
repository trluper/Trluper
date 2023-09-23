#include "message.h"

namespace Trluper{

AbstractMsg::AbstractMsg()
{

}

AbstractMsg::~AbstractMsg()
{

}

IOState::IOState(IO_Direction dic):IO_DIC(dic)
{

}

IOState::~IOState()
{
    
}

Message::Message(IO_Direction dic):IOState(dic)
{
}

UserMsg::UserMsg(IO_Direction dic):IOState(dic)
{
}

UserMsg::~UserMsg()
{
    
}

}
