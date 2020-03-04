package com.example.justgottagetrightouttahere_client.network;

import com.example.justgottagetrightouttahere_client.Constants.Constants;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class TCPClient{
    Socket clientSocket;

    /**
     * Connects the socket to default port and adress given in Constants file
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
     * Test purpose don't use this
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
        Thread clientT = new Thread(new ClientReceiver(client.clientSocket,null));
        clientT.start();

        client.send("a");

        /*W8 before closing*/
        try {
            serverT.join();
            clientT.join();
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
        System.err.println("[CLI] Client listening ...");

        for(;;){
            try {
                stringBuffer.append(reader.readLine());
                System.err.println("[CLI] Received : " + stringBuffer.toString());
                if(handler != null)handler.handle(stringBuffer.toString());
            } catch (IOException e) {
                e.printStackTrace();
                System.exit(1);
            }
        }
    }
}