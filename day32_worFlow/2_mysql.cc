#include <workflow/WFFacilities.h>
#include <workflow/MySQLResult.h>
#include <workflow/MySQLUtil.h>
#include <workflow/MySQLMessage.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::string;
using std::vector;

// 全局静态变量：作用域只在本文件中
static WFFacilities::WaitGroup gWaitGroup(1); // 设置计数器为1

void handler(int signum) // 信号到达时要处理的函数
{
    printf("signum = %d\n", signum);
    gWaitGroup.done(); // 计数器减1
}

void mysqlCallback(WFMySQLTask *mysqlTask)
{
    // 检查连接错误
    if (mysqlTask->get_state() != WFT_STATE_SUCCESS)
    {
        cerr << WFGlobal::get_error_string(mysqlTask->get_state(), mysqlTask->get_error());
        return;
    }
    // 检查mysql语法错误
    protocol::MySQLResponse *resp = mysqlTask->get_resp();
    if (resp->get_packet_type() == MYSQL_PACKET_ERROR)
    {
        cerr << "mysql error : " << resp->get_error_msg() << "\n";
        return;
    }
    // 使用cursor获取执行结果
    protocol::MySQLResultCursor cursor(resp);
    // 可以查看多条mysql语句的结果
    do
    {
        // 写入操作的结果
        if (cursor.get_cursor_status() == MYSQL_STATUS_OK)
        {
            cerr << "affected rows : " << cursor.get_affected_rows() << "\n";
        }
        // 读取操作的结果
        else
        {
            vector<vector<protocol::MySQLCell>> rows;
            cursor.fetch_all(rows);
            for (auto &row : rows)
            {
                for (auto &cell : row)
                {
                    if (cell.is_int())
                    {
                        cout << cell.as_int() << " ";
                    }
                    if (cell.is_string())
                    {
                        cout << cell.as_string() << " ";
                    }
                    if (cell.is_ulonglong())
                    {
                        cout << cell.as_ulonglong() << " ";
                    }
                }
                cout << "\n";
            }
        }

    } while (cursor.next_result_set());
}

int main()
{
    signal(SIGINT, handler); // 注册信号和信号函数
    WFMySQLTask *mysqlTask = WFTaskFactory::create_mysql_task(
        "mysql://root:112358@localhost", 0, mysqlCallback);
    protocol::MySQLRequest *req = mysqlTask->get_req();
    string query = "insert into 54test.users values(8, 'xsssa', '234');";
    query += "select * from 54test.users;";
    req->set_query(query);
    mysqlTask->start();
    gWaitGroup.wait(); // 等待（阻塞）直到计数器为0
    return 0;
}
