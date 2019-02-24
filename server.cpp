// ConsoleApplication2Server.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"

#include <iostream>
#include <SFML/Network.hpp>
#include <vector>
#include <windows.h>

#include <fstream>
#include <chrono>
//#include <msx>

using std::vector;
using std::string;

const unsigned short PORT = 5000;
const std::string IPADDRESS("127.0.0.1");//change to suit your needs


std::string msgSend;
std::vector<std::vector<std::string>> data;
sf::Mutex globalMutex;
std::vector<sf::TcpSocket*> sockets;
sf::SocketSelector selector;
std::vector<std::string> msgs;



bool mode = 0;
bool quit = false;
#include <time.h>
using namespace std::chrono;
using std::cout;
using std::pair;

void printTable()
{
	cout << "|================================================================|  " << std::endl;
	cout << "|    Time       ID      Resource       Target        HashCode    |    " << std:: endl;
	cout << "|================================================================| " << std::endl;

	
		
	for (int i = 0; i < data.size(); i++)
	{
		cout << "|";
		for (int j = 0; j < data[i].size(); j++)
		{
			cout << data[i][j];
			for (int k = 0; k < 13 - data[i][j].length()-1; k++)
			{
				cout << " ";
			}
			cout << "|";

		}
		cout << std::endl;
	}
	cout << "|================================================================| " << std::endl;


}

void DoStuff(int n)
{
	
	static std::string oldMsg;
	while (!quit)
	{
		vector<string> log;

		auto start = std::chrono::high_resolution_clock::now();
		//sf::Packet packetSend;
		//globalMutex.lock();
		//packetSend << msgSend;
		//globalMutex.unlock();
		//socket.send(packetSend)	
		int rex = -1;
	
					std::string msg;
					sf::Packet packetReceive;
					int rev = -1;
					sockets[n]->receive(packetReceive);

					if ((packetReceive >> msg) && msgs[n] != msg && !msg.empty())
					{
						/*
						std::cout << "sending :"; 
						for (int j = 0; j < msg.length(); j++)
						{
							std::cout << msg[j];
							Sleep(50);


						}
						std::cout << std::endl;

						std::ofstream myfile;
						myfile.open("example.txt",std::ios::app);
						
						auto finish = std::chrono::high_resolution_clock::now();
						std::chrono::duration<double> elapsed = finish - start;
						log.push_back(std::to_string(elapsed.count()));
						
					
						myfile << elapsed.count() << " ";				
						myfile << " ";
						myfile << msg;

						


						int l= msg.find(":");
						int l2 = msg.find_last_of(":");

						log.push_back(msg.substr(0, l));
						log.push_back(msg.substr(l+1, l2-l-1));
						log.push_back(msg.substr(l2+1));

						if (msg.substr(l + 1, l2 - l - 1)== "exit")
						{
							quit = !quit;
							break;
						}


						myfile << "\n";
						myfile.close();
						std::string filec;

						double code=0;
						std::ifstream readfile("example.txt");
						if (readfile.is_open())
						{
							while (getline(readfile, filec))
							{
								for (int i = 0; i < filec.length(); i++)
								{
									code += int(filec.at(i));
								}
								//double temp = std::stod(filec);
								
							}
							readfile.close();
						}
						log.push_back(std::to_string(code));

						std::cout << std::endl;
						std::cout << "code is " << code << std::endl;
						*/
						for (int i = 0; i < sockets.size(); i++)
						{		
							if (i != n)
							{
								sf::Packet packetSend;
								globalMutex.lock();
								packetSend << msg;
								globalMutex.unlock();
								sockets[i]->send(packetSend);
							}

						}
						msgs[n] = msg;
						//data.push_back(log);
						//log.clear();
						//printTable();

					}

				
				

			
		
	}

}

void GetInput(void)
{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin, s);
	if (s == "exit")
		quit = true;
	globalMutex.lock();
	msgSend = s;
	globalMutex.unlock();
}

void Server(void)
{
	while (!quit)
	{
	  sf::TcpListener listener;
	  if (listener.listen(PORT) == sf::Socket::Done)
	  {
  
  	}
  	sf::TcpSocket *temp;
  	temp = new sf::TcpSocket();
  	listener.accept(*temp);
	
	  std::cout << "New client connected: " << temp->getRemoteAddress() << std::endl;
	  sockets.push_back(temp);
	  selector.add(*sockets.back());
	  msgs.push_back("");
	  sf::Thread* thread = 0;
	  thread = new sf::Thread(&DoStuff,sockets.size()-1);
	  thread->launch();
  }
}




int main()
{


	//sf::Thread* thread = 0;
	sf::Thread* listener = 0;

	//Server();

//	thread = new sf::Thread(&DoStuff);
	listener = new sf::Thread(&Server);

//	thread->launch();
	listener->launch();

	while (!quit)
	{
	//	GetInput();		
	}

	//if (thread)
	//{
		//thread->wait();
		//delete thread;
	//}

	if (listener)
	{
		listener->wait();
		delete listener;
	}
	int hai;
	cout << "bye";
	std::cin >> hai;
	return 0;
}

