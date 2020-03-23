package com.example.justgottagetrightouttahere_client.Constants;

public class MessageTemplates {

    public static String createMoveMessage(String direction){
        return "{\"Action\":\"move\",\"Direction\":\"" + direction +"\"}";
    }

    public static String createActionMessage(){
        return "{\"Action\":\"action\"}";
    }
}
