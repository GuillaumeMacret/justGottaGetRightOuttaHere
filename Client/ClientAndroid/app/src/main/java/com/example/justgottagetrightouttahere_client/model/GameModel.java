package com.example.justgottagetrightouttahere_client.model;

import android.util.Log;

import com.example.justgottagetrightouttahere_client.network.TCPClient;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;
import java.util.Timer;

import static com.example.justgottagetrightouttahere_client.Constants.Constants.DEFAULT_SIZE_X;
import static com.example.justgottagetrightouttahere_client.Constants.Constants.DEFAULT_SIZE_Y;

/**
 * Class representing the game board and its players
 * This is NOT including the lobby
 */
public class GameModel {
    /**Singleton**/
    private static GameModel INSTANCE = null;
    public static GameModel getInstance() {
        if (INSTANCE == null) {
            INSTANCE = new GameModel(DEFAULT_SIZE_X,DEFAULT_SIZE_Y);
        }
        return INSTANCE;
    }

    public int sizeX, sizeY;
    public int [][] blocksLayer;
    public int [][] objectLayer;
    public List<Player> players;
    public String levelName = "No name";
    public long startTime = System.currentTimeMillis();

    /**
     * Creates the game with its matrix according to size given in parameter
     * @param sizeX
     * @param sizeY
     */
    public GameModel(int sizeX, int sizeY){
        this.sizeX = sizeX;
        this.sizeY = sizeY;
        blocksLayer = new int[sizeX][sizeY];
        objectLayer = new int[sizeX][sizeY];
        players = new ArrayList<>();
    }

    public void movePlayer(int playerId, int xPos, int yPos){
        Log.e("INFO","Game model : Moving player "+playerId);
        players.get(playerId).posX = xPos;
        players.get(playerId).posY = yPos;
    }

    public void updateObjectLayer(List<Tile>tiles){
        for(Tile t : tiles){
            objectLayer[t.posX][t.posY] = t.spriteId;
        }
    }

    public void loadLevel(int blocks[][]){
        Log.e("INFO","Loading a new level");
        sizeX = blocks.length;
        sizeY = blocks[0].length;
        blocksLayer = new int[sizeX][sizeY];
        for(int i = 0; i < blocks.length; ++i){
            for(int j = 0; j < blocks[i].length; ++j){
                blocksLayer[i][j] = blocks[i][j];
            }
        }
    }

}
