package com.example.justgottagetrightouttahere_client.Constants;

public class MessageTemplates {

    public static String createMoveMessage(String direction){
        return "{\"Action\":\"move\",\"Direction\":\"" + direction +"\"}";
    }

    public static String createGameListMessage() { return "{\"Action\":\"gameList\"}"; }

    public static String createActionMessage(){
        return "{\"Action\":\"action\"}";
    }

    public static String createGameMessage() { return "{\"Action\":\"createGame\"}"; }

    public static String createJoinGameMessage(int gameId) { return "{\"Action\":\"joinGame\",\"GameId\":"+ gameId +"}"; }

    public static String createChangeMapMessage(String map) { return "{\"Action\":\"changeMap\",\"Map\":\""+ map +"\"}"; }

    public static String createChangeRoleMessage(int playerId, int role) { return "{\"Action\":\"roleChange\",\"RoleId\":"+ role +"}"; }

    public static String createStartGameMessage() { return "{\"Action\":\"startGame\"}"; }
}
