package com.example.justgottagetrightouttahere_client.model;

import com.example.justgottagetrightouttahere_client.network.MessageHandler;

import org.json.JSONException;
import org.json.JSONObject;

public class GameMessageHandler implements MessageHandler {

    /**
     * Handles the message in the game board context
     * TODO
     * @param s
     */
    @Override
    public void handle(String s){
        System.err.println("Handling message < "+s+" > TODO");
        try {
            JSONObject jsonObject = new JSONObject("{\"Action\":\"TestAction\"}");
            //System.out.println("Action : "+jsonObject.getString("Action"));
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }
}
