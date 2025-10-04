
# Websockets Console Chat App

Hi let me give u a little intro to who i am.

My name is Bradley im a 23 year old. Studying Computer Science my background is in C# so im no C++ wizard by any means. But i have a huge intrest in everything that has to do with low level programming so i used this project to learn more about Websockets , Multithreading , Syncronization ,and Deadlock prevention. 

# Learning about Websockets
Since i have little to no knowledge about Websockets in C++ ( and in general i dont know alot about it ) i decided to learn a little bit about them and why they are quite cool. i found someone explaining it this way “A websocket is basically a network protocol that provides full duplex communication over a single persistent TCP connection” at this point i got even more confused. Duplex communication ? single persistent TCP Connection?  after letting it sink in for a bit and searching more explanations i think i started to get it . Let me explain in case ur still confused about it. The best way to explain it is by telling u that it works the opposite way off how HTTP works . Because HTTP is a  request - response model meaning the Client sends a Request to a specific endpoint on the server and the server responds to that request.  for a websocket its different a websocket keeps a constant connection between the Client and the Server this means no refreshes are needed + its fast.

# Some rules i made for myself
I decided to only use the winsock.h i did not want to use libraries like Socket.IO since the purpose off this project is learning and that would kinda make it “Easier” in a way.   

# Baby steps
Since i have little to no experience with websockets and my understanding off them is still not great il start off with just a simple Client Server model where the server echo’s my message back. so i will leave the threading and deadlock situation to the side for now. 

# Creating the TCP Echo Application
Reading into the documentation off winsock i stumbled upon 

https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

after reading some info on what behaviour a server/client should have i got directed towards the Initialize winsock this section in the documentation told me about how all processes or DLL’s that call winsock functions must initialize the use off the windows Socket DLL. the provided a function WSAStartup that takes two parameters 1st one lets u tell what version u want i just went with 2,2 since thats what the docs did and i had no reason to do elseway. second parameter is an output parameter this one i thought was intresting and useless at the same time the require u to pass a WSADATA reference. i dont know what it is but i just dont like the fact off creating unitialized variables in C++ and i did not see any use for the variable (in  my case ).   

Creating the socket was not as bad as i thought it would be after a few hair scratches and a coffee or 2 right now binding and creating the socket is still in one function but i will probably split that up into their own functions 

i wont be covering start and stop functions since its straight forward what the functions do. with this the server side is done for the TCP Echo server example

following the examples form microsoft docs the client was very easy to setup.  because Initializing & creating the actual socket is identical to how we did it for the server so all what was left to do was create a message loop and handle Receiving and Sending messages