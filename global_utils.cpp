#include "global_utils.h"

using namespace std;

namespace utils{
const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

const string LOCKFILE = "tsf_lock.tmp";  // Lockfile to prevent infinite loops

//Console text colors
const int COL_BKONBK= 0;
const int COL_BLUE = 1;
const int COL_GREEN = 2;
const int COL_TEAL = 3;
const int COL_RED = 4;
const int COL_PURPLE = 5;
const int COL_ORANGE = 6;
const int COL_WHITE = 7;
const int COL_GREY = 8;
const int COL_LBLUE = 9;
const int COL_LIME = 10;
const int COL_CYAN = 11;
const int COL_PINK = 12;
const int COL_MAGENTA = 13;
const int COL_YELLOW = 14;
const int COL_BRWHITE = 15;
const int COL_BKONWH = 240;
//Solid color blocks
const int COL_BLOCK_WHITE = 255;
const int COL_BLOCK_BLUE = 17;
const int COL_BLOCK_TEAL = 51;
const int COL_BLOCK_GREEN = 34;
const int COL_BLOCK_LIME= 170;
const int COL_BLOCK_PURPLE = 85;
const int COL_BLOCK_ORANGE = 102;
const int COL_BLOCK_PINK = 221;
const int COL_BLOCK_GREY = 136;
const int COL_BLOCK_YELLOW = 238;
const int COL_BLOCK_RED = 68;

//Data structures
struct menu
{
    string intro, prompt;
    int options_limit;
    vector<string> options;
    int input;

    void display()
    {
        cout<<intro;
        cout<<prompt;
        for(int x=1; x<options_limit;x++){cout<<" ["<<x<<"] \t|\t"<<options[x]<<"\n";}
        cout<<" ["<<options_limit<<"] \t|\tBack\n";
    }
};

struct user
{
    string name, password;
    int age, ID;

    bool getPassword()
    {
        prln("Please enter the user's password:");
        string p = getInps();
        if (p == password) return true;
        else return false;
    }

    void show_details()
    {
        prln("User Details:");
        cout << "\n Name: " << name << "\n";
        cout << "\n Age: " << name << "\n";
        cout << "\n ID: " << name << "\n";
    }

    int generateID(){return rollfrom(100000, 999999);}

