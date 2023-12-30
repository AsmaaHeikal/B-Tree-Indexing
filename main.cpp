#include <bits/stdc++.h>

using namespace std;
int n, m;

struct Node
{
    int type;
    vector<pair<int, int>> keyValuePairs;
    bool isFull;
    Node(int m) : type(-1), keyValuePairs(m, make_pair(-1, -1)) {}
};

// main functions
void CreateIndexFileFile(char *filename, int numberOfRecords, int m);
int InsertNewRecordAtIndex(char *filename, int RecordID, int Reference);
void DeleteRecordFromIndex(char *filename, int RecordID);
void DisplayIndexFileContent(char *filename);
int SearchARecord(char *filename, int RecordID);

// utils
vector<Node> loadBTreeInMemory(char *filename, int numNodes, int m);

void writeBTreeToFile(char *filename, const vector<Node> nodes, int m);

int findHighestKey(const vector<pair<int, int>> &keyValuePairs);

void splitRootNode(vector<Node> &nodes, int m, int newKey);

void SimpleInsert(Node &node, int RecordID, int Reference);

int splitChild(vector<Node> &nodes, int parentIndex, int childIndex, int newKey);

void updateRootNodeContent(vector<Node> &nodes);

void updateNextEmptyNodeIndex(vector<Node> &nodes);

int main()
{
    char filename[] = "btree_index.bin";
    n = 10, m = 5;
    CreateIndexFileFile(filename, n, m);
    InsertNewRecordAtIndex(filename, 3, 12);
    InsertNewRecordAtIndex(filename, 7, 24);
    InsertNewRecordAtIndex(filename, 10, 48);
    InsertNewRecordAtIndex(filename, 24, 60);
    InsertNewRecordAtIndex(filename, 14, 72);
    InsertNewRecordAtIndex(filename, 19, 48);
    InsertNewRecordAtIndex(filename, 30, 96);
    InsertNewRecordAtIndex(filename, 15, 108);
    InsertNewRecordAtIndex(filename, 1, 120);
    InsertNewRecordAtIndex(filename, 5, 132);

    InsertNewRecordAtIndex(filename, 2, 144);

    InsertNewRecordAtIndex(filename, 8, 156);
    InsertNewRecordAtIndex(filename, 9, 168);
    InsertNewRecordAtIndex(filename, 6, 180);
    InsertNewRecordAtIndex(filename, 11, 192);
    InsertNewRecordAtIndex(filename, 12, 204);
    InsertNewRecordAtIndex(filename, 18, 228);
    InsertNewRecordAtIndex(filename, 17, 216);
    DisplayIndexFileContent(filename);

    cout << SearchARecord(filename, 3) << endl;
    cout << SearchARecord(filename, 7) << endl;
    cout << SearchARecord(filename, 10) << endl;
    cout << SearchARecord(filename, 24) << endl;
    cout << SearchARecord(filename, 14) << endl;
    cout << SearchARecord(filename, 19) << endl;
    cout << SearchARecord(filename, 30) << endl;
    cout << SearchARecord(filename, 15) << endl;
    cout << SearchARecord(filename, 1) << endl;
    cout << SearchARecord(filename, 5) << endl;
    cout << SearchARecord(filename, 2) << endl;
    cout << SearchARecord(filename, 8) << endl;
    cout << SearchARecord(filename, 9) << endl;
    cout << SearchARecord(filename, 6) << endl;
    cout << SearchARecord(filename, 11) << endl;
    cout << SearchARecord(filename, 12) << endl;
    cout << SearchARecord(filename, 18) << endl;
    cout << SearchARecord(filename, 17) << endl;

    cout << SearchARecord(filename, 13) << endl;
    cout << SearchARecord(filename, 20) << endl;
    cout << SearchARecord(filename, 16) << endl;

    // Main Menu
    //    cout << "Welcome to the B-tree indexing file!:\n";
    //    while(true){
    //        cout << "\n1.Insert a new record\n"
    //                "2.Delete a record\n"
    //                "3.Display file content\n"
    //                "4.Search a record\n"
    //                "5.Exit\n";
    //
    //        int choice;
    //        cout << "Enter your choice: ";
    //        cin >> choice;
    //        switch (choice) {
    //            case 1:
    ////            InsertNewRecordAtIndex();
    //                break;
    //            case 2:
    ////            DeleteRecordFromIndex();
    //                break;
    //            case 3:
    //                DisplayIndexFileContent("BTreeIndexFile.bin");
    //                break;
    //            case 4:
    ////            SearchARecord();
    //                break;
    //            case 5:
    //                return 0;
    //            default:
    //                cout << "Invalid choice.Please try again!\n";
    //                break;
    //        }
    //    }
    //}
}

