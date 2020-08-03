#include <iostream>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

const int socket_count = 100;
string ip, port;
int sockNum;

bool isValidIp(string s){
    int cnt{};
    for (char& c : s) {
        if(c == '.'){
            c = ' ';
            cnt++;
        }
    }
    if (cnt != 3) return false;
    stringstream ss(s);
    string d;
    while (ss >> d){
        if(d.length() > 3){
            return false;
        }
        int v;
        try {
            v = stoi(d);
        } catch(invalid_argument e){
            e.what();
            return false;
        } catch (out_of_range e) {
            return false;
            e.what();
        }
        if (v < 0 || v > 255) return false;
    }
    return true;
}

bool isValidPort(string s){
    int v;
    try {
        v = stoi(s);
    }catch(invalid_argument e){
        return false;
    }catch(out_of_range e){
        return false;
    }
    if (v < 0 || v > 65535) return false;
    return true;
}

void handle_arguments(int argc, const char* argv[], string& v_ip, string& v_port){
    if(argc != 3){
        cerr << "Wrong number of arguments" << endl;
        exit(1);
    }
    v_ip = string(argv[1]), v_port = string(argv[2]);
    if(!isValidIp(v_ip) || !isValidPort(v_port)){
        cerr << "Wrong format of ip address or port" << endl;
        exit(1);
    }
}

void thread_code() {
    // cerr << "Thread id" << thread::get_id() << endl;
    while (true) {
        // if (sockNum > socket_count) break;
        struct sockaddr_in dest;
        dest.sin_family = AF_INET;
        dest.sin_port = htons(stoi(port));
        struct hostent* host = (struct hostent*)gethostbyname(ip.c_str());
        dest.sin_addr = *((struct in_addr*)host->h_addr);
        int nbytes;

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        cerr << "Socket created: " << sock << "Created socket number: " << sockNum << endl;
        sockNum++;
        if (sock == -1) {
            perror("CLIENT socket creation");
            exit(1);
        }

        int connect_n = connect(sock, (struct sockaddr*)&dest, sizeof(dest));
        if (connect_n != 0) {
            fprintf(stderr, "ERROR in connecting: %d\n", errno);
            perror("connecting");
            close(sock);
            break;
        }
        int value{1};
        nbytes = send(sock, &value, sizeof(value), 0);
        char buffer[1024];
        while (recv(sock, buffer, sizeof(buffer), 0));
        sleep(3);
        close(sock);
    }
}

void exit_thread(){
    while (true) {
        int n;
        cin >> n;
        if (n == 0)
            exit(1);
    }
}

int main(int argc, const char* argv[]) {
    handle_arguments(argc, argv, ip, port);
    // int n = thread::hardware_concurrency();
    int n = 20;
    thread* a;
    a = new thread[n + 1];
    for (int i = 0; i < n; i++) {
        a[i] = thread(thread_code);
    }
    a[n] = thread(exit_thread);
    for (size_t i{}; i < n + 1; i++) {
        a[i].join();
    }
    return 0;
}