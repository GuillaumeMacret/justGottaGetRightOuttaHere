package com.example.justgottagetrightouttahere_client.model;

/**
 * Class representing the game info in the lobby and game list
 */
public class GameInfo {
    public int id;
    public int nbPlayers;

    public GameInfo(int id) {
        this(id,0);
    }

    public GameInfo(int id, int nbPlayers) {
        this.id = id;
        this.nbPlayers = nbPlayers;
    }
}
