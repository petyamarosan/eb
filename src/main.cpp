#include <unistd.h>
#include <netdb.h>

#include <iostream>
#include <vector>

#include "Common.capnp.h"
#include "Command.capnp.h"
#include "Response.capnp.h"
#include <capnp/serialize.h>

#include "ServerClientFunctions.cpp"
#include "utils.cpp"

using namespace std;
using namespace ericsson2017::protocol::semifinal;

extern std::vector<std::vector<int>> table;
extern vector<int> posX, posY;
extern uint currentHealth;


int main(int argc, char *argv[])
{
    // Set up client
    int sockfd = setUpClient();
    int state = 0;
    int margin = 20;
    vector<vector<int>> LUT = {{3, 2}, {0, 3}, {1, 0}, {2, 1}};
    int playgroundUpperIndex = 2;
    int playgroundLowerIndex = 77;
    int playgroundLeftIndex = 2;
    int playgroundRightIndex = 97;
    vector<int> playgroundSize{playgroundLowerIndex - playgroundUpperIndex + 1,
                               playgroundRightIndex - playgroundLeftIndex + 1};

    // Send login request
    sendLoginRequest(sockfd);
    receiveResponse(sockfd);

    while (true)
    {
        goToUpperLeftCorner(sockfd, 0);
        
        for (uint stateIndex = 0; stateIndex < 4; stateIndex++)
        {
            moveUnit(sockfd, margin, static_cast<Direction>(LUT[stateIndex][0]));

            totyogas(sockfd, 0, stateIndex, playgroundUpperIndex, playgroundLowerIndex,
            playgroundLeftIndex, playgroundRightIndex);

            if (moveUnit(sockfd, playgroundSize[stateIndex % 2] + 1, static_cast<Direction>(LUT[stateIndex][1])))
            {
                switch(stateIndex)
                {
                    case 0:
                        playgroundLeftIndex += margin;
                        break;
                    case 1:
                        playgroundLowerIndex -= margin;
                        break;
                    case 2:
                        playgroundRightIndex -= margin;
                        break;
                    case 3:
                        playgroundUpperIndex += margin;
                        break;
                }
                playgroundSize = vector<int>{playgroundLowerIndex - playgroundUpperIndex + 1,
                               playgroundRightIndex - playgroundLeftIndex + 1};

                margin = max(5, margin - 1);
            }
            else
            {
                break;
            }
            if (newLevel)
            {
                cout << "newlevel!!!" << endl;
                newLevel = false;
                playgroundUpperIndex = 2;
                playgroundLowerIndex = 77;
                playgroundLeftIndex = 2;
                playgroundRightIndex = 97;
                playgroundSize = vector<int>{playgroundLowerIndex - playgroundUpperIndex + 1,
                               playgroundRightIndex - playgroundLeftIndex + 1};
                margin = 20;
                state = 0;
                break;
            }
        }
    }

    // Close connection
    close(sockfd);
    return 0;
}
