#include <unistd.h>
#include <netdb.h>

#include <iostream>
#include <vector>

#include "Common.capnp.h"
#include "Command.capnp.h"
#include "Response.capnp.h"
#include <capnp/serialize.h>
#include <capnp/serialize-packed.h>

using namespace std;
using namespace ericsson2017::protocol::semifinal;

std::vector<std::vector<int>> table(80, std::vector<int>(100, 0));
vector<int> posX, posY;
uint currentHealth;
/*
 * Send a login request to the server
 * fd - socket's file descriptor
 * hash - team's hash code
 * team - team name
*/
void sendLoginRequest(int fd, ::capnp::Text::Reader hash = "r8h9nqvoio0ugpwr51fz02i7fbgaixzz6", ::capnp::Text::Reader team = "okostukortojasok")
{
    // Init message
    ::capnp::MallocMessageBuilder message;
    // Init request capnp schema, connect to message
    Command::Builder command = message.initRoot<Command>();
    // Set request
    command.initCommands().initLogin();
    command.getCommands().getLogin().setHash(hash);
    command.getCommands().getLogin().setTeam(team);
    // Print request
    cout << endl
         << "Request details: \n\t"
         << "Hash: " << command.getCommands().getLogin().getHash().cStr() << "\n\t"
         << "Team: " << command.getCommands().getLogin().getTeam().cStr() << endl;
    // Send message
    writePackedMessageToFd(fd, message);
}

void sendCommand(int fd, int32_t unit, Direction dir)
{
    // Init message
    ::capnp::MallocMessageBuilder message;
    // Init request capnp schema, connect to message
    Command::Builder command = message.initRoot<Command>();
    // Set request
    ::capnp::List<Move>::Builder moves = command.initCommands().initMoves(1);
    moves[0].setUnit(unit);
    moves[0].setDirection(dir);

    // Print request
    //cout<<endl<<"Request details: \n\t"<<"Hash: "<<moves[0].getU.cStr()<<"\n\t"<<"Team: "<<command.getCommands().getLogin().getTeam().cStr()<<endl;
    // Send message
    writePackedMessageToFd(fd, message);
}
/*
 * Receive response
 * numBugs - number of actual bugs
 * exit_ - end of the game or not
 */
void receiveResponse(int fd, bool disp=true)
{
    posX.clear();
    posY.clear();
    // Init message

    // Get message from socket
    capnp::PackedFdMessageReader message(fd);
    // Get response
    Response::Reader response = message.getRoot<Response>();

    // Print Status
    if (disp)
    cout << "Response details:\n\tStatus: " << response.getStatus().cStr();

    Response::Info::Reader info = response.getInfo();
    if (disp)
    cout << "\n \tOwns: " << info.getOwns() << "\n \tLevel: " << info.getLevel() << "\n \tTick: " << info.getTick();

    for (Unit::Reader unit : response.getUnits())
    {
        if (disp)
        cout << "\n \tUnit: "
             << "\n \t\tPos: [" << unit.getPosition().getX() << ", " << unit.getPosition().getY() << "]" << endl;
        if (disp)
        cout << "\t\tDirection: ";
        switch (unit.getDirection())
        {
        case Direction::UP:
        if (disp)
            cout << "UP";
            break;
        case Direction::DOWN:
        if (disp)
            cout << "DOWN";
            break;
        case Direction::LEFT:
        if (disp)
            cout << "LEFT";
            break;
        case Direction::RIGHT:
        if (disp)
            cout << "RIGHT";
            break;
        default:
            cout << "Unknown direction!";
        }
        if (disp){
        cout << endl;
        cout << "\t\tHealth: " << unit.getHealth();
        currentHealth = unit.getHealth();
        cout << "\n\t\tKiller: " << unit.getKiller();
        cout << "\n\t\tOwner: " << unit.getOwner();}
        posX.push_back(unit.getPosition().getX());
        posY.push_back(unit.getPosition().getY());
    }

    vector<int> colE;
    vector<int> rowE;

    for (Enemy::Reader enemy : response.getEnemies())
    {
        if (disp)
        cout << "\n \tEnemies: "
             << "\n \t\tPos: [" << enemy.getPosition().getX() << ", " << enemy.getPosition().getY() << "]" << endl;
             if (disp)
        cout << "\t\tDirection: [";
        switch (enemy.getDirection().getVertical())
        {
        case Direction::UP:
        if (disp)
            cout << "UP";
            break;
        case Direction::DOWN:
        if (disp)
            cout << "DOWN";
            break;
        default:
            cout << "Unknown direction!";
        }
        if (disp)
        cout << ", ";
        switch (enemy.getDirection().getHorizontal())
        {
        case Direction::LEFT:
        if (disp)
            cout << "LEFT";
            break;
        case Direction::RIGHT:
        if (disp)
            cout << "RIGHT";
            break;
        default:
            cout << "Unknown direction!";
        }
    if (disp)
            cout << "]" << endl;

            colE.push_back(enemy.getPosition().getX());
            rowE.push_back(enemy.getPosition().getY());
        }
        int i = 0;
        int j = 0;

        
        for (capnp::List<Cell>::Reader cellList : response.getCells())
        {
            j = 0;
            for (Cell::Reader cell : cellList)
            {
                table[i][j] = cell.getOwner();
                if(disp){
                    bool enemyOrUnit = false;
                    for(int indU = 0;indU<posY.size();indU++)
                    {
                         if (j == posY[indU] && i == posX[indU])
                         {
                            cout << "* ";
                            enemyOrUnit = true;
                            break;
                         }
                           
                    }
                    for(int indE = 0;indE<rowE.size();indE++)
                    {
                        if (j == rowE[indE] && i == colE[indE])
                        {
                            cout << "  ";
                            enemyOrUnit = true;
                            break;
                        }
                    }
                    if(!enemyOrUnit)
                        cout << cell.getOwner() << " ";
                }
                j++;
            }
            if(disp)
                cout << endl;
            i++;
        }
    if(disp)
    cout << endl;
}

/*
 * Set up client
 * hostName - hostname
 * portno - port number
 * return socket fd
 */
int setUpClient(const char *hostName = "epb2017.dyndns.org", int portno = 11224)
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        runtime_error("Cannot open socket!\n");

    server = gethostbyname(hostName);
    if (server == NULL)
        runtime_error("No such host\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        runtime_error("Cannot connecting!\n");
    return sockfd;
}