    void set_details()
    {
        prln("New User - \n\nEnter your name:");
        name = getInps();
        prln("Enter your age:");
        age = getInpn();
        generateID();
        prln("New User Created.");
        show_details();
    }


};

//Getting input
string getInps() {string input = ""; cout << ">"; if (cin.peek() == '\n') cin_clear(); getline(cin, input); return input;}

int getInpn()
{
    int input = 0;
    cout << ">";
    while(!input)
    {
        prln("Input error; Please enter a number:");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //GPT
        cout << ">";
        cin >> input;
    }
    return input;
}

//Handling output
void prln(const std::string& text) {cout << "\n" << text << "\n";}

void debugMsg(std::string data_t, int data_n){cout << "\n" << data_t; if (data_n > -1) cout << ": " << data_n; cout << endl;}

void printOwnFilePath(){cout << "\nPath to current program: " << addQuotes(getOwnInstallDir(true)) << "\n";}

void printFilePath(const string program){cout << "\nPath to " << program << ": " << addQuotes(getInstallDirFromRegistry(program)) << "\n";}

void outputFilePath()
{
    string path = getOwnInstallDir(true);
    ofstream save;
    save.open("filepath.txt");
    save << path;
    save.close();
}


//Text and string manipulation
void cin_clear() {cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');}

string addQuotes(const std::string& text) {return "\"" + text + "\"";}

vector<char> command_buffer(const std::string& command)
{
    vector<char> buffer(command.begin(), command.end());
    buffer.push_back('\0');
    return buffer;
}

void br() { cout << "\n"; }

bool stringIsNull(std::string& s) { return (s.empty() || s == "0" || s == "none" || s == "null" || s == "NULL"); }

//Random number generators
int roll(int sides) { return rand()% sides + 1; }

bool rollfor(int targ, int sides) { if ( rollfz(sides) <= targ) return true; else return false; }

int rollfrom(int start, int range) { return rand() % range + start; }

int rollfz(int sides){ return rand()% sides; }

bool flip() { return rollfor(1, 2); }

int rollcap(int start, int range, int cap) { int res = rand() % range + start; if ( res > cap ) return cap; else return res; }


//Handling lockfiles
bool hasLockFile() {
    ifstream file(LOCKFILE);
    return file.good();  // Returns true if the file exists
}

void createLockFile() {
    ofstream file(LOCKFILE);
    file << "LOCKED";
    file.close();
}

void removeLockFile() {remove(LOCKFILE.c_str());}

//Fetching process or file information
string getOwnInstallDir(bool include_exe)
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    string fullPath(path);
    size_t last_slash = fullPath.find_last_of("\\/");
    return include_exe ? fullPath : fullPath.substr(0, last_slash);
}

string getInstallDir(const string& exeName) {
    char path[MAX_PATH];

    // Search for the program in the system's PATH
    if (SearchPath(NULL, exeName.c_str(), ".exe", MAX_PATH, path, NULL)) {
        string fullPath(path);
        size_t last_slash = fullPath.find_last_of("\\/");
        return fullPath.substr(0, last_slash);  // Return only directory
    }

    return "Not Found";
}

string getInstallDirFromRegistry(const std::string& programName) {
    HKEY hKey;
    vector<string> registryPaths = {
        // Standard uninstall registry keys
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        "SOFTWARE\\Valve\\Steam\\Apps" // Steam game installs
    };

    for (const auto& regPath : registryPaths) {
        HKEY baseKey = HKEY_LOCAL_MACHINE;
        if (RegOpenKeyEx(baseKey, regPath.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS) {
            char subKeyName[256];
            DWORD subKeySize = sizeof(subKeyName);
            DWORD index = 0;

            while (RegEnumKeyEx(hKey, index, subKeyName, &subKeySize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
                HKEY programKey;
                string fullPath = regPath + "\\" + subKeyName;
                debugMsg("Checking " + fullPath, -1);

                if (RegOpenKeyEx(baseKey, fullPath.c_str(), 0, KEY_READ | KEY_WOW64_64KEY, &programKey) == ERROR_SUCCESS) {
                    char displayName[256], installDir[1024];
                    DWORD nameSize = sizeof(displayName);
                    DWORD dirSize = sizeof(installDir);

                    if (RegQueryValueEx(programKey, "DisplayName", NULL, NULL, (LPBYTE)displayName, &nameSize) == ERROR_SUCCESS &&
                        programName == displayName) {
                        if (RegQueryValueEx(programKey, "InstallLocation", NULL, NULL, (LPBYTE)installDir, &dirSize) == ERROR_SUCCESS) {
                            RegCloseKey(programKey);
                            RegCloseKey(hKey);
                            return string(installDir);
                        }
                    }
                    RegCloseKey(programKey);
                }
                subKeySize = sizeof(subKeyName);
                index++;
            }
            RegCloseKey(hKey);
        }
    }

    // Check common system locations for system apps like notepad.exe, cmd.exe
    vector<string> systemPaths = {
        "C:\\Windows\\System32\\",
        "C:\\Windows\\",
        "C:\\Windows\\SysWOW64\\"
    };

    for (const auto& path : systemPaths) {
        string fullPath = path + programName;
        if (GetFileAttributes(fullPath.c_str()) != INVALID_FILE_ATTRIBUTES) {return fullPath;}
    }

    return "File Not Found"; // Not found
}

string getOwnProcessName()
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    string fullPath(path);
    size_t last_slash = fullPath.find_last_of("\\/");
    return fullPath.substr(last_slash + 1);
}

bool isRunning(const string& processName)
{
    ostringstream command;
    command << "tasklist /FI \"IMAGENAME eq " << processName << "\"";
    string cmdOutput = command.str();

    FILE* pipe = _popen(cmdOutput.c_str(), "r");
    if(!pipe) return false;

    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {result += buffer;}
    _pclose(pipe);

    return result.find(processName) != std::string::npos;
}

bool isRunning_tlhelp32 (string processName)
{
    DWORD myPID = GetCurrentProcessId();
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    bool is_found = 0;
    if (hSnap == INVALID_HANDLE_VALUE) {cout << "Error: could not create snapshot.\n"; return false;}

    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32);

    //Search for existing TSF windows
    if (Process32First(hSnap, &procEntry))
    {
        do{
            if (processName == procEntry.szExeFile) {
                //Ignore current instance
                if (procEntry.th32ProcessID != myPID){ CloseHandle(hSnap); is_found = true; } //Second instance found
            }
        } while (Process32Next(hSnap, &procEntry));
    }

    //Did not find second window
    CloseHandle(hSnap);
    return is_found;
}

bool isAnotherInstanceRunning()
{
    HANDLE hMutex = CreateMutex(NULL, TRUE, "Global\\SelfLaunchMutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hMutex);
        return true; // Another instance is running
    }
    return false;
}


//Using Windows Terminal
void createTerminal(const std::string& command_line)
{
    if (hasLockFile()) {
        cout << "Instance already launched, exiting...\n";
        removeLockFile();
        return;  // Prevent recursion
    }

    createLockFile();  // Mark that an instance has been launched

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    string terminalCmd = "wt -d . -- " + command_line;
    vector<char> cmdBuffer = command_buffer(terminalCmd);

    FreeConsole();
    if (CreateProcess(NULL, cmdBuffer.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        debugMsg("Terminal launched successfully", 1);

        // Wait for the new process to initialize (prevents instant closure)
        WaitForInputIdle(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        ExitProcess(0);
    }
    else cerr << "Failed to run Terminal.\n Error: " << GetLastError() << endl;
}

bool isTerminalAvailable() { return (system("where wt >nul 2>nul") == 0); }

void setLaunchPlatform()
{
    string command = addQuotes(getOwnInstallDir(true) );
    if (!isAnotherInstanceRunning() && isTerminalAvailable() ) createTerminal(command);
}

void run_in_terminal() {cout << "Attempting to launch self in Windows Terminal...\n"; setLaunchPlatform();}


//Interacting with other programs
void run_program(const std::string& filepath)
{
    string command = "cmd start " + filepath;
    system(command.c_str() );
}

} //End of namespace utils
