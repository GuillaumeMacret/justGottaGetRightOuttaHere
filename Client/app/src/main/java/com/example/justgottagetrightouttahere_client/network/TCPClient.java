package com.example.justgottagetrightouttahere_client.network;

import com.example.justgottagetrightouttahere_client.Constants.Constants;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;

public class TCPClient {
    Socket clientSocket;

    public TCPClient(){
        try {
            clientSocket =  new Socket(Constants.SRVER_ADDRESS,Constants.SERVER_PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void send(String s){
        PrintWriter printWriter = null;
        try {
            printWriter = new PrintWriter(clientSocket.getOutputStream(),true);
        } catch (IOException e) {
            e.printStackTrace();
        }
        printWriter.write(s);
        printWriter.flush();
        printWriter.close();
    }

    public static void main(String[] args) {
        TCPClient client = new TCPClient();
    }
}

class ClientReceiveThread implements Runnable{

    @Override
    public void run() {

    }
}