import { WebSocketServer, WebSocket } from 'ws';
import { randomUUID } from 'crypto';


/**
 * WebSocket communication primarily revolves around exchanging messages between the client and server.
 * @param {WebSocket} WebSocket - WebSocket connection object 
 * @param {WebSocket} wss - websocket server
 */
const wss = new WebSocketServer({port: 8080});
const clients = new Map();
let clientCount = [
  { ClientID: 1, flag: false },
  { ClientID: 2, flag: false },   
  { ClientID: 3, flag: false },
  { ClientID: 4, flag: false },
  { ClientID: 5, flag: false },
];

/**
 * event listener connection
 * This code block will be executed each time a client establishes a WebSocket connection with the server.
 * Code to handle new webSocket connections 
 */
wss.on('connection', (ws) => {

    // const id = randomUUID(); // i would like a different way to make an id. maybe create a function call
    const id = generateClientID();

    if (id === false) {
        ws.send(`Server is full, please try again later :(`);
        ws.close();
        return;
    }
    clients.set(ws, id);
    console.log(`newly connected client assigned id: ${id}`);

    ws.isAlive = true;

    ws.on('pong', () => {
        ws.isAlive = true; // reset on pong
    });

    //sends intial data to the client with welcome and new id 
    ws.send(`Welcome to the WebSocket server! You have been assigned id: ${id}`);
    console.log(`New Client connected to server. New Client id: ${id}`);

    /**
    * stop tracking the client upon that client closing the connectio
     * event listener to handle disconnections 
     * This event will be triggered when a WebSocket connection is closed by the client or due to some error. 
     * You can use this event to clean up resources associated with the client or update the list of connected clients.
     */
    ws.on('close', () => {

        const clientID = parseInt(id.replace('Player ', ''));
        removeClientID(clientID);
        console.log(`connection (id = ${id}) closed`);
        // console.log(`connection (id = ${clients.get(ws)}) closed`);
        
        clients.delete(ws);
        serverBroadcast(`Client ${id} has disconnected`);
    });

    /**
     * defined the logic to handle incoming messages from clients. 
     * This code block will be executed whenever the server receives a WebSocket message from a connected client.
     * event listener message
     * Code to handle incoming messages
     * We can perform custom logic based on the received message, such as:
     *  updating the application state, broadcasting the message to other connected clients, or triggering specific actions.
     */
    ws.on('message', (message) => {

        console.log(`Received data from ${clients.get(ws)}: ${message}`);
        // reply to message received with echo
        ws.send(`Your message ${message}`); 

        // message handling 
        if (message == "test") {
            ws.send(`test has been received.`);
        } else if (message == "bye") {
            ws.send(`Goodbye, you have been removed from the websocket server.`);
            ws.close();
        } else {
            ws.send(`unknown message`);
        }
        
        
        // Broadcasting a message to all connected clients 
        serverBroadcast(`Client ${clients.get(ws)} ${message}`);
    });

    // Track connected clients 
});

setInterval(() => {
    console.log(`Number of connected clients in server: ${clients.size}`);
    serverBroadcast(`Number of connected clients in server: ${clients.size}`);
}, 5000); // value is interval at which broadcast to server is sent.

const interval = setInterval(() => {
    wss.clients.forEach((ws) => {
        if (ws.isAlive === false) {
            console.log(`Client timed out. Terminating...`);
            return ws.terminate();
        }

        ws.isAlive = false;
        ws.ping(); // triggers 'pong' if client is alive
    });
}, 10000); // every 10 seconds

// function for sending a message to every connected client
function serverBroadcast(message) {
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(message);
        }
    });
}

function generateClientID() {

    for (let i = 0; i < clientCount.length; i++) {
        if (clientCount[i].flag === false) {
            clientCount[i].flag = true;
            return `Player ${clientCount[i].ClientID}`;
        }
    }

    return false;
    
}

function removeClientID(clientID) {

    for (let i = 0; i < clientCount.length; i++) {
        if (clientCount[i].ClientID == clientID && clientCount[i].flag === true) {
            clientCount[i].flag = false;
            return;
        }
        
        continue;
    }

    return `Error, the clientId provided was not correct: ${clientID}`;
}

console.log('WebSocket server is running... now awaiting connections...');