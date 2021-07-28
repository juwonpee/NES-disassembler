#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "types.h"
#include "disassembler.h"


using namespace std;

void help() {

}

int main(int argc, char* argv[]) {

    string directory;
    int bankNum;
    bool disassemble = false;
    bool hexDump = false;

    cout << "main thread" << endl;

    int opt;
    while ((opt = getopt(argc, argv, "r:b:dh")) != -1) {
        switch (opt) {
            case 'r':
                directory = optarg;
                continue;
            case 'b':
                bankNum = atoi(optarg);
                continue;
            case 'd':
                disassemble = true;
                continue;
            case 'h':
                hexDump = true;
                continue;
        }
    }
    if (directory == "") {
        cout << "Needs directory option" << endl;
        help();
        exit(EXIT_FAILURE);
        
    }
}