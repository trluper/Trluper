#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_


namespace Trluper
{
template<typename T>
struct Node
{
    T data;
    Node* next;
    Node(T value):data(value),next(nullptr) {}
};

template<typename T>
struct LinkedList
{
public:
    LinkedList():m_pHead(nullptr),m_pTail(nullptr),m_len(0) {}
    ~LinkedList() {
        while(m_pHead != nullptr) {
            Node<T>* temp = m_pHead;
            m_pHead = m_pHead->next;
            //!segmentation fault
            delete temp;
            --m_len;
        }
    }
    
    /// @brief 尾部插入新节点，函数内部创建节点
    /// @param _value 节点的值
    void append(T _value) {
        Node<T>* newNode = new Node<T>(_value);
        if(m_pHead == nullptr) {
            m_pHead = newNode;
            m_pTail = m_pHead;
        }
        else {
            m_pTail->next = newNode;
            m_pTail = newNode;
        }
        ++m_len;
    }
    /// @brief 尾部插入新节点
    /// @param _node 新节点
    void addNode(Node<T>* _node) {
        if(m_pHead == nullptr)  {
            m_pHead = _node;
            m_pTail = m_pHead;
        }
        else {
            m_pTail->next = _node;
        }
        ++m_len;
    }
    Node<T>* getHead() { return m_pHead; }
    void setNullptr() { 
        m_pHead = nullptr, m_pTail = nullptr;
        m_len = 0; 
    }
    bool empty() { return m_len==0; }
    size_t size() { return m_len; }

private:
    Node<T>* m_pHead;
    Node<T>* m_pTail;
    size_t m_len;
};


} // namespace Trluper

#endif