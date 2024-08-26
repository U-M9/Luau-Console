#pragma warning(disable : 4996)
#include "lua.h"
#include "lualib.h"
#include "Luau/Compiler.h"
#include "Luau/Common.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

void reportError(lua_State* L) {
    const char* message = lua_tostring(L, -1);
    if (message) {
        std::cout << "Error: " << message << std::endl;
        lua_pop(L, 1);
    }
}

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", std::localtime(&now));
    return std::string(buf);
}

int main() {
    lua_State* L = luaL_newstate();

    luaL_openlibs(L);

    while (true) {
        std::cout << "Enter Luau script (or type 'exit' to quit):" << std::endl;

        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "exit") {
            break;
        }

        std::string bytecode = Luau::compile(userInput);

        if (!bytecode.empty()) {
            int loadStatus = luau_load(L, "script", bytecode.data(), bytecode.size(), 0);

            if (loadStatus == LUA_OK) {
                if (lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK) {
                    reportError(L);
                }
                else {
                    std::string timestamp = getCurrentTimestamp();
                    std::string filename = timestamp + ".luau";

                    std::ofstream outFile(filename, std::ios::binary);
                    if (outFile) {
                        outFile.write(bytecode.data(), bytecode.size());
                        outFile.close();
                        std::cout << "Bytecode saved to " << filename << std::endl;
                    }
                    else {
                        std::cout << "Error: Could not save bytecode to file." << std::endl;
                    }
                }
            }
            else {
                reportError(L);
            }
        }
        else {
            std::cout << "Error: Compilation failed." << std::endl;
        }
    }

    lua_close(L);

    return 0;
}
