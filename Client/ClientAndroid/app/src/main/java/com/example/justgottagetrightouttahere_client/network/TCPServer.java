package com.example.justgottagetrightouttahere_client.network;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class TCPServer implements Runnable{
    ServerSocket serverSocket = null;
    ServerReceiver receiver = null;
    /**
     * TCP server for tests
     */
    public TCPServer(){
        try {
            serverSocket = new ServerSocket(1789);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void run(){
        Socket socket = null;
        BufferedReader reader = null;
        StringBuffer stringBuffer = new StringBuffer();

        System.err.println("[SRV] Server listening for connections");
        try {
            socket = serverSocket.accept();
            reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.err.println("[SRV] Client connected!");
        Thread t = new Thread(new ServerReceiver(socket));
        t.start();

        for(;;){
            Scanner myObj = new Scanner(System.in);  // Create a Scanner object
            System.err.println("Enter line to send");

            String line = myObj.nextLine();  // Read user input
            send(line,socket);
        }
        //System.err.println("[SRV] Client disconected!");
    }

    public void send(String s, Socket socket){
        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(socket.getOutputStream(),true);
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.err.println("[SRV] Server sending "+s);
        printWriter.println(s);
        printWriter.flush();
    }

    public static void main(String[] args) {
        TCPServer server = new TCPServer();
        server.run();
    }
}

//{"Action":"loadLevel","Blocks":[[1,1,1],[2,2,2]],"Players":[{"xPos":0,"yPos":0},{"xPos":1,"yPos":1},{"xPos":0,"yPos":23},{"xPos":0,"yPos":21}],"Bullshit":"Shitbull"}
//{"Action":"move","PosX":10,"PosY":0,"Player":0}
class ServerReceiver implements Runnable{
    Socket socket;

    public ServerReceiver(Socket socket) {
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
        System.err.println("[CLI] Server thread listening ...");

        for(;;){
            try {
                stringBuffer = new StringBuffer();
                stringBuffer.append(reader.readLine());
                if(stringBuffer.toString() !=null)System.err.println("[SRV] Received : " + stringBuffer.toString());
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}