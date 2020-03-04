package com.example.justgottagetrightouttahere_client.model;

import com.example.justgottagetrightouttahere_client.network.MessageHandler;

public class GameMessageHandler implements MessageHandler {
    @Override
    public void handle(String s){
        System.err.println("Handle message <"+s+"> TODO");
    }
}
