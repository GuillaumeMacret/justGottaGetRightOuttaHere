package com.example.justgottagetrightouttahere_client.network;

import com.example.justgottagetrightouttahere_client.Constants.Constants;
import com.example.justgottagetrightouttahere_client.model.GameMessageHandler;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class TCPClient{
    /**Socket to bind to the server**/
    Socket clientSocket;
    /**The thread that will listen for incoming messages**/
    Thread listeningThread;

    /**
     * Connects the socket to default port and address given in Constants file
     */
    public TCPClient(){
        try {
            clientSocket =  new Socket(Constants.SRVER_ADDRESS,Constants.SERVER_PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Sends the given string over the socket
     * @param s
     */
    public void send(String s){
        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(clientSocket.getOutputStream(),true);
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.err.println("[CLI] Client sending "+s);
        printWriter.println(s);
        printWriter.flush();
    }

    /**
     * Launches a new thread to listen for incoming messages
     */
    public void startListeningThread(MessageHandler messageHandler){
        listeningThread = new Thread(new ClientReceiver(clientSocket,messageHandler));
        listeningThread.start();
    }

    /**
     * Test purpose don't use this or I will find you, and I will kill you
     * Launches both client and server and try to send one message each way
     * This throws an exception when used in console only beacuse json is handled by android
      * @param args
     */
    public static void main(String[] args) {
        TCPServer server = new TCPServer();
        Thread serverT = new Thread(server);
        serverT.start();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        /*############################*/

        TCPClient client = new TCPClient();
        client.startListeningThread(new GameMessageHandler());

        client.send("a");

        /*W8 before closing*/
        try {
            serverT.join();
            client.listeningThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        /*
        try {
            client.clientSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

         */
    }
}

class ClientReceiver implements Runnable{
    Socket socket;
    MessageHandler handler;

    public ClientReceiver(Socket socket, MessageHandler handler){
        this.socket = socket;
        this.handler = handler;
    }

    /**
     * Listen to the socket given when building the object
     * And calls the message handler when a message is received (if null does nothing)
     */
    @Override
    public void run() {
        BufferedReader reader = null;
        StringBuffer stringBuffer = new StringBuffer();

        try {
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }
        System.err.println("[CLI] Client thread listening ...");

        for(;;){
            try {
                stringBuffer.append(reader.readLine());
                System.err.println("[CLI] Received : " + stringBuffer.toString());
                if(handler != null)handler.handle(stringBuffer.toString());
                else System.err.println("[CLI][WARN] Handler is null! Skipping handling");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}