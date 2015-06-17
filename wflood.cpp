// compile with  g++ wflood.cpp httpcpp.cpp -o wflood -std=c++11 
// by sha0coder using the great lib httpcpp form https://github.com/amcewen/HttpClient


#include "httpcpp.h"
#include <iostream>

using namespace std;

static int counter = 0;

class HttpResponseHndl : public HttpResponseHandler {
    public:
        void handle(HttpResponse* const response) {
            cout << counter-- << " [" << response->get_code() << "] ";
            cout << response->get_body().size() << " bytes" << endl;
            if (counter <= 0)
                exit(1);
        }
};





int main(int argc, char **argv) {
    if (argc != 7) {
        cout << argv[0] << " [ip] [port] [method] [path] [post] [num] " << endl;
        exit(1);
    }

    counter = atoi(argv[6]);

    AsyncHttpClient* client = new AsyncHttpClient();
    for (int i=0; i<counter; i++)
        client->fetch(argv[1], atoi(argv[2]), argv[3], argv[4], argv[5], new HttpResponseHndl);
    
    IOLoop::instance()->start();
    delete client;
}
