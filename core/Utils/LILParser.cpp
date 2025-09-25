//
// Created by Somed on 9/23/2025.
//

#include "LILParser.h"

#include <stack>

void LILParser::ProcessLILDat(std::ifstream &lildat, std::unordered_map<std::string, InterpreterFramework> &interpreter)
{
    //Procedure:
    /*
     * 1) Search for valid key (Ensure no duplicate key names)
     * 2) If key lacks any required primitives, bump required level (number of indents) for next keys, otherwise process
     * primitives, send data. <Required depth level does not apply
     * 3) Recur.
     * 4) If next key doesn't have the required level, if more- spit an error, if less, check if all required components
     * of the current item are fulfilled, if not, spit an error.
     * 5) Loop until EOF.
     */

    //Current depth = stack size. The top of the stack is the current key being scanned.
    std::stack<InterpreterFramework> interpreterStack;
    std::unordered_set<std::string> currentTokens;

    InterpreterFramework currentKey = {};
    std::string line;
    unsigned int currentLine = 0;
    int currentPrimitivesProcessed = 0;

    //TODO break down into smaller functions.

    if(lildat.is_open())
    {
        while(std::getline(lildat, line))
        {   // get a whole line
            currentLine++;

            //Get number of indents
            int numTabs = 0;
            while (line.front() == '\t')
            {
                line.erase(line.begin());
                numTabs++;
            }

            std::stringstream ss(line);
            while(getline(ss, line, ' '))
            {
                // You now have separate entities here
                if (isValidKey(line))
                {
                    char startChar = line.front();
                    line.erase(line.begin());
                    //Determine if item is contained in interpreter.
                    if (interpreter.contains(line))
                    {
                        //If interpreter stack isn't empty, ensure key is a component, and whether it's properly marked as a required or optional one.
                        if (!interpreterStack.empty())
                        {
                            //Valid key
                            if (numTabs <= interpreterStack.size())
                            {
                                if (numTabs < interpreterStack.size())
                                {
                                    //Attempt to drop a depth level, throw an error if mandatory
                                    if (!IsItemComplete(interpreterStack.top(), currentTokens, currentPrimitivesProcessed))
                                    {
                                        throw std::invalid_argument("LILParser::ProcessLILDat: Attempting to exit incomplete item at line " + std::to_string(currentLine));
                                    }
                                    for (const auto& it : interpreterStack.top().requiredComponents)
                                    {
                                        //All will exist here if previous check is successful.
                                        currentTokens.erase(it);
                                    }
                                    interpreterStack.pop();
                                } else
                                {
                                    //Attempting to work with a component.
                                    if (interpreterStack.top().requiredComponents.contains(line))
                                    {
                                        if (startChar != '>')
                                        {
                                            throw std::invalid_argument("LILParser::ProcessLILDat: Mandatory component " + line + "at line: " + std::to_string(currentLine) + " must be marked with '>'\n");
                                        }
                                        currentTokens.emplace(line);
                                    } else if (interpreterStack.top().optionalComponents.contains(line))
                                    {
                                        if (startChar != '<')
                                        {
                                            throw std::invalid_argument("LILParser::ProcessLILDat: Optional component " + line + "at line: " + std::to_string(currentLine) + " must be marked with '<'\n");
                                        }
                                    } else
                                    {
                                        throw std::invalid_argument("LILParser::ProcessLILDat: Invalid LILDAT component " + line + "at line: " + std::to_string(currentLine) + "\n");
                                    }
                                    interpreterStack.emplace(interpreter.at(line));
                                }
                            } else
                            {
                                throw std::invalid_argument("LILParser::ProcessLILDat: Too many indents on line " + std::to_string(currentLine));
                            }
                        } else
                        {
                            if (startChar != '>')
                            {
                                throw std::invalid_argument("LILParser::ProcessLILDat: Root item " + line + "at line: " + std::to_string(currentLine) + " must be marked with '>'\n");
                            }
                            interpreterStack.emplace(interpreter.at(line));
                        }
                    } else
                    {
                        throw std::invalid_argument("LILParser::ProcessLILDat: Invalid LILDAT key " + line + "at line: " + std::to_string(currentLine) + "\n");
                    }

                    //Successfully processed item, clear prims.
                    currentPrimitivesProcessed = 0;

                } else
                {
                    //Is primitive.
                    try
                    {
                        ProcessPrimitives(interpreterStack.top(), line);
                    } catch (...)
                    {
                        throw std::invalid_argument("LILParser::ProcessLILDat: Couldn't process primitive " + line + "at line: " + std::to_string(currentLine) + "\n");
                    }
                    currentPrimitivesProcessed++;
                }
            }
        }
    }

}

bool LILParser::isValidKey(const std::string& key)
{
    //> means either a root point or a mandatory component, < means an optional component with a default.
    //Single characters of either < or > do not count as keys.
    return key.size() > 1 && key[0] == '>' || key[0] == '<';
}

bool LILParser::IsItemComplete(const InterpreterFramework& frm, const std::unordered_set<std::string> &currentItems, int completedPrimitives)
{
    for (const auto& it : frm.requiredComponents)
    {
        if (!currentItems.contains(it))
        {
            return false;
        }
    }

    if (frm.numRequiredPrimitives > 0)
    {
        return completedPrimitives == frm.numRequiredPrimitives;
    }

    return true;
}

void LILParser::ProcessPrimitives(InterpreterFramework frm, std::string &input)
{
    if (frm.numRequiredPrimitives == 0)
    {
        throw std::invalid_argument("LILParser::ProcessPrimitive: Extraneous primitive: " + input + "\n");
    }

    std::stringstream ss(input);
    while(getline(ss, input, ','))
    {
        int i = 0;
        float f = 0.0f;
        bool b = false;
        void* ptr = nullptr;

        if (frm.injector == nullptr)
        {
            throw std::invalid_argument("LILParser::ProcessPrimitive: An Injector is null!\n");
        }

        switch (frm.requiredPrimitiveType)
        {
            case LILDAT_INT:
                //check if int
                    ptr = &i;
            try
            {
                i = std::stoi(input);
            } catch (...)
            {
                throw std::invalid_argument("LILParser::ProcessPrimitive: Invalid literal for int: " + input + "\n");
            }
            break;
            case LILDAT_FLOAT:
                //check if double.
                    ptr = &f;
            try
            {
                f = std::stof(input);
            } catch (...)
            {
                throw std::invalid_argument("LILParser::ProcessPrimitive: Invalid literal for float: " + input + "\n");
            }
            break;
            case LILDAT_BOOL:
                //check for literals.
                    ptr = &b;
            if (input == "true" || input == "1")
            {
                b = true;
            } else if (input == "false" || input == "0")
            {
                b = false;
            } else
            {
                throw std::invalid_argument("LILParser::ProcessPrimitive: Invalid literal for bool: " + input + "\n");
            }
            break;
            case LILDAT_STRING:
                //just toss it in immediately ya know.
                    ptr = &input;
            break;
            case LILDAT_CHAR:
                //throw if input is too large.
                    ptr = &input[0];
            if (input.size() > 1)
            {
                throw std::invalid_argument("LILParser::ProcessPrimitive: Invalid literal for char: " + input + "\n");
            }
            break;
        }

        frm.injector(ptr, frm.requiredPrimitiveType);
    }
}


