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

void totyogas(int sockfd, int teamClassNumber, uint stateIndex,
              int playgroundUpperIndex, int playgroundLowerIndex,
              int playgroundLeftIndex, int playgroundRightIndex)
{
    bool totyogjunke = true;
    int stepLimit = 1000;

    while (totyogjunke)
    {
        stepLimit = 1000;
        // stateIndex % 2 = 0 -> vizszintesen megyunk -> x-et!!! kell vizsgalni
        if (stateIndex % 2 == 1)
        {
            for (int enemyIndex = 0; enemyIndex < enemyDirX.size(); ++enemyIndex)
            {
                //cout << "posX: " << posX[0] << " posY: " << posY[0] << endl;
                //cout << "enemyPosX: " << enemyPosX[enemyIndex] << " enemyPosY: " << enemyPosY[enemyIndex] << endl;
                //cout << "enemyDirX: " << enemyDirX[enemyIndex] << " enemyDirY: " << enemyDirY[enemyIndex] << endl;

                if (posX[0] < enemyPosX[enemyIndex])
                {
                    if (enemyDirX[enemyIndex] == -1)
                    {
                        int temp = abs(enemyPosX[enemyIndex] - posX[0]);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                    else
                    {
                        int temp = abs(enemyPosX[enemyIndex] - posX[0]) +
                                2 * abs(enemyPosX[enemyIndex] - playgroundLowerIndex);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                }
                else if (posX[0] > enemyPosX[enemyIndex])
                {    
                    if (enemyDirX[enemyIndex] == 1)
                    {
                        int temp = abs(enemyPosX[enemyIndex] - posX[0]);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                    else
                    {
                        int temp = abs(enemyPosX[enemyIndex] - posX[0]) +
                                2 * abs(enemyPosX[enemyIndex] - playgroundUpperIndex);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                }
                else
                {
                    //TODO
                    stepLimit = 0;
                }
            }

            totyogjunke = (stepLimit < playgroundRightIndex - playgroundLeftIndex + 1);
        }
        else
        {
            for (int enemyIndex = 0; enemyIndex < enemyDirY.size(); ++enemyIndex)
            {
                if (posY[0] < enemyPosY[enemyIndex])
                {
                    if (enemyDirY[enemyIndex] == -1)
                    {
                        int temp = abs(enemyPosY[enemyIndex] - posY[0]);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                    else
                    {
                        int temp = abs(enemyPosY[enemyIndex] - posY[0]) +
                                2 * abs(enemyPosY[enemyIndex] - playgroundRightIndex);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                }
                else if (posY[0] > enemyPosY[enemyIndex])
                {
                    if (enemyDirY[enemyIndex] == 1)
                    {
                        int temp = abs(enemyPosY[enemyIndex] - posY[0]);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                    else
                    {
                        int temp = abs(enemyPosY[enemyIndex] - posY[0]) +
                                2 * abs(enemyPosY[enemyIndex] - playgroundLeftIndex);
                        if (temp < stepLimit)
                        {
                            stepLimit = temp;
                        }
                    }
                }
                else
                {
                    //TODO
                    stepLimit = 0;
                }
            }

            totyogjunke = (stepLimit < playgroundLowerIndex - playgroundUpperIndex + 1);
        }
    
        cout << "steplimit: " << stepLimit << endl;

        if (totyogjunke)
        {
            sendCommand(sockfd, teamClassNumber, Direction::RIGHT);
            receiveResponse(sockfd);
            sendCommand(sockfd, teamClassNumber, Direction::LEFT);
            receiveResponse(sockfd);
        }
    }    
}