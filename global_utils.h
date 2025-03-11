#pragma once

#ifndef GLOBAL_UTILS_H_INCLUDED
#define GLOBAL_UTILS_H_INCLUDED

#include <iostream>
#include <windows.h>
#include <filesystem>
#include <string>
#include <tlhelp32.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <type_traits>
#include <shlobj.h>
#include <limits>
#include <utility>


namespace utils {
    extern const HANDLE hConsole;

    //File system handle
    namespace fs = std::filesystem;

//Data structures

struct menu;
struct user;

//Console text colors
extern const int COL_BKONBK;
extern const int COL_BLUE;
extern const int COL_GREEN;
extern const int COL_TEAL;
extern const int COL_RED;
extern const int COL_PURPLE;
extern const int COL_ORANGE;
extern const int COL_WHITE;
extern const int COL_GREY;
extern const int COL_LBLUE;
extern const int COL_LIME;
extern const int COL_CYAN;
extern const int COL_PINK;
extern const int COL_MAGENTA;
extern const int COL_YELLOW;
extern const int COL_BRWHITE;
extern const int COL_BKONWH;
//Solid color blocks
extern const int COL_BLOCK_WHITE;
extern const int COL_BLOCK_BLUE;
extern const int COL_BLOCK_TEAL;
extern const int COL_BLOCK_GREEN;
extern const int COL_BLOCK_LIME;
extern const int COL_BLOCK_PURPLE;
extern const int COL_BLOCK_ORANGE;
extern const int COL_BLOCK_PINK;
extern const int COL_BLOCK_GREY;
extern const int COL_BLOCK_YELLOW;
extern const int COL_BLOCK_RED;

    //Getting input
    std::string getInps();
    int getInpn();

    //Handling output
    void prln(const std::string& text);
    void debugMsg(std::string data_t, int data_n);
    void printFilePath(const std::string program);
    void printOwnFilePath();
    void outputFilePath();

    //ASCII structures


    //Text or string manipulation
    std::string addQuotes(const std::string& text);
    std::vector<char> command_buffer(const std::string& command);
    void cin_clear();
    void br();
    bool stringIsNull(std::string& s);

    //Fetching process or file information
    std::string getInstallDirFromRegistry(const std::string& programName);
    std::string getInstallDir(bool include_exe);
    std::string getOwnInstallDir(bool include_exe);
    std::string getProcessName();
    std::string getOwnProcessName();
    bool isRunning(const std::string& processName);
    bool isRunning_tlhelp32(std::string processName);
    bool isAnotherInstanceRunning();

    //Handling lockfiles
    bool hasLockFile();
    void createLockFile();
    void removeLockFile();

    //Using Windows Terminal
    void createTerminal(const std::string& command_line);
    bool isTerminalAvailable();
    void setLaunchPlatform();
    void run_in_terminal();

    //Interacting with other programs
    void run_program(const std::string& filepath);

    //Random number generators
    int roll(int sides);
    int rollfz(int sides);
    bool rollfor(int targ, int sides);
    int rollfrom(int start, int sides);
    int rollcap(int start, int range, int cap);
    bool flip();
}

#endif // GLOBAL_UTILS_H_INCLUDED
