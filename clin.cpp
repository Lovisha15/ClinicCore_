
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>
using namespace std; 

// ====================== Class ======================
class Patient {
public:
    int id;
    string name;
    int age;
    string gender;
    string contact;
    float weight;
    string problem;
    string date;
};

// ====================== Global Data ======================
vector<Patient> patients;

// ====================== Utility Functions ======================
string getDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", ltm);
    return string(date);
}

int getInt(string prompt, int minV, int maxV) {
    string s; int val;
    while (true) {
        cout << prompt;
        getline(cin, s);
        bool valid = !s.empty();
        for (char c : s) if (!isdigit(c)) valid = false;
        if (!valid) { cout << "Invalid input.\n"; continue; }
        val = stoi(s);
        if (val < minV || val > maxV) { cout << "Out of range.\n"; continue; }
        return val;
    }
}

float getFloat(string prompt, float minV, float maxV) {
    string s; float val;
    while (true) {
        cout << prompt;
        getline(cin, s);
        bool valid = !s.empty(), dot=false;
        for(char c:s){
            if(c=='.'){ if(dot) {valid=false; break;} dot=true; }
            else if(!isdigit(c)) { valid=false; break; }
        }
        if(!valid){ cout<<"Invalid input!\n"; continue; }
        val = stof(s);
        if(val<minV || val>maxV){ cout<<"Out of range.\n"; continue; }
        return val;
    }
}

string getName() {
    string s;
    while (true) {
        cout << "Name: "; getline(cin, s);
        if (s.length() < 2 || !isalpha(s[0])) { cout << "Invalid!\n"; continue; }
        bool ok=true;
        for(char c:s) if(!isalpha(c)&&c!=' ') ok=false;
        if(!ok){ cout<<"Letters and spaces only.\n"; continue; }
        return s;
    }
}

string getContact() {
    string s;
    while(true){
        cout<<"Contact (10 digits): "; getline(cin,s);
        if(s.length()!=10){ cout<<"Must be 10 digits\n"; continue;}
        bool ok=true; for(char c:s) if(!isdigit(c)) ok=false;
        if(!ok){ cout<<"Invalid input!\n"; continue; }
        return s;
    }
}

string getGender(){
    int g;
    while(true){
        cout<<"Gender [1.Male 2.Female 3.Other]: ";
        g=getInt("",1,3);
        if(g==1) return "Male";
        if(g==2) return "Female";
        if(g==3) return "Other";
    }
}

// ====================== File Operations ======================
void loadFile(){
    patients.clear();
    ifstream f("clinical_records.txt");
    string line; 
    while(getline(f,line)){
        if(line.empty()) continue;
        stringstream ss(line); string t; vector<string> tok;
        while(getline(ss,t,',')) tok.push_back(t);
        if(tok.size()==8){
            Patient p;
            p.id = stoi(tok[0]);
            p.name = tok[1];
            p.age = stoi(tok[2]);
            p.gender = tok[3];
            p.contact = tok[4];                 
            p.weight = stof(tok[5]);
            p.problem = tok[6];
            p.date = tok[7];
            patients.push_back(p);
        }
    }
    f.close();
}

void saveFile(){
    ofstream f("clinical_records.txt");
    for(auto&p:patients)
        f<<p.id<<","<<p.name<<","<<p.age<<","<<p.gender<<","<<p.contact<<","<<p.weight<<","<<p.problem<<","<<p.date<<"\n";
    f.close();
}

// ====================== Core Operations ======================
void addPatient(){
    Patient p;
    p.id = patients.empty()?1:patients.back().id+1;
    cout<<"ID: "<<p.id<<endl;
    p.name=getName();
    p.age=getInt("Age: ",1,120);
    p.gender=getGender();
    p.contact=getContact();
    p.weight=getFloat("Weight: ",1,500);
    cout<<"Problem: "; getline(cin,p.problem);
    p.date=getDate();
    patients.push_back(p);
    saveFile();
    cout << "\nPatient added successfully!\n";
}

void viewRecords(){
    if(patients.empty()){ cout<<"No records.\n"; return;}
    cout << "\n===== PATIENT RECORDS =====\n";
    cout<<left<<setw(5)<<"ID"<<setw(15)<<"Name"<<setw(5)<<"Age"<<setw(8)<<"Gender"<<setw(12)<<"Contact"<<setw(7)<<"Weight"<<setw(20)<<"Problem"<<setw(12)<<"Date"<<endl;
    cout<<string(84,'-')<<endl;
    for(auto&p:patients)
        cout<<left<<setw(5)<<p.id<<setw(15)<<p.name<<setw(5)<<p.age<<setw(8)<<p.gender<<setw(12)<<p.contact<<setw(7)<<p.weight<<setw(20)<<p.problem<<setw(12)<<p.date<<endl;
}

void deleteRecord(){
    int id=getInt("Enter ID to delete: ",1,999999);
    for(auto it=patients.begin();it!=patients.end();++it){
        if(it->id==id){
            char c; cout<<"Confirm delete? (y/n): "; cin>>c; cin.ignore();
            if(c=='y'||c=='Y'){ patients.erase(it); saveFile(); cout<<"Deleted.\n"; }
            return;
        }
    }
    cout<<"Not found.\n";
}

void updateRecord(){
    int id=getInt("Enter ID to update: ",1,999999);
    for(auto &p:patients){
        if(p.id==id){
            cout<<"1.Name 2.Age 3.Gender 4.Contact 5.Weight 6.Problem\n";
            int choice=getInt("Field to update: ",1,6);
            switch(choice){
                case 1: p.name=getName(); break;
                case 2: p.age=getInt("Age: ",1,120); break;
                case 3: p.gender=getGender(); break;
                case 4: p.contact=getContact(); break;
                case 5: p.weight=getFloat("Weight: ",1,500); break;
                case 6: cout<<"Problem: "; getline(cin,p.problem); break;
            }
            p.date=getDate(); saveFile(); cout << "Record updated successfully!\n"; return;
        }
    }
    cout<<"ID not found.\n";
}

void searchById(){
    int id=getInt("Enter ID to search: ",1,999999);
    cout << string(40,'-') << endl;
    int l=0,r=patients.size()-1; bool found=false;
    while(l<=r){
        int m=(l+r)/2;
        if(patients[m].id==id){ found=true;
            auto&p=patients[m];
            cout<<"ID:"<<p.id<<"\nName:"<<p.name<<"\nAge:"<<p.age<<"\nGender:"<<p.gender<<"\nContact:"<<p.contact<<"\nWeight:"<<p.weight<<"\nProblem:"<<p.problem<<"\nDate:"<<p.date<<endl;
            cout << string(40,'-') << endl;
            break;
        } 
        else if(patients[m].id<id) l=m+1;
        else r=m-1;
    }
    if(!found) cout<<"Not found.\n";
} 

// ====================== Main ======================
int main(){
    loadFile();
    int choice;
    do{
        cout << "\n====== Clinical Management System ======\n";
        cout << "1. Add Patient Record\n";
        cout << "2. View All Records\n";
        cout << "3. Delete Record\n";
        cout << "4. Search Record by ID\n";
        cout << "5. Update Patient Record\n";
        cout << "6. Exit\n";
        choice=getInt("Enter your choice (1-6): ",1,6);
        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewRecords(); break;
            case 3: deleteRecord(); break;
            case 4: searchById(); break;
            case 5: updateRecord(); break;
            case 6:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please select 1-6.\n";
        }
    } while (choice != 6);

    return 0;
}