void DeleteRecordFromIndex(char *filename, int RecordID)
{
}

void CreateIndexFileFile(char *filename, int numberOfRecords, int m)
{
    ofstream outFile(filename, ios::binary);

    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    int cellNonNode = -1;
    outFile.write((char *)(&cellNonNode), sizeof(cellNonNode));

    int nextFreeNodeIndex = 1;
    outFile.write((char *)(&nextFreeNodeIndex), sizeof(nextFreeNodeIndex));

    for (int i = 0; i < (2 * m - 1); ++i)
    {
        outFile.write((char *)(&cellNonNode), sizeof(cellNonNode));
    }

    Node defaultNode(m);
    for (int i = 1; i < numberOfRecords; ++i)
    {
        // all uninitialized yet type = -1
        outFile.write((char *)(&defaultNode.type), sizeof(defaultNode.type));
        for (const auto &kv : defaultNode.keyValuePairs)
        {
            int k = kv.first;
            outFile.write((char *)(&k), sizeof(k));
            int v = kv.second;
            outFile.write((char *)(&v), sizeof(v));
        }
    }

    outFile.close();
}

void DisplayIndexFileContent(char *filename)
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
    while (inFile.read((char *)(&node.type), sizeof(node.type)))
    {
        cout << index++ << " | " << (node.type == -1 ? "X" : (node.type == 0 ? "Y" : "N")) << " | ";
        for (int i = 0; i < m; ++i)
        {
            int key, value;
            inFile.read((char *)(&key), sizeof(key));
            inFile.read((char *)(&value), sizeof(value));
            cout << key << " | " << value << " | ";
        }
        cout << endl;
    }

    cout << "\n\nI=Index | T=Node Type (X=unintialized [= -1], Y=YES [leaf], N=NO [non leaf])\n K=Key | R=reference/value" << endl;

    inFile.close();
}

int SearchARecord(char *filename, int RecordID)
{
    vector<Node> nodes = loadBTreeInMemory(filename, n, m);
    Node root = nodes[1];
    int ans = -1, target = -1;

    // If the current node is a leaf node
    if (!root.type)
    {
        for (int i = 0; i < root.keyValuePairs.size() and root.keyValuePairs[i].first != -1; i++)
        {
            if (root.keyValuePairs[i].first == RecordID)
            {
                ans = root.keyValuePairs[i].second;
                break;
            }
        }
    }
    // If the current node is a non-leaf node
    else
    {
        bool entered = false;
        for (int i = 0; i < root.keyValuePairs.size() and root.keyValuePairs[i].first != -1; i++)
        {
            // Find the target node for RecordID
            if (root.keyValuePairs[i].first >= RecordID)
            {
                target = root.keyValuePairs[i].second;
                break;
            }
        }
        // If target is not found, set it to the last available target
        if (target == -1)
            target = root.keyValuePairs[root.keyValuePairs.size() - 1].second;

        Node node = nodes[target];
        // While the current node is not a leaf node
        while (node.type)
        {
            entered = true;
            int j;
            for (j = 0; j < node.keyValuePairs.size() and node.keyValuePairs[j].first != -1; j++)
            {
                if (node.keyValuePairs[j].first >= RecordID)
                {
                    target = node.keyValuePairs[j].second;
                    break;
                }
            }
            if (target == -1)
                target = node.keyValuePairs[node.keyValuePairs.size() - 1].second;

            node = nodes[target];
        }

        // Check if the loop was entered at least once
        if (!entered)
        { // If not entered, it means the initial node itself is a leaf node
            for (int i = 0; i < node.keyValuePairs.size() and node.keyValuePairs[i].first != -1; i++)
            {
                if (node.keyValuePairs[i].first == RecordID)
                {
                    ans = node.keyValuePairs[i].second;
                    break;
                }
            }
        }

        else
        { // If entered, it means the loop reached a leaf node
            for (int i = 0; i < node.keyValuePairs.size() and node.keyValuePairs[i].first != -1; i++)
            {
                if (node.keyValuePairs[i].first == RecordID)
                {
                    ans = node.keyValuePairs[i].second;
                    break;
                }
            }
        }
    }
    return ans;
}

