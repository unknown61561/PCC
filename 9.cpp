#include <bits/stdc++.h>
using namespace std;

/*
Supported instructions:
ORG
JMP
MOV
ADD
AND
HLT
*/

void mov_hex_value(vector<int> &reg, int start, int len, int val)
{
    for (int i = start; i < start + len; i++)
    {
        reg[i] = val % 16;
        val = val / 16;
    }
}

void add_hex_value(vector<int> &reg, int start, int len, vector<int> &reg2, int start2, int len2)
{
    if (len != len2)
    {
        cout << "Error" << endl;
        return;
    }

    int carry = 0;

    for (int i = start, j = start2; i < start + len, j < start2 + len2; i++, j++)
    {
        int val = carry + reg[i] + reg2[j];
        reg[i] = val % 16;
        carry = val / 16;
    }
}

void and_hex_value(vector<int> &reg, int start, int len, int val)
{
    for (int i = start; i < start + len; i++)
    {
        reg[i] = (reg[i] & val) % 16;
        val = val / 16;
    }
}

void show_reg(vector<int> &reg)
{
    for (int i = reg.size() - 1; i >= 0; i--)
    {
        char ch = 'A' + (reg[i] - 10);
        if (reg[i] <= 9)
            cout << reg[i];
        else
            cout << ch;
    }
    cout << endl;
}

int main()
{
    unordered_map<string, int> symbolTable;
    unordered_map<string, string> opCode;

    opCode["JMP"] = "EA", opCode["MOV"] = "B0", opCode["ADD"] = "04";
    opCode["AND"] = "84", opCode["HLT"] = "F4";

    vector<vector<int>> reg(4, vector<int>(4, 0)); // registers

    int starting_address = 0;
    int lines = 0;

    ifstream rdfil;
    rdfil.open("input.asm");

    string line;

    // Pass 1
    while (rdfil >> line)
    {
        if (line == "ORG")
        {
            rdfil >> line;
            starting_address = stoi(line);
        }

        else if (line == "HLT")
        {
            lines++;
            continue;
        }

        else if (line == "JMP")
        {
            rdfil >> line;
            if (symbolTable.find(line) == symbolTable.end())
                symbolTable[line] = -1;
        }

        else if (line == "MOV" or line == "ADD" or line == "AND")
        {
            rdfil >> line;
            rdfil >> line;
        }

        else
        {
            line.pop_back(); // omitting colon
            symbolTable[line] = starting_address + lines;
        }

        lines++;
    }

    cout << "The Symbol Table after Pass 1: " << endl;
    cout << "Label"
         << "\t"
         << "Address" << endl;
    for (auto i = symbolTable.begin(); i != symbolTable.end(); i++)
        cout << i->first << "\t" << i->second << endl;

    cout << endl;
    rdfil.close();

    rdfil.open("input.asm");
    ofstream wtfil("output.txt");

    lines = 0;
    // Pass 2
    while (rdfil >> line)
    {
        wtfil << starting_address + lines << " ";
        if (line == "ORG")
        {
            wtfil << "ORG ";
            rdfil >> line;
            wtfil << line << endl;
        }

        else if (line == "MOV" or line == "ADD" or line == "AND")
        {
            string instruction = line;
            wtfil << opCode[line] << " ";
            rdfil >> line;
            wtfil << line << " ";
            line.pop_back(); // drop comma
            string reg_name = line;
            rdfil >> line;
            wtfil << line << endl;

            int reg_no = reg_name[0] - 'A';
            int len = (reg_name[1] == 'X' ? 4 : 2);
            int start = (reg_name[1] == 'H' ? 2 : 0);

            int literal;
            if (instruction != "ADD")
                literal = stoi(line);

            if (instruction == "MOV")
                mov_hex_value(reg[reg_no], start, len, literal);

            else if (instruction == "AND")
                and_hex_value(reg[reg_no], start, len, literal);

            else
            {
                int reg2_no = line[0] - 'A';
                int len2 = (line[1] == 'X' ? 4 : 2);
                int start2 = (line[1] == 'H' ? 2 : 0);
                add_hex_value(reg[reg_no], start, len, reg[reg2_no], start2, len2);
            }
        }

        else if (line == "JMP")
        {
            wtfil << opCode[line] << " ";
            rdfil >> line;
            string label = line;
            wtfil << symbolTable[label] << endl;
            int line_no = symbolTable[label] - starting_address;
            rdfil.close();
            rdfil.open("input.asm");

            int ct = 0;
            while (line_no != ct && getline(rdfil, line))
                ct++;
            rdfil >> line;
        }

        else if (line == "HLT")
        {
            wtfil << opCode[line];
            break;
        }

        else
            wtfil << endl;

        lines++;
    }

    cout << "Output of Pass 2 has been written in output.txt !!!" << endl
         << endl;
    cout << "Here is the value of registers after the program" << endl;
    for (int i = 0; i < 4; i++)
    {
        string str = "";
        str += (char)('A' + i);
        str += "X";
        cout << str << " ";
        show_reg(reg[i]);
    }

    rdfil.close();
    wtfil.close();
    return 0;
}
