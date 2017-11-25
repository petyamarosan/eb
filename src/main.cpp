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
    int margin = 9;
    vector<vector<int>> LUT = {{2, 3}, {1, 2}, {0, 1}, {3, 0}};
    vector<int> tableSize{76, 96};

    // Send login request
    sendLoginRequest(sockfd);
    receiveResponse(sockfd);

    while (true)
    {
        goToUpperLeftCorner(sockfd, 0);
        
        for (uint stateIndex = 0; stateIndex < 4; stateIndex++)
        {
            moveUnit(sockfd, margin, static_cast<Direction>(LUT[stateIndex][0]));
            if (moveUnit(sockfd, tableSize[(stateIndex + 1) % 2] + 1, static_cast<Direction>(LUT[stateIndex][1])))
            {
                tableSize[(stateIndex) % 2] = tableSize[(stateIndex) % 2] - margin;
            }
            else
            {
                break;
            }
        }
    }

    // Close connection
    close(sockfd);
    return 0;
}
