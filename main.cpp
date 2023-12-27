#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
int m;
struct Node {
    int type; // -1  => uninitialized, 0 => leaf, 1 => non-leaf
    vector<std::pair<int, int>> keyValuePairs; 

    Node(int m) : type(-1), keyValuePairs(m, make_pair(-1, -1)) {}
};


void initializeBinaryFile(const std::string& filename, int numNodes, int m) {
    ofstream outFile(filename, std::ios::binary);

    if (!outFile.is_open()) {
        cerr << "Error opening file for writing." << std::endl;
        return;
    }

    int integerForNonNode = -1;
    outFile.write((char*)(&integerForNonNode), sizeof(integerForNonNode));

    // The second integer =>  index of the next free node
    int nextFreeNodeIndex = 1;
    outFile.write((char*)(&nextFreeNodeIndex), sizeof(nextFreeNodeIndex));


    for (int i = 0; i < (2 * m - 1); ++i) {
        outFile.write((char*)(&integerForNonNode), sizeof(integerForNonNode));
    }

    Node defaultNode(m);
    for (int i = 1; i < numNodes; ++i) {
        outFile.write((char*)(&defaultNode.type), sizeof(defaultNode.type));
        for (const auto& kv : defaultNode.keyValuePairs) {
            int k = kv.first;
            outFile.write((char*)(&k), sizeof(k));
            int v = kv.second;
            outFile.write((char*)(&v), sizeof(v));
        }
    }

    outFile.close();
}


void printFile(const string& filename, int m) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    cout << "I | T | ";
    for (int i = 1; i <= m; ++i) {
        cout << "K" << i << " | ";
        cout << "R" << i << " | ";
    }
    cout << endl;

    int index = 0;
    Node node(m);
    while (inFile.read((char*)(&node.type), sizeof(node.type))) {
        cout << index++ << " | " << (node.type == -1 ? "X" : (node.type == 0 ? "Y" : "N")) << " | ";
        for (int i = 0; i < m; ++i) {
            int key, value;
            inFile.read((char*)(&key), sizeof(key));
            inFile.read((char*)(&value), sizeof(value));
            cout << key << " | " << value << " | ";
        }
        cout << endl;
    }

    cout << "\n\nI=Index | T = Node Type (X = unintialized, Y=YES [leaf], N=NO [non leaf])\n K=Key | R = reference/pointer/value" << endl;


    inFile.close();
}

int readNextFreeNodeIndex(const string& filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cerr << "Error opening file for reading." << endl;
        return -1;
    }


    inFile.seekg(sizeof(int), ios::beg);

    int nextFreeNodeIndex;
    inFile.read((char*)(&nextFreeNodeIndex), sizeof(nextFreeNodeIndex));

    inFile.close();
    return nextFreeNodeIndex;
}

int InsertNewRecordAtIndex (char *filename, int RecordID, int Reference) {
    int nextFreeNodeIndex = readNextFreeNodeIndex(filename);

    fstream file(filename, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error: In opening file for reading and writing." << endl;
        return -1;
    }

    int nodeOffset = sizeof(int) * (1 + 2 * m) + sizeof(Node) * (nextFreeNodeIndex - 1);
    file.seekg(nodeOffset);

    Node node(m);
    file.read((char*)(&node.type), sizeof(node.type));
    if (node.type == -1) { // TODO
        node.type = 0;
    }

    for (int i = 0; i < m; ++i) {
        int k, v;
        file.read((char*)(&k), sizeof(k));
        file.read((char*)(&v), sizeof(v));
        node.keyValuePairs[i] = make_pair(k, v);
    }

    // Find right place for key
    int i;
    for (i = 0; i < m && node.keyValuePairs[i].first != -1 && node.keyValuePairs[i].first < RecordID; ++i);

    // Update reference only if key exist
    if (i < m && node.keyValuePairs[i].first == RecordID) {
        node.keyValuePairs[i].second = Reference;
    } else {
        // Shift
        for (int j = m - 1; j > i; --j) {
            node.keyValuePairs[j] = node.keyValuePairs[j - 1];
        }
        node.keyValuePairs[i] = make_pair(RecordID, Reference);
    }


    file.seekp(nodeOffset);
    file.write((char*)(&node.type), sizeof(node.type));
    for (const auto& kv : node.keyValuePairs) {
        int k = kv.first;
        file.write((char*)(&k), sizeof(k));
        int v = kv.second;
        file.write((char*)(&v), sizeof(v));
    }

    file.close();
    return 1; // TODO
}


int main()
{
    char* filename = "btree_index.bin";
    int numNodes = 10; // Number of nodes
    m = 5;             // Order of the B-tree

    initializeBinaryFile(filename, numNodes, m);

    // key, value
    InsertNewRecordAtIndex(filename,  15, 100);
    InsertNewRecordAtIndex(filename,  16, 99);
    InsertNewRecordAtIndex(filename,  19, 22);
    InsertNewRecordAtIndex(filename,  17, 80);
    InsertNewRecordAtIndex(filename,  11, 88);
    InsertNewRecordAtIndex(filename,  11, 19);


    printFile(filename, m);
    return 0;
}
