#include "httpFactory.hpp"
#include "httpTextRole.hpp"
#include "httpImageRole.hpp"
#include "httpRequest.hpp"
#include "httpDataProcess.hpp"
#include "server.h"

int main(){
    std::string configPath = "/home/project/Trluper/config/config.json";
    HttpFactory* factorySingle = dynamic_cast<HttpFactory*>(HttpFactory::GetSingle());
    TextRole* textAPP=new TextRole();
    ImageRole* imageAPP=new ImageRole();
    factorySingle->setFApplycationObj(HTTP_REQUEST_TYPE::TEXT,textAPP);
    factorySingle->setFApplycationObj(HTTP_REQUEST_TYPE::IMAGE,imageAPP);
    Trluper::Server::ServerExceptionStop();
    Trluper::Server::ServerInit(configPath,factorySingle,true);
    Trluper::Server::ServerRun();
}