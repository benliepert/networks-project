# networks-project
High level workflow w/ implementation questions:

1: clients connect to server
    How do we connect to the server?
    What is the message format that we will send?
    Conditions for sending?
    How do we implement the server waiting for a message?
    How will we implement our GUI with it?
2: clients send info to server
    What do we do after we send?
    How will we implement “waiting” for a reply?
    What does the server do with the message? 
3: server “echos” info back to clients 
    How do we send the message to the other clients connected to server?
    Error codes? Connection issues?
