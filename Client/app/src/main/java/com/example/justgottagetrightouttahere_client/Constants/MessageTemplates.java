package com.example.justgottagetrightouttahere_client.Constants;

public class MessageTemplates {

    public static String createMoveActionMessage(String direction){
        return "{\"Action\":\"move\",\"Direction\":\"" + direction +"\"}";
    }
}
