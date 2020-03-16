package com.example.justgottagetrightouttahere_client.network;

import com.example.justgottagetrightouttahere_client.Constants.Constants;

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

    /**
     * TCP server for tests
     */
    public TCPServer(){
        try {
            serverSocket = new ServerSocket(Constants.SERVER_PORT);
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