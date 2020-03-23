package com.example.justgottagetrightouttahere_client.model;

/**
 * Class representing the game info in the lobby and game list
 */
public class GameInfo {
    public String id;
    public int nbPlayers;

    public GameInfo(String id) {
        this(id,0);
    }

    public GameInfo(String id, int nbPlayers) {
        this.id = id;
        this.nbPlayers = nbPlayers;
    }
}
