#include "disassembler.h"

disassembler::disassembler(string directory, int bankNum, bool disassemble, bool hexDump) {
    ifstream reader;
    int bankNumROM;
    reader.open(directory, ios::binary);
    if (reader.fail()) {
        cout << "File does not exist" << endl;
        exit(EXIT_FAILURE);
    }
    copy(
        istream_iterator<uint8_t>(reader),
        istream_iterator<uint8_t>(),
        back_inserter(buffer)
    );

    // check NES header and rom size
    if ((buffer)[0] == 'N' & (buffer)[1] == 'E' & (buffer)[2] == 'S' & (buffer)[3] == 0x1A) {
        bankNumROM = (buffer)[4];
        if (bankNumROM > bankNum) {
            cout << "File has less banks than specified" << endl;
            exit(EXIT_FAILURE);
        }
    }
    // Copy specified bank
    copy(&buffer[16], &buffer[16 + bankNum * 16384], back_inserter(ROM));

    disassemble(hexDump);
}
