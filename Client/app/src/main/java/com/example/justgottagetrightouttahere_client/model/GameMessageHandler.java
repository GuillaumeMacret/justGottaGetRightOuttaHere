package com.example.justgottagetrightouttahere_client.model;

import android.util.Log;

import com.example.justgottagetrightouttahere_client.network.MessageHandler;

import org.json.JSONException;
import org.json.JSONObject;

public class GameMessageHandler implements MessageHandler {

    GameModel model;

    public GameMessageHandler(GameModel model){
        this.model = model;
    }

    /**
     * Handles the message in the game board context
     * TODO
     * @param s
     */
    @Override
    public void handle(String s){
        System.err.println("Handling message < "+s+" > TODO");
        try {
            JSONObject jsonObject = new JSONObject(s);
            Log.e("INFO","Action : "+jsonObject.getString("Action"));

            switch (jsonObject.getString("Action")){
                case "move":
                    movePlayerAction(jsonObject);
                    break;
                default:
                    Log.e("ERROR","Game handler can't handle action "+jsonObject.getString("Action"));
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    void movePlayerAction(JSONObject jsonObject){
        try {
            int playerId = jsonObject.getInt("Player");
            int posX = jsonObject.getInt("PosX");
            int posY = jsonObject.getInt("PosY");

            model.movePlayer(playerId,posX,posY);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
