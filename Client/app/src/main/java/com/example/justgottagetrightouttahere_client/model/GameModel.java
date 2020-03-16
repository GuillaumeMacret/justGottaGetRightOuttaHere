package com.example.justgottagetrightouttahere_client.model;

import android.util.Log;

import java.util.ArrayList;
import java.util.List;

/**
 * Class representing the game board and its players
 * This is NOT including the lobby
 */
public class GameModel {
    public int sizeX, sizeY;
    public int [][] gameMatrix;
    public List<Player> players;

    /**
     * Creates the game with its matrix according to size given in parameter
     * @param sizeX
     * @param sizeY
     */
    public GameModel(int sizeX, int sizeY){
        this.sizeX = sizeX;
        this.sizeY = sizeY;
        gameMatrix = new int[sizeX][sizeY];
        players = new ArrayList<>();
    }

    public void movePlayer(int playerId, int xPos, int yPos){
        Log.e("INFO","Game model : Moving player "+playerId);
        players.get(playerId).posX = xPos;
        players.get(playerId).posY = yPos;
    }

    public void updateTiles(List<Tile>tiles){
        for(Tile t : tiles){
            gameMatrix[t.posX][t.posY] = t.spriteId;
        }
    }
}
