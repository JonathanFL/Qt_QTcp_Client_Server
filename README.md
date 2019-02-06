# Qt_QTcp_Client_Server
Storing JSON-formatted data via a TCP connection between 2 Raspberry Pi Zero W.

The client and server was developed for a 3rd semester project at IKT at Aarhus University. 
We created a drink machine and needed to store drinks in a secondary place.

The Client side can send and receive JSON data on the form:

{
    "Gin og tonic": {
        "ingredienser": {
            "Gin": 2,
            "Tonic": 14
        }
    },
    "Rum & Cola": {
        "ingredienser": {
            "Rum": 2,
            "Cola": 14
        }
    }
}

As long as the receiver has a file called drinks.json.
The client can add multiple objects to the drinks, and also remove them again.
The methods on client is used as a singleton as seen in mainwindow.cpp.

The server side creates one thread per connections, so multiple clients can use the same server.

The source code is free of usage and can be modified as one want to.

For further information contact to j.lydemann@gmail.com.
