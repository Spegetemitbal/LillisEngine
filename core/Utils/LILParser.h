//
// Created by Somed on 9/23/2025.
//

#ifndef LILPARSER_H
#define LILPARSER_H
#include <unordered_set>

enum LILDataType
{
    LILDAT_INT = 0,
      LILDAT_FLOAT,
      LILDAT_STRING,
      LILDAT_CHAR,
      LILDAT_BOOL
  };

typedef void( *LILDataInjector )(void* dat, LILDataType type);

//Can self reference.
struct InterpreterFramework
{
    std::unordered_set<std::string> requiredComponents;
    std::unordered_set<std::string> optionalComponents;
    LILDataType requiredPrimitiveType = LILDAT_INT;
    //If negative, will continue to check until next valid key type appears.
    int numRequiredPrimitives = 0;
    LILDataInjector injector = nullptr;
};

class LILParser {
public:
    static void ProcessLILDat(std::ifstream& lildat, std::unordered_map<std::string,InterpreterFramework>& interpreter);
private:

    static bool isValidKey(const std::string& key);

    static void ReadFile(std::ifstream& lildat, std::string& output);

    static void ProcessPrimitives(InterpreterFramework frm, std::string& input);
    static bool IsItemComplete(const InterpreterFramework& frm, const std::unordered_set<std::string> &currentItems, int completedPrimitives);
};



#endif //LILPARSER_H