// ################### insertion ###################
int InsertNewRecordAtIndex(char *filename, int RecordID, int Reference)
{

    vector<Node> nodes = loadBTreeInMemory(filename, n, m);

    // root is at index 1
    Node &node = nodes[1];

    if (node.type == -1)
    {
        node.type = 0;
        SimpleInsert(node, RecordID, Reference);
    }
    else
    {
        if (node.isFull && node.type == 0)
        {
            splitRootNode(nodes, m, RecordID);

            // insert the new record into the right node
            int rootNodeIndex = nodes[1].type == 0 ? 1 : (RecordID < nodes[1].keyValuePairs[0].first ? 2 : 3);
            SimpleInsert(nodes[rootNodeIndex], RecordID, Reference);
        }
        else if (node.type == 1)
        {
            // Find the right child based on the key
            int childIndex = -1;
            int i;
            for (i = 0; i < m; ++i)
            {
                if (node.keyValuePairs[i].first > RecordID)
                {
                    childIndex = node.keyValuePairs[i].second;
                    break;
                }

                // in case new key is bigger than all of the existing ones in root
                // then take the last one once you see a key = -1
                if (node.keyValuePairs[i].first == -1)
                {
                    childIndex = node.keyValuePairs[i - 1].second;
                    i--;
                    break;
                }
            }

            if (childIndex == -1)
            {
                //  if all keys are less than RecordID
                childIndex = node.keyValuePairs[m - 1].second;
                i = m - 1;
            }

            if (nodes[childIndex].isFull)
            {
                int newChildIndex = splitChild(nodes, 1, childIndex, RecordID);

                SimpleInsert(nodes[newChildIndex], RecordID, Reference);

                updateRootNodeContent(nodes);
                updateNextEmptyNodeIndex(nodes);
            }

            SimpleInsert(nodes[childIndex], RecordID, Reference);

            // in case the new key than the existing key in root
            // update the key with the one of highest value
            node.keyValuePairs[i].first = findHighestKey(nodes[childIndex].keyValuePairs);
        }
        else
        {
            SimpleInsert(node, RecordID, Reference);
        }
    }

    updateNextEmptyNodeIndex(nodes);
    writeBTreeToFile(filename, nodes, m);

    return 0;
}

// utils
vector<Node> loadBTreeInMemory(char *filename, int numNodes, int m)
{
    ifstream inFile(filename, ios::binary);
    vector<Node> nodes;

    if (!inFile.is_open())
    {
        cerr << "Error opening file for reading." << endl;
        return nodes;
    }

    for (int i = 0; i < numNodes; ++i)
    {
        Node node(m);
        inFile.read((char *)(&node.type), sizeof(node.type));
        for (auto &kv : node.keyValuePairs)
        {
            inFile.read((char *)(&kv.first), sizeof(kv.first));
            inFile.read((char *)(&kv.second), sizeof(kv.second));
        }
        node.isFull = (node.keyValuePairs.back().first != -1 && node.keyValuePairs.back().second != -1);

        nodes.push_back(node);
    }

    inFile.close();
    return nodes;
}

