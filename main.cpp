#include "main.h"

using namespace std;

int openBF1942(const char* modID = 0, const char* ip_port = 0, const char* password = 0){
    string execPath("BF1942.exe +restart 1");
    if(modID){
        execPath.append(" +game ");
        execPath.append(modID);
    }
    if(ip_port){
        execPath.append(" +joinServer ");
        execPath.append(ip_port);
    }else
        execPath.append(" +goToInterface 6");
    if(password){
        execPath.append(" +password ");
        execPath.append(password);
    }
    return WinExec(execPath.c_str(), SW_SHOWNORMAL);
}

int clientAwknowlage(){
    string word;
    while(1){
        if (word == "yes"){
            cin.ignore();
            return(1);
        }
        else if (word == "no"){
            cin.ignore();
            return(0);
        }
        cin >> word;
    }
}

void clientAwknowlage2(){
    cout << "Press enter to go back to the game." << endl;
    getchar();
}

int patchClientExe(){
    const char *patches[N_PATCHES] = {
        PATCH_1,
        PATCH_2,
        PATCH_3,
        PATCH_4
    };
    int patch_locations[N_PATCHES] = {
        PATCH_1_POS,
        PATCH_2_POS,
        PATCH_3_POS,
        PATCH_4_POS
    };
    int patch_sizes[N_PATCHES] = {
        sizeof(PATCH_1) - 1,
        sizeof(PATCH_2) - 1,
        sizeof(PATCH_3) - 1,
        sizeof(PATCH_4) - 1
    };

    ofstream client_exe;
    client_exe.open(CLIENT_EXE, fstream::out|fstream::binary|fstream::in);

    if(!client_exe.is_open()){
        cout << "Error at open(): " << CLIENT_EXE << endl;
        return(0);
    }

    for(unsigned int i=0; i<N_PATCHES; i++){
        client_exe.seekp(patch_locations[i]);
        client_exe.write(patches[i], patch_sizes[i]);
    }
    return(1);
}

void LoadFileInResource(int name, int type, DWORD& size, const char*& data){
    HMODULE handle = ::GetModuleHandle(NULL);
    HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
    HGLOBAL rcData = ::LoadResource(handle, rc);
    size = ::SizeofResource(handle, rc);
    data = static_cast<const char*>(::LockResource(rcData));
}

int addDataField42Exe(){
    DWORD size = 0;
    const char* data = NULL;
    ofstream updater_file;

    LoadFileInResource(FILE_ID, FILE_TYPE_ID, size, data);

    updater_file.open(DATAFIELD42_EXE, fstream::out|fstream::binary|fstream::trunc);
    if(!updater_file.is_open()){
        cout << "Error at open(): " << DATAFIELD42_EXE << endl;
        return(0);
    }
    updater_file.write(data, size);
    updater_file.close();
    return(1);
}

int main(int argc, char** argv){
    cout << "Welcome to DataField42, the automatic download tool for BF1942" << endl << endl;
    cout << "When you join a server and you dont have the map DataField42 will pop-up and try to download it for you.." << endl << endl;

    int success1 = patchClientExe();
    int success2 = addDataField42Exe();

    if(success1 && success2){
        cout << "DataField42 is installed correctly." << endl;
    }else{
        cout << "ERROR: DataField42 cannot be installed correctly!!" << endl;
    }

    cout << "Do you want to open Battlefield 1942?" << endl << endl;
    cout << "(Type 'yes' or 'no' and hit enter)" << endl;
    if(clientAwknowlage()){
        return openBF1942();
    }
    return 0;
}

