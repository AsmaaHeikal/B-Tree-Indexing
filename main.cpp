#include<bits/stdc++.h>

using namespace std;
int n,m;

// struct node{
// public:
//     int status;
//     int size;
//     int *arr;

//     node(){
//         this->status = -1;
//     }
//     node(int m, int next) {
//         this->size = m * 2;
//         this->status = -1;
//         arr = new int[size];
//         arr[0] = next;
//         for (int i = 1; i < (size); ++i) {
//             arr[i] = -1;
//         }
//     }
// };

struct Node
{
    int type;
    vector<pair<int, int>> keyValuePairs;
    bool isFull;
    Node(int m) : type(-1), keyValuePairs(m, make_pair(-1, -1)) {}
};

void CreateIndexFile (char* filename, int numberOfRecords, int m);
int InsertNewRecordAtIndex (char* filename, int RecordID, int Reference);
void DeleteRecordFromIndex (char* filename, int RecordID);
void DisplayIndexFileContent (char* filename);
int SearchARecord (char* filename, int RecordID);

int main() {
    n=10, m=5;
    CreateIndexFile ("BTreeIndexFile.bin", n, m);

    cout << "Welcome to the B-tree indexing file!:\n";
    while(true){
        cout << "\n1.Insert a new record\n"
                "2.Delete a record\n"
                "3.Display file content\n"
                "4.Search a record\n"
                "5.Exit\n";

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
//            InsertNewRecordAtIndex();
                break;
            case 2:
//            DeleteRecordFromIndex();
                break;
            case 3:
                DisplayIndexFileContent("BTreeIndexFile.bin");
                break;
            case 4:
//            SearchARecord();
                break;
            case 5:
                return 0;
            default:
                cout << "Invalid choice.Please try again!\n";
                break;
        }
    }
}

// void CreateIndexFile (char* filename, int numberOfRecords, int m){
//     ofstream Btree;
//     Btree.open(filename, ios::in | ios::out | ios::binary | ios::app);

//     // from 0 to numberOfRecords - 2 as the last one is a special case (-1)
//     for (int i = 0; i < numberOfRecords - 1; i++) {
//         node Node(m, i + 1);
//         // write the node to the file
//         Btree.write((char *) &Node, sizeof(Node));
//     }
//     // the last node is a special case(-1)
//     node Node(m, -1);
//     Btree.write((char *) &Node, sizeof(Node));

//     Btree.close();
// }


int InsertNewRecordAtIndex (char* filename, int RecordID, int Reference){

}

void DeleteRecordFromIndex (char* filename, int RecordID){

}

void CreateIndexFileFile(const char *filename, int numNodes, int m)
{
    ofstream outFile(filename, ios::binary);

    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    int cellNonNode = -1;
    outFile.write((char*)(&cellNonNode), sizeof(cellNonNode));

    int nextFreeNodeIndex = 1;
    outFile.write((char*)(&nextFreeNodeIndex), sizeof(nextFreeNodeIndex));


    for (int i = 0; i < (2 * m - 1); ++i)
    {
        outFile.write((char*)(&cellNonNode), sizeof(cellNonNode));
    }

    Node defaultNode(m);
    for (int i = 1; i < numNodes; ++i)
    {
        // all uninitialized yet type = -1
        outFile.write((char*)(&defaultNode.type), sizeof(defaultNode.type));
        for (const auto &kv : defaultNode.keyValuePairs)
        {
            int k = kv.first;
            outFile.write((char*)(&k), sizeof(k));
            int v = kv.second;
            outFile.write((char*)(&v), sizeof(v));
        }
    }

    outFile.close();
}

void DisplayIndexFileContent(const char *filename)
{
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open())
    {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    cout << "I | T | ";
    for (int i = 1; i <= m; ++i)
    {
        cout << "K" << i << " | ";
        cout << "R" << i << " | ";
    }
    cout << endl;

    int index = 0;
    Node node(m);
    while (inFile.read(reinterpret_cast<char *>(&node.type), sizeof(node.type)))
    {
        cout << index++ << " | " << (node.type == -1 ? "X" : (node.type == 0 ? "Y" : "N")) << " | ";
        for (int i = 0; i < m; ++i)
        {
            int key, value;
            inFile.read(reinterpret_cast<char *>(&key), sizeof(key));
            inFile.read(reinterpret_cast<char *>(&value), sizeof(value));
            cout << key << " | " << value << " | ";
        }
        cout << endl;
    }

    cout << "\n\nI=Index | T=Node Type (X=unintialized [= -1], Y=YES [leaf], N=NO [non leaf])\n K=Key | R=reference/value" << endl;


    inFile.close();
}

// void DisplayIndexFileContent (char* filename){
//     fstream Btree;
//     Btree.open(filename, ios::in | ios::out | ios::binary | ios::app);

//     Btree.seekg(0, ios::beg);
//     node Node;
//     // read the node from the file
//     while (Btree.read((char *) &Node, sizeof(Node))) {
//         cout << Node.status << " ";
//         for (int i = 0; i < Node.size; ++i) {
//             cout << Node.arr[i] << " ";
//         }
//         cout << endl;
//     }

//     Btree.close();
// }

int SearchARecord (char* filename, int RecordID){

}