void writeBTreeToFile(char *filename, vector<Node> nodes, int m)
{
    ofstream outFile(filename, ios::binary);

    if (!outFile.is_open())
    {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    for (const auto &node : nodes)
    {
        outFile.write((char *)(&node.type), sizeof(node.type));
        for (const auto kv : node.keyValuePairs)
        {
            outFile.write((char *)(&kv.first), sizeof(kv.first));
            outFile.write((char *)(&kv.second), sizeof(kv.second));
        }
    }

    outFile.close();
}

void SimpleInsert(Node &node, int RecordID, int Reference)
{
    int i = 0;
    while (i < m && node.keyValuePairs[i].first != -1 && node.keyValuePairs[i].first < RecordID)
    {
        i++;
    }

    if (i < m && node.keyValuePairs[i].first == RecordID)
    {
        return;
    }
    else
    {

        for (int j = m - 1; j > i; --j)
        {
            node.keyValuePairs[j] = node.keyValuePairs[j - 1];
        }
        node.keyValuePairs[i] = {RecordID, Reference};
    }
}

void splitRootNode(vector<Node> &nodes, int m, int newKey)
{
    int leftChildIndex = nodes[0].keyValuePairs[0].first;
    int rightChildIndex = leftChildIndex + 1;

    Node &leftChild = nodes[leftChildIndex];
    Node &rightChild = nodes[rightChildIndex];
    leftChild.type = 0;
    rightChild.type = 0;

    int medianKey = nodes[1].keyValuePairs[m / 2].first;
    int medianIndex = newKey <= medianKey ? m / 2 : m / 2 + 1;

    int maxKeyLeft = -1, maxKeyRight = -1;

    for (int i = 0; i < medianIndex; ++i)
    {
        leftChild.keyValuePairs[i] = nodes[1].keyValuePairs[i];
        maxKeyLeft = max(maxKeyLeft, leftChild.keyValuePairs[i].first);
    }
    for (int i = medianIndex; i < m; ++i)
    {
        rightChild.keyValuePairs[i - medianIndex] = nodes[1].keyValuePairs[i];
        maxKeyRight = std::max(maxKeyRight, rightChild.keyValuePairs[i - medianIndex].first);
        nodes[1].keyValuePairs[i] = make_pair(-1, -1);
    }

    nodes[1].type = 1;
    nodes[1].keyValuePairs[0] = make_pair(maxKeyLeft, leftChildIndex);
    nodes[1].keyValuePairs[1] = make_pair(maxKeyRight, rightChildIndex);

    for (int i = 2; i < m; ++i)
    {
        nodes[1].keyValuePairs[i] = make_pair(-1, -1);
    }
}

int splitChild(vector<Node> &nodes, int parentIndex, int childIndex, int newKey)
{
    Node &parent = nodes[parentIndex];
    Node &child = nodes[childIndex];

    int medianKey = child.keyValuePairs[m / 2].first;
    int medianIndex = newKey <= medianKey ? m / 2 : m / 2 + 1;

    // Create new node using first existing free node
    int newChildIndex = nodes[0].keyValuePairs[0].first;
    Node &newNode = nodes[newChildIndex];
    newNode.type = 0;

    for (int i = medianIndex; i < m; ++i)
    {
        newNode.keyValuePairs[i - medianIndex] = child.keyValuePairs[i];
        child.keyValuePairs[i] = make_pair(-1, -1);
    }

    // depending on the new key and the chosen median
    // determine which node index the new key-value pair will go to
    // (the new child node or the other child node we are now splitting)
    return newKey <= medianKey ? childIndex : newChildIndex;
}

void updateNextEmptyNodeIndex(vector<Node> &nodes)
{
    int newIndex = 1;
    for (int i = 0; i < nodes.size(); i++)
    {
        // skip next free node indicator
        if (i == 0)
        {
            continue;
        }

        if (nodes[i].type != -1)
        {
            newIndex++;
        }
    }

    nodes[0].keyValuePairs[0].first = newIndex;
}

int findHighestKey(const vector<pair<int, int>> &keyValuePairs)
{
    int highestKey = numeric_limits<int>::min();

    for (const auto &kv : keyValuePairs)
    {
        if (kv.first > highestKey)
        {
            highestKey = kv.first;
        }
    }

    return highestKey;
}

void updateRootNodeContent(vector<Node> &nodes)
{
    Node &root = nodes[1];

    for (int i = 0; i < root.keyValuePairs.size(); i++)
    {
        root.keyValuePairs[i] = make_pair(-1, -1);
    }

    for (int i = 2; i < nodes.size() && nodes[i].type == 0; i++)
    {
        int maxKeyForChildNode = findHighestKey(nodes[i].keyValuePairs);
        SimpleInsert(root, maxKeyForChildNode, i);
    }
}