#include<bits/stdc++.h>

using namespace std;

void CreateIndexFile (char* filename, int numberOfRecords, int m);
int InsertNewRecordAtIndex (char* filename, int RecordID, int Reference);
void DeleteRecordFromIndex (char* filename, int RecordID);
void DisplayIndexFileContent (char* filename);
int SearchARecord (char* filename, int RecordID);

int main(){
    int choice;
    cin >> choice;
    cout << "Welcome to the B-tree indexing file!:\n";
    while(true){
        cout << "1.Insert a new record\n"
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
//            DisplayIndexFileContent();
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

void CreateIndexFileFile (char* filename, int numberOfRecords, int m){

}
int InsertNewRecordAtIndex (char* filename, int RecordID, int Reference){

}
void DeleteRecordFromIndex (char* filename, int RecordID){

}
void DisplayIndexFileContent (char* filename){

}
int SearchARecord (char* filename, int RecordID){

}