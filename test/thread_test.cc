#include "thread.h"
#include "threadPool.h"
using namespace std;

int main(){
    Trluper::ThreadPool::ptr manager(new Trluper::ThreadPool(8,3000));
    

}