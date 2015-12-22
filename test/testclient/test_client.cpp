#include "public/base/san_msg.h"
#include "public/base/config.h"
#include "thirdparty/json2pb/json2pb.h"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        cout << "Usage:\n\t" << basename(argv[0]) << " ip port json_file" << endl;
        return -1;
    }
    FLAGS_logtostderr = true;
    SanMessage req;
    SanMessage rsp;
    char json[4096];
    ifstream f;
    f.open(argv[3]);
    f.read(json, sizeof(json));
    f.close();
    json2pb(req, json, strlen(json));
    cout << "send:\n" << req.DebugString() << endl;
    int ret = SanMsgUtil::SendAndRecv(argv[1], atoi(argv[2]), req, rsp);
    if (ret == 0)
    {
        cout << "recv:\n" << rsp.DebugString() << endl;        
    }
    return 0;
}

