package com.example.justgottagetrightouttahere_client.network;

import com.example.justgottagetrightouttahere_client.Constants.Constants;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {
    ServerSocket serverSocket = null;
    public TCPServer(){
        try {
            serverSocket = new ServerSocket(Constants.SERVER_PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    void run(){
        Socket socket;
        BufferedReader input = null;
        System.err.println("Server listening for connections");
        try {
            socket = serverSocket.accept();
            input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.err.println("Client connected!");

        String read=null;
        for(;;){
            try {

                read = input.readLine();

                if(read!=null)System.err.println("Read from client : " + read);
                /*
                BufferedWriter out = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
                    out.write("TstMsg");
                    updateConversationHandler.post(new updateUIThread(read));
                */
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        TCPServer server = new TCPServer();
        server.run();
    }
}