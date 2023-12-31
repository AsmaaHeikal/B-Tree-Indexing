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

    DeleteRecordFromIndex(filename, 10);
    DisplayIndexFileContent(filename);
    DeleteRecordFromIndex(filename, 9);
    DisplayIndexFileContent(filename);
    DeleteRecordFromIndex(filename, 7);
    DisplayIndexFileContent(filename);
    DeleteRecordFromIndex(filename, 15);
    DisplayIndexFileContent(filename);
    DeleteRecordFromIndex(filename, 11);
    DisplayIndexFileContent(filename);
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

void DeleteRecordFromIndex(char *filename, int RecordID){
    vector<Node> nodes=loadBTreeInMemory(filename, n, m);
    //check if the record is in the tree
    int ans=SearchARecord(filename, RecordID);
    if(ans==-1){
        cout<<"Record not found!\n";
        return;
    }
    //get the number of levels that the key is in
    int level=0;
    for(int i=0;i<nodes.size();i++){
        for(int j=0;j<nodes[i].keyValuePairs.size();j++) {
            if (nodes[i].keyValuePairs[j].first == RecordID) {
                level++;
            }
        }
    }
    //get the index of the node that contains the record
    int nodeIndex=0;
    int keyIndex=0;
    for(int i=0;i<nodes.size();i++){
        if(nodes[i].type==0){
            for(int j=0;j<nodes[i].keyValuePairs.size();j++){
                if(nodes[i].keyValuePairs[j].first==RecordID){
                    nodeIndex=i;
                    keyIndex=j;
                    break;
                }
            }
        }
    }

    //calc the actual number of keys in the node
    int NodeSize=0;
    for(int i=0;i<nodes[nodeIndex].keyValuePairs.size();i++){
        if(nodes[nodeIndex].keyValuePairs[i].first!=-1){
            NodeSize++;
        }
    }
    if(NodeSize>2){
        if(level==3||level==2){
            //I want to put a pair of -1,-1 in the place of the key
            nodes[nodeIndex].keyValuePairs[keyIndex].first=-1;
            nodes[nodeIndex].keyValuePairs[keyIndex].second=-1;
            //get the key that will replace the deleted key which is the last key in the node now
            int newKey=nodes[nodeIndex].keyValuePairs[keyIndex-1].first;
            cout<<newKey<<endl;

            if(level==2){
                //i want to loop on the nodes which type is 1 and find the node that has the deleted key to replace it with the new key
                for(int i=0;i<nodes.size();i++){
                    if(nodes[i].type==1){
                        for(int j=0;j<nodes[i].keyValuePairs.size();j++){
                            if(nodes[i].keyValuePairs[j].first==RecordID){
                                nodes[i].keyValuePairs[j].first=newKey;
                                break;
                            }
                        }
                    }
                }
            }
            else {
                //first search for the key in the first node of nodes vector then it with the new key
                for (int i = 0; i < nodes[1].keyValuePairs.size(); i++) {
                    if (nodes[1].keyValuePairs[i].first == RecordID) {
                        nodes[1].keyValuePairs[i].first = newKey;
                        break;
                    }
                }
                //then search in nodes that have type 1 for the deleted key with skipping the first node and replace the key with the new key
                for (int i = 2; i < nodes.size(); i++) {
                    if (nodes[i].type == 1) {
                        for (int j = 0; j < nodes[i].keyValuePairs.size(); j++) {
                            if (nodes[i].keyValuePairs[j].first == RecordID) {
                                nodes[i].keyValuePairs[j].first = newKey;
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if(level==1){
            //i want to shif the elements to the left by one
            for(int i=keyIndex;i<NodeSize-1;i++){
                nodes[nodeIndex].keyValuePairs[i]=nodes[nodeIndex].keyValuePairs[i+1];
            }
            nodes[nodeIndex].keyValuePairs[NodeSize-1].first=-1;
            nodes[nodeIndex].keyValuePairs[NodeSize-1].second=-1;
        }
    }
    else{
        //check if the predecessor on my parent node has more than 2 keys
        //get the predecessor node

        //get the last element in my node
        int lastKeyInCurrentNode=nodes[nodeIndex].keyValuePairs[NodeSize-1].first;
        cout<<lastKeyInCurrentNode<<endl;
        Node leftNode(m);
        Node rightNode(m);
        int leftNodeIndex=0;
        int rightNodeIndex=0;
        bool noLeftNode=true;
        bool noRightNode=true;
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i].type == 1) {
                for (int j = 0; j < nodes[i].keyValuePairs.size(); j++) {
                    if (nodes[i].keyValuePairs[j].first == lastKeyInCurrentNode) {
                        char s[] = "btree_index.bin";
                        if(nodes[i].keyValuePairs[j-1].second!= SearchARecord(s, nodes[i].keyValuePairs[j-1].first)){

                            leftNode=nodes[nodes[i].keyValuePairs[j-1].second];
                            leftNodeIndex=nodes[i].keyValuePairs[j-1].second;
                            noLeftNode=false;
                        }
                        if(nodes[i].keyValuePairs[j+1].second!= SearchARecord(s, nodes[i].keyValuePairs[j+1].first)){
                            rightNode=nodes[nodes[i].keyValuePairs[j+1].second];
                            rightNodeIndex=nodes[i].keyValuePairs[j+1].second;
                            noRightNode=false;
                        }
                        break;
                    }
                }
            }
        }
        int leftNodeSize=0;
        int rightNodeSize=0;
        for(int i=0;i<leftNode.keyValuePairs.size();i++){
            if(leftNode.keyValuePairs[i].first!=-1){
                leftNodeSize++;
            }
        }
        for(int i=0;i<rightNode.keyValuePairs.size();i++){
            if(rightNode.keyValuePairs[i].first!=-1){
                rightNodeSize++;
            }
        }

        if(!noLeftNode&&leftNodeSize>2){
            //get the key which is the last key in left node
            int newKey=leftNode.keyValuePairs[leftNodeSize-1].first;
            //get its reference
            int newReference=leftNode.keyValuePairs[leftNodeSize-1].second;
            int keyBeforeNewKey=leftNode.keyValuePairs[leftNodeSize-2].first;

            //i want to loop on the nodes which type is 1 and find the node that has the deleted key to replace it with the new key
            for(auto & node : nodes){
                if(node.type==1){
                    for(int j=0;j<node.keyValuePairs.size();j++){
                        if(node.keyValuePairs[j].first==newKey){
                            node.keyValuePairs[j].first=keyBeforeNewKey;
                            break;
                        }
                    }
                }
            }
            //remove the last key in left node
            leftNode.keyValuePairs[leftNodeSize-1].first=-1;
            leftNode.keyValuePairs[leftNodeSize-1].second=-1;
            nodes[leftNodeIndex]=leftNode;
            //now i want to shift the elements from beginning to the right by one until reaching the deleted key
            for(int i=0;i<keyIndex;i++){
                nodes[nodeIndex].keyValuePairs[i+1]=nodes[nodeIndex].keyValuePairs[i];
            }
            //now i want to put the new key in the first place in the node
            nodes[nodeIndex].keyValuePairs[0].first=newKey;
            nodes[nodeIndex].keyValuePairs[0].second=newReference;
            //now i want to delete the deleted key from all levels
            if(level==3||level==2){
                //I want to put a pair of -1,-1 in the place of the key
//                nodes[nodeIndex].keyValuePairs[keyIndex].first=-1;
//                nodes[nodeIndex].keyValuePairs[keyIndex].second=-1;
                //get the key that will replace the deleted key which is the last key in the node now
                int newKey=nodes[nodeIndex].keyValuePairs[NodeSize-1].first;

                if(level==2){
                    //i want to loop on the nodes which type is 1 and find the node that has the deleted key to replace it with the new key
                    for(int i=0;i<nodes.size();i++){
                        if(nodes[i].type==1){
                            for(int j=0;j<nodes[i].keyValuePairs.size();j++){
                                if(nodes[i].keyValuePairs[j].first==RecordID){
                                    nodes[i].keyValuePairs[j].first=newKey;
                                    break;
                                }
                            }
                        }
                    }
                }
                else {
                    //first search for the key in the first node of nodes vector then it with the new key
                    for (int i = 0; i < nodes[1].keyValuePairs.size(); i++) {
                        if (nodes[1].keyValuePairs[i].first == RecordID) {
                            nodes[1].keyValuePairs[i].first = newKey;
                            break;
                        }
                    }
                    //then search in nodes that have type 1 for the deleted key with skipping the first node and replace the key with the new key
                    for (int i = 2; i < nodes.size(); i++) {
                        if (nodes[i].type == 1) {
                            for (int j = 0; j < nodes[i].keyValuePairs.size(); j++) {
                                if (nodes[i].keyValuePairs[j].first == RecordID) {
                                    nodes[i].keyValuePairs[j].first = newKey;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else if(level==1){
                //i want to shift the elements to the left by one
                for(int i=keyIndex;i<NodeSize-1;i++){
                    nodes[nodeIndex].keyValuePairs[i]=nodes[nodeIndex].keyValuePairs[i+1];
                }
                nodes[nodeIndex].keyValuePairs[NodeSize-1].first=-1;
                nodes[nodeIndex].keyValuePairs[NodeSize-1].second=-1;
            }
        }
        else if(!noRightNode&&rightNodeSize>2) {
            //get the key which is the first key in right node
            int newKey=rightNode.keyValuePairs[0].first;
            //get its reference
            int newReference=rightNode.keyValuePairs[0].second;
            //shift the elements in right node to the left by one
            for(int i=0;i<rightNodeSize-1;i++){
                rightNode.keyValuePairs[i]=rightNode.keyValuePairs[i+1];
            }
            //remove the last key in right node
            rightNode.keyValuePairs[rightNodeSize-1].first=-1;
            rightNode.keyValuePairs[rightNodeSize-1].second=-1;
            nodes[rightNodeIndex]=rightNode;
            //now i will delete the deleted key from the current node and shift items to the left by one and add the new key in the last place
            for(int i=keyIndex;i<NodeSize-1;i++){
                nodes[nodeIndex].keyValuePairs[i]=nodes[nodeIndex].keyValuePairs[i+1];
            }
            //now i want to put the new key in the last place in the node
            nodes[nodeIndex].keyValuePairs[NodeSize-1].first=newKey;
            nodes[nodeIndex].keyValuePairs[NodeSize-1].second=newReference;
            //now i want to delete the deleted key from the nodes that have type 1 except the first node
            for(int i=0;i<nodes.size();i++){
                if(nodes[i].type==1){
                    for(int j=0;j<nodes[i].keyValuePairs.size();j++){
                        if(nodes[i].keyValuePairs[j].first==lastKeyInCurrentNode){
                            nodes[i].keyValuePairs[j].first=newKey;
                            break;
                        }
                    }
                }
//            }
//            //now i want to delete the deleted key from the first node
//            for(int i=0;i<nodes[1].keyValuePairs.size();i++){
//                if(nodes[1].keyValuePairs[i].first==lastKeyInCurrentNode){
//                    nodes[1].keyValuePairs[i].first=newKey;
//                    break;
//                }
            }
        }
        else{
            //merge the left node with the current node

            //get the first key and the second key in current node
            int firstKeyInCurrentNode=nodes[nodeIndex].keyValuePairs[0].first;
            int secondKeyInCurrentNode=nodes[nodeIndex].keyValuePairs[1].first;

            //put the type of the node to -1
            nodes[nodeIndex].type=-1;
            //delete all the keys in the current node
            for(int i=0;i<nodes[nodeIndex].keyValuePairs.size();i++){
                nodes[nodeIndex].keyValuePairs[i].first=-1;
                nodes[nodeIndex].keyValuePairs[i].second=-1;
            }
            //update the first key in the zero node to be the index of the empty node
            nodes[0].keyValuePairs[0].first=nodeIndex;

            //get the last key in left node
            int lastKeyInLeftNode=leftNode.keyValuePairs[leftNodeSize-1].first;

            //check if the deleted key is in the first element in the node or the second
            if(keyIndex==0){
                //add pair to the left node,the pair contains the second key in the current node and its reference
                char s[] ="btree_index.bin";
                leftNode.keyValuePairs[leftNodeSize].first=secondKeyInCurrentNode;
                leftNode.keyValuePairs[leftNodeSize].second=SearchARecord( s,secondKeyInCurrentNode);
//                leftNode.keyValuePairs.insert(leftNode.keyValuePairs.end(),make_pair(secondKeyInCurrentNode,SearchARecord( s,secondKeyInCurrentNode)));
                //find the node that contains the last Key in left node and shift the elements from the end to the right by one
                for(int i=0;i<nodes.size();i++){
                    if(nodes[i].type==1){
                        for(int j=0;j<nodes[i].keyValuePairs.size();j++){
                            if(nodes[i].keyValuePairs[j].first==lastKeyInLeftNode) {
                                //get the actual number of keys in the node
                                int INodeSize=0;
                                for(int i=0;i<nodes[i].keyValuePairs.size();i++){
                                    if(nodes[i].keyValuePairs[i].first!=-1){
                                        INodeSize++;
                                    }
                                }
                                for (int k = j; k < nodes[i].keyValuePairs.size() - 1; k++) {
                                    nodes[i].keyValuePairs[k] = nodes[i].keyValuePairs[k + 1];
                                }
                                nodes[i].keyValuePairs[INodeSize+1].first = -1;
                                nodes[i].keyValuePairs[INodeSize+1].second = -1;
                                break;
                            }
                        }
                    }
                }
            }
            else{
                //put first key in current node in the last place in left node
                char s[] ="btree_index.bin";
                leftNode.keyValuePairs[leftNodeSize].first=firstKeyInCurrentNode;
                leftNode.keyValuePairs[leftNodeSize].second=SearchARecord(s,firstKeyInCurrentNode);
//                leftNode.keyValuePairs.insert(leftNode.keyValuePairs.end(),make_pair(firstKeyInCurrentNode,SearchARecord(s,firstKeyInCurrentNode)));
                //loop on the tree and if found the deleted key replace it with the second key in current node
                for(auto & node : nodes){
                    if(node.type==1){
                        for(int j=0;j<node.keyValuePairs.size();j++){
                            if(node.keyValuePairs[j].first==RecordID){
                                node.keyValuePairs[j].first=firstKeyInCurrentNode;
                                break;
                            }
                        }
                    }
                }
                //find the node that contains the last Key in left node and shift the elements from the end to the right by one
                for(int i=0;i<nodes.size();i++){
                    if(nodes[i].type==1){
                        for(int j=0;j<nodes[i].keyValuePairs.size();j++){
                            if(nodes[i].keyValuePairs[j].first==lastKeyInLeftNode) {
                                //get the actual number of keys in the node
                                int INodeSize=0;
                                for(int i=0;i<nodes[i].keyValuePairs.size();i++){
                                    if(nodes[i].keyValuePairs[i].first!=-1){
                                        INodeSize++;
                                    }
                                }
                                for (int k = j; k < nodes[i].keyValuePairs.size() - 1; k++) {
                                    nodes[i].keyValuePairs[k] = nodes[i].keyValuePairs[k + 1];
                                }
                                nodes[i].keyValuePairs[INodeSize+1].first = -1;
                                nodes[i].keyValuePairs[INodeSize+1].second = -1;
                                break;
                            }
                        }
                    }
                }
            }
            //update the left node in the nodes vector
            nodes[leftNodeIndex]=leftNode;
        }
    }
    writeBTreeToFile(filename, nodes, m);
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