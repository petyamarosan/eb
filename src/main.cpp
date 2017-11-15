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


int main(int argc, char *argv[])
{
    // Set up client
    int sockfd = setUpClient();

    // Send login request
    sendLoginRequest(sockfd);
    receiveResponse(sockfd);

    int margin = 9;

    vector<int>  d, posSafeX, posSafeY;
    // RIGHT
    for (int i = 0; i < margin; i++)
    {
        sendCommand(sockfd, 0, Direction::DOWN);
        receiveResponse(sockfd);
        getClosestSafeCell(d, posSafeX, posSafeY);
        cout<<"d: "<<d[0]<<"x: "<<posSafeX[0]<<"y: "<<posSafeY[0]<<endl;
    }
    sendCommand(sockfd, 0, Direction::RIGHT);
    receiveResponse(sockfd);
    sendCommand(sockfd, 0, Direction::RIGHT);
    receiveResponse(sockfd);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::RIGHT);
        receiveResponse(sockfd);
        getClosestSafeCell(d, posSafeX, posSafeY);
        cout<<"d: "<<d[0]<<"x: "<<posSafeX[0]<<"y: "<<posSafeY[0]<<endl;
    }
   
    // DOWN
    for (int i = 0; i < margin; i++)
    {
        sendCommand(sockfd, 0, Direction::LEFT);
        receiveResponse(sockfd);
    }
    sendCommand(sockfd, 0, Direction::DOWN);
    receiveResponse(sockfd);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::DOWN);
        receiveResponse(sockfd);
    }

    // LEFT 
    for (int i = 0; i < 2.2*margin; i++)
    {
        sendCommand(sockfd, 0, Direction::UP);
        receiveResponse(sockfd);
    }
    sendCommand(sockfd, 0, Direction::LEFT);
    receiveResponse(sockfd);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::LEFT);
        receiveResponse(sockfd);
    }


    // UP 
    for (int i = 0; i < 2.5*margin; i++)
    {
        sendCommand(sockfd, 0, Direction::RIGHT);
        receiveResponse(sockfd);
    }
    sendCommand(sockfd, 0, Direction::UP);
    receiveResponse(sockfd);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::UP);
        receiveResponse(sockfd);
    }







// RIGHT
margin = 6;
    for (int i = 0; i < 2*margin; i++)
    {
        sendCommand(sockfd, 0, Direction::DOWN);
        receiveResponse(sockfd);
    }
    sendCommand(sockfd, 0, Direction::RIGHT);
    receiveResponse(sockfd);
    sendCommand(sockfd, 0, Direction::RIGHT);
    receiveResponse(sockfd);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::RIGHT);
        receiveResponse(sockfd);
    }
   
    // DOWN
    for (int i = 0; i < margin; i++)
    {
        sendCommand(sockfd, 0, Direction::LEFT);
        receiveResponse(sockfd);
    }
    sendCommand(sockfd, 0, Direction::DOWN);
    receiveResponse(sockfd);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::DOWN);
        receiveResponse(sockfd);
    }

    // LEFT 
    for (int i = 0; i < margin-2; i++)
    {
        sendCommand(sockfd, 0, Direction::UP);
        receiveResponse(sockfd);
    }
    sendCommand(sockfd, 0, Direction::LEFT);
    receiveResponse(sockfd);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::LEFT);
        receiveResponse(sockfd);
    }


    for (int i = 0; i < 33;i++){
        sendCommand(sockfd, 0, Direction::LEFT);
        receiveResponse(sockfd);
        sendCommand(sockfd, 0, Direction::RIGHT);
        receiveResponse(sockfd);
    }

    // UP 
    for (int i = 0; i < margin; i++)
    {
        sendCommand(sockfd, 0, Direction::RIGHT);
        receiveResponse(sockfd, true);
    }
    sendCommand(sockfd, 0, Direction::UP);
    receiveResponse(sockfd, true);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::UP);
        receiveResponse(sockfd, true);
    }










// RIGHT
margin =4;
    for (int i = 0; i < margin; i++)
    {
        sendCommand(sockfd, 0, Direction::DOWN);
        receiveResponse(sockfd, true);
    }
    sendCommand(sockfd, 0, Direction::RIGHT);
    receiveResponse(sockfd, true);
    sendCommand(sockfd, 0, Direction::RIGHT);
    receiveResponse(sockfd, true);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::RIGHT);
        receiveResponse(sockfd, true);
    }
   
    // DOWN
    for (int i = 0; i < margin; i++)
    {
        sendCommand(sockfd, 0, Direction::LEFT);
        receiveResponse(sockfd, true);
    }
    sendCommand(sockfd, 0, Direction::DOWN);
    receiveResponse(sockfd, true);

    while (table[posX[0]][posY[0]] != 1){
        sendCommand(sockfd, 0, Direction::DOWN);
        receiveResponse(sockfd, true);
    }





    /*
   int i = 0;
    while (i < 1)
    {
        cout << "iter: " << i << endl;
        sendCommand(sockfd, 0, Direction::DOWN);
        table = receiveResponse(sockfd);
        //sendCommand(sockfd, 0, Direction::RIGHT);
        //receiveResponse(sockfd);

        i++;
    }
*/
    // Close connection
    close(sockfd);
    return 0;
}