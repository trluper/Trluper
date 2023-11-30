# 目录结构
- **目录结构介绍：**
	- `include`：框架声明的头文件，后续所有的抽象类和继承类头文件
	- `src`:放置框架的源文件
	- `build`:放置框架构建好的文件
	- `test`:放置测试代码
	- `TEST`:放置测试的`debug`文件
	- `client`存放客户端代码
```
.
├── CMakeLists.txt
├── client
├── TEST
├── build
├── include
├── lib
├── src
└── test
```


# 框架介绍
该服务器框架是基于epoll和socket技术的基础上实现的一个并发tcp_server框架。该框架采用三层结构的设计思想，实现服务器对客户端请求处理过程的解耦合。三层结构为连接层`Connections`、数据处理层`DataProcess`和业务应用层`FApplycations`。

此外，还集成了**路由功能**；封装**线程、锁和信号量**等调用模块和**线程池管理**；配备了**日志系统`log`**；配备了**定时器`Timer`**模块；支持**`TCP/HTTP`协议**，以提供快速部署高性能的TcpServer和HTTPServer的能力

开发可以基于此框架开发自己的WebServer和其他相关服务器，也可以通过实现该框架加深对后端知识的理解。

# 三层结构
<img src="images/TrluperPro.png" width =1000>

## 三层架构功能概括及处理流程
- `Server`：负责对`Server`服务器的初始化工作，如创建`Socket`、`epoll`和是否启动线程池、执行监听等操作，监听触发：
	- 当`Server`异步监听到一个连接请求后会调用一个`CreateAllObjWhenAccept`纯虚函数（需要实现），会创建该连接自己的`Connections`对象、`DataProcess`对象、`FApplycations`对象并一一绑定。
	- 监听若是是数据读写操作后，交由`Connections`层处理
- `Connections`连接层：**负责对已经建立连接的`Socket`文件描述符执行接收数据、发送数据功能**。
	- 若为接收数据，接收数据后会向绑定的`DataProcess`传输；
	- 若为发送，则向客户端发生数据。
- `DataProcess`数据处理层：**主要负责对数据格式的解析**，在这一层设用在可以设计自己的数据协议解析操作(如设计`TLV`形式的数据协议解决TCP粘包、如使用protobuf序列化技术等均可支持)；同样当业务应用多时，可通过实现路由`Router`功能进行寻找，防止因责任链过于冗长而耗时。
	- 对于来自`Conections`的数据，执行相应的数据处理操作后，交给指定的业务层对象`FApplycations`
	- 对于来自`FApplycations`的数据，执行相应的反数据处理后，交给该连接的`Connections`处理
- `FApplycations`应用层：**由应用开发者实现一些业务应用**。也可配备路由模块`Router`执行业务路由寻址。

## 三层需实现的纯虚函数
开发者要使用该框架，**必须**继承这三层并至少实现以下纯虚函数：
- `Connections`连接层：只需要实现一个纯虚函数：**`GetDataProcessObj(Message& msg)`**。
	- `virtual DataProcess* GetDataProcessObj(Message& msg)`:返回一个`DataProcess*`对象指针。
- `Dataprocess`数据处理层：需要实现`GetConnectionsObj`、`GetApplycationsObj`、`MsgToRequest`、`RequestToMsg`四个纯虚函数:
	- `GetConnectionsObj`：返回一个`Connections*`，用于指示数据由谁发送
	- `GetApplycationsObj`：返回一个`FApplycations*`对象，用于指示数据交给哪个业务对象。
	- `Request* MsgToRequest(Message& msg)`：将连接层递交上来的`message`处理为`Request`类型，即数据处理为`Request`请求后交给业务层
	- ` Message* RequestToMsg(Request& request)`：将应用层递交下来的`Request`类型的数据转为`Message`类型
- `FApplycations`应用层：需要实现` ProcRequest`和`GetNextApplycationsObj`两个纯虚函数：
	- `Request* ProcRequest(Request& _request)`：当前业务对象处理请求的函数，內部实现业务处理逻辑；若还需要进一步处理，则需要返回相关的请求`Request*`信息（堆对象），否则返回`nullptr`
	- `FApplycations* GetNextApplycationsObj(AbstractMsg& msg)`：下一个业务处理者处理

**tips：** 该框架定义了`AbstractMsg`抽象信息类，`Message`继承实现于它，`Message`为数据层与数据处理层的数据传输类，无需应用开发者重写实现；同时定时实现了`Request`，为数据处理层和应用层之间的数据传输类，需要开发者重新实现自己的`request`。

