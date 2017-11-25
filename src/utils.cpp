#include <iostream>
#include <vector>

#include "Common.capnp.h"
#include "Command.capnp.h"
#include "Response.capnp.h"
#include <capnp/serialize.h>

extern std::vector<std::vector<int>> table;
extern vector<int> posX, posY;

void getClosestEnemy(vector<int> &d, vector<int> &posX, vector<int> &posY)
{
}
void getClosestSafeCell(vector<int> &d, vector<int> &posSafeX, vector<int> &posSafeY, int owner = 1)
{
    d = vector<int>(posX.size(), 1000000);
    posSafeX = vector<int>(posX.size(), 0);
    posSafeY = vector<int>(posX.size(), 0);

    for (int col = 0; col < table.size(); col++)
        for (int row = 0; row < table[col].size(); row++)
            for (int numUnit = 0; numUnit < posX.size(); numUnit++)
            {
                if (table[col][row] == owner)
                {
                    int dist = abs(col - posX[numUnit]) + abs(row - posY[numUnit]);
                    if (d[numUnit] > dist)
                    {
                        d[numUnit] = dist;
                        posSafeX[numUnit] = col;
                        posSafeY[numUnit] = row;
                    }
                }
            }
}

bool moveUnit(int sockfd, uint numOfSteps, Direction dir)
{
    for (uint stepIndex = 0; stepIndex < numOfSteps; stepIndex++)
    {
        sendCommand(sockfd, 0, dir);
        uint prevHealth = currentHealth;
        receiveResponse(sockfd);
        if (currentHealth != prevHealth)
        {
            return false;
        }
    }
    return true;
}

void goToUpperLeftCorner(int sockfd, int teamClassNumber)
{
    int minCol = 1000;
    int minRow = 1000;

    for (int row = 0; row < table.size(); row++)
    {
        for (int col = 0; col < table[row].size(); col++)
        {
            if (table[row][col] == 0)
            {
                if (col + row <= minCol + minRow)
                {
                    minCol = col;
                    minRow = row;
                }
            }
        }
    }

    
    while (posX[0] < minRow - 1)
    {
        sendCommand(sockfd, teamClassNumber, Direction::DOWN);
        receiveResponse(sockfd);
    }
    while (posY[0] < minCol - 1)
    {
        sendCommand(sockfd, teamClassNumber, Direction::RIGHT);
        receiveResponse(sockfd);
    }
}
