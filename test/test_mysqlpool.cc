#include "mysqlpool.h"
#include "config.h"

int main(){
    Json::Value _config = Trluper::getConfig("../config/config.json");
    Trluper::MySQLPool::ptr mysqlpool(new Trluper::MySQLPool(4,
        _config["mysql_host"].asString(), _config["mysql_user"].asString(),
        _config["mysql_password"].asString(), _config["mysql_database"].asString(),
        _config["mysql_port"].asInt()));
    MYSQL* conn = mysqlpool->getFreeConn();
    if(mysql_query(conn,"select * from CUSTOMER order by ID;") != 0){
        std::cout<<"failed"<<std::endl;
    }else{
        MYSQL_RES *result = mysql_store_result(conn);
        if (result != nullptr) {
        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result))) {
            for (int i = 0; i < num_fields; i++) {
                printf("%s ", row[i] ? row[i] : "NULL");
            }
            printf("\n");
        }
        mysql_free_result(result);
        }else {
            fprintf(stderr, "mysql_store_result failed: %s\n", mysql_error(conn));
            return 1;
        }
    }
    return 0;
}