## 快速开始
此节示例一个简单C/S回显功能：
- `Connections`数据层类的实现：
```
inline MyDataProcess *MyConnections::GetDataProcessObj(Trluper::Message &msg)
{
    return process;
}
```

- `DataProcess`数据处理层实现：
```
inline Trluper::Connections *MyDataProcess::GetConnectionsObj(Trluper::AbstractMsg &msg)
{
    return conn;
}

inline Trluper::FApplycations *MyDataProcess::GetApplycationsObj(Trluper::AbstractMsg &msg)
{
    return app;
}


Trluper::Request *MyDataProcess::MsgToRequest(Trluper::Message& msg)
{
    ListRequest* ret = nullptr;
    std::string& str = msg.message;
    while (1)
    {
        //8字节无法满足
        if(str.size()<8)  break;
        int id = 0;
        id |= str[0]<<0;
        id |= str[1]<<8;
        id |= str[2]<<16;
        id |= str[3]<<24;
        int bytelength = 0;
        bytelength |= str[4]<<0;
        bytelength |= str[5]<<8;
        bytelength |= str[6]<<16;
        bytelength |= str[7]<<24;
        //请求不完整
        if(str.size()-8<bytelength){
             break;
        }
        MyRequest* _request =new MyRequest((REQUEST_TYPE)id,str.substr(8,bytelength));
        lreq->lRequest.push_back(_request);
        str.erase(0,8+bytelength);
    }
    if(lreq->lRequest.empty()==false){
        ret = lreq;
    }
   
    return ret;
}

Trluper::Message *MyDataProcess::RequestToMsg(Trluper::Request &request)
{
    Trluper::Message* _CMsg =new Trluper::Message(Trluper::IO_Direction::OUT);
    DYNAMIC_GETREF(MyRequest,_outRequest,request);
    std::string str=_outRequest->ret;
    int id = _outRequest->requestType;
    int bytelength = str.size();
    _CMsg->message.push_back((id>>0) & 0xff);
    _CMsg->message.push_back((id>>8) & 0xff);
    _CMsg->message.push_back((id>>16) & 0xff);
    _CMsg->message.push_back((id>>24) & 0xff);
    _CMsg->message.push_back((bytelength>>0) & 0xff);
    _CMsg->message.push_back((bytelength>>8) & 0xff);
    _CMsg->message.push_back((bytelength>>16) & 0xff);
    _CMsg->message.push_back((bytelength>>24) & 0xff);
    _CMsg->message.append(str);
    delete _outRequest;
    _outRequest = nullptr;
    return _CMsg;
}
```

- `FApplycations`应用层实现：
```
inline Trluper::Request *MyFApplycations::ProcRequest(Trluper::Request& _request)
{
    DYNAMIC_GETREF(ListRequest,_lRequestObj,_request);
   while(false == _lRequestObj->lRequest.empty()){
        auto request = _lRequestObj->lRequest.front();
        _lRequestObj->lRequest.pop_front();
        std::cout<<"Type is: "<<request->requestType<<std::endl;
        switch (request->requestType)
        {
        case REQUEST_TYPE::REQUEST_TYPE_ECHO:
            echoFunc(request->ret);
            break;
        case REQUEST_TYPE::REQUEST_TYPE_ECHO_LEN:
            echoAndLenFunc(request->ret);
            break;
        }
        //释放数据层的分配的堆
        delete request;
    }
    return nullptr;
}

inline Trluper::FApplycations *MyFApplycations::GetNextApplycationsObj(Trluper::AbstractMsg& msg)
{
    return nullptr;
}
```

**除此以外，我们还需要实现带有`CreateAllObjWhenAccept`的工厂类，当一个连接请求带来时，应该建立好这三层对象和责任链关心，此外还需定义自己的`Request`:**

- `factory`类：
```
Trluper::Connections *Factory::CreateAllObjWhenAccept(int _dataFd)
{
    MyConnections* conn = new MyConnections(_dataFd);//8/12
    MyDataProcess* process = new MyDataProcess();//4/12
    MyFApplycations* app = new MyFApplycations();//2/12
    conn->SetDataProcessObj(process);
    process->SetConnectionsObj(conn);
    process->SetFApplycationsObj(app);
    app->SetMyDataProcessObj(process);
    return conn;
}
```

- `MyRequest`类：
```
class MyRequest:public Trluper::Request{
public:
    
    MyRequest(REQUEST_TYPE _type,std::string str);
    virtual ~MyRequest();
    

public:
    //用户的protobuf消息指针
    std::string ret;
    //定义了两种类型请求，一个是回显，一个是回显+消息长度
    REQUEST_TYPE requestType;
};
```

**上述的具体实现可看`TEST`目录下的源文件**

# 日志系统的